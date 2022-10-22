#pragma once
#include "WizardWindowsAPI.h"
#include "Utilities.h"
#include "Utilities/WizardException.h"
#include "Keyboard.h"
#include "Mouse.h"


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


	void setName(const Char* c);

public:
	Keyboard m_Keyboard;
	Mouse m_Mouse;
	WindowProperties specs;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private :
	int m_Width;
	int m_Height;
	HWND m_Handler;
	class WindowClass;
	class Exception;
};

class Window::WindowClass
{
public:
	WindowClass() ;
	~WindowClass();
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;

	static const Char* GetName();
	static HINSTANCE GetInstance();

private:
	static constexpr const Char* s_WindowNameCls = L"Abhisek's Window";
	static WindowClass s_WindowClass;
	HINSTANCE m_Instance;
};

class Window::Exception :  public WizardException
{
public:
	Exception(int line, const char* file, HRESULT hr) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	virtual const wchar_t* GetTypeW() const noexcept;
	static std::string TranslateErrorCode(HRESULT hr) noexcept;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;
private:
	HRESULT hr;
};

#define WND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define WND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )