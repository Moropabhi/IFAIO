#pragma once
#include "WizardWindowsAPI.h"
#include "core/dataStructs/Utilities.h"
#include "Core/graphics/Graphics.h"
#include "Core/Input/Keyboard.h"
#include "Core/Input/Mouse.h"

#include <optional>

namespace IFAIO
{

	struct WindowProperties
	{

		int width; int height; const Char* name;
	};

	class Window
	{
	public:
		Window(int width, int height, const Char* name);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;


		void setName(const std::wstring c);
		std::optional<int> processMessages(bool& isQueueEmpty, EventType& e);

		Graphics& getGraphics() { return *p_Graphics; }
		float getWidth() { return m_Width; }
		float getHeight() { return m_Height; }
	public:
		Keyboard m_Keyboard;
		Mouse m_Mouse;
		//WindowProperties specs;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		int m_Width;
		int m_Height;
		EventType m_eventType;
		HWND m_Handler;
		Ref<Graphics> p_Graphics;
		class WindowClass;
	public:
		class Exception;
	};

	class Window::WindowClass
	{
	public:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static const wchar_t* GetName();
		static HINSTANCE GetInstance();

	private:
		static constexpr const wchar_t* s_WindowNameCls = L"Abhisek's Window";
		static WindowClass s_WindowClass;
		HINSTANCE m_Instance;
	};
	class Window::Exception : public WizardException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* getType() const noexcept override;
		static std::string translateErrorCode(HRESULT hr) noexcept ;
		HRESULT getErrorCode() const noexcept override;
		std::string getErrorString() const noexcept override;
	private:
		HRESULT hr;
	};

}

#define WND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define WND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )

