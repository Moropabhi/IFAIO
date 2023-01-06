#include "Window.h"
#include "core/debug/WizardException.h"

#include "Wizardwin32.h"
#include "../resource.h"


namespace IFAIO
{

	Window::WindowClass  Window::WindowClass::s_WindowClass;

	Window::Window(int width, int height, const Char* name)
		:m_Width(width), m_Height(height)
	{
		// calculate window size based on desired client region size
		RECT windowRect;
		windowRect.left = 100;
		windowRect.right = width + windowRect.left;
		windowRect.top = 100;
		windowRect.bottom = height + windowRect.top;
		if (!AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
		{
			throw WND_LAST_EXCEPT();
		};
		// create window & get hWnd
		m_Handler = CreateWindow(
			WindowClass::GetName(), name,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			nullptr, nullptr, WindowClass::GetInstance(), this
		);

		if (!m_Handler)
		{
			throw WND_LAST_EXCEPT();
		}

		p_Graphics = std::make_unique<Graphics>(m_Handler);
		// show window
		ShowWindow(m_Handler, SW_SHOWDEFAULT);
	}


	Window::~Window()
	{
		DestroyWindow(m_Handler);
	}

	void Window::setName(const std::wstring c)
	{
		SetWindowText(m_Handler, c.c_str());
	}

	std::optional<int> Window::processMessages(bool& isQueueEmpty, EventType& e)
	{
		MSG msg;
		isQueueEmpty = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		e = m_eventType;
		return {};
	}

	LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			// forward message to window class handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// retrieve ptr to window class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{

		m_eventType = EventType::UNKNOWM;
		switch (msg)
		{
			// we don't want the DefProc to handle this message because
			// we want our destructor to destroy the window, so return 0 instead of break
		case WM_CLOSE:
			PostQuitMessage(0);
			m_eventType = EventType::WINDOW_CLOSE;
			return 0;
			// clear keystate when window loses focus to prevent input getting "stuck"
		case WM_KILLFOCUS:
			m_Keyboard.clearState();
			break;

		}
		m_Keyboard.getAnyEvent(msg, wParam, lParam, m_eventType);
		m_Mouse.getAnyEvent(hWnd, msg, wParam, lParam, m_Width, m_Height, m_eventType);

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	Window::WindowClass::WindowClass()
		:m_Instance(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = static_cast<HICON>(LoadImage(
			GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 64, 64, 0
		));
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();
		wc.hIconSm = static_cast<HICON>(LoadImage(
			GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 32, 32, 0
		));
		RegisterClassEx(&wc);
	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(GetName(), GetInstance());
	}

	const Char* Window::WindowClass::GetName()
	{
		return s_WindowNameCls;
	}

	HINSTANCE Window::WindowClass::GetInstance()
	{
		return s_WindowClass.m_Instance;
	}

	Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
		:WizardException::WizardException(line, file), hr(hr)
	{
	}

	const char* Window::Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << getType() << std::endl
			<< "[Error Code] " << getErrorCode() << std::endl
			<< "[Description] " << getErrorString() << std::endl
			<< getOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Window::Exception::getType() const noexcept
	{
		return "WizardWindowException";
	}

	std::string Window::Exception::translateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		// windows will allocate memory for err string and make our pointer point to it
		DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
		);
		// 0 string length returned indicates a failure
		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		// copy error string from windows-allocated buffer to std::string
		std::string errorString = pMsgBuf;
		// free windows buffer
		LocalFree(pMsgBuf);
		return errorString;
	}

	HRESULT Window::Exception::getErrorCode() const noexcept
	{
		return hr;
	}

	std::string Window::Exception::getErrorString() const noexcept
	{
		return translateErrorCode(hr);
	}

}