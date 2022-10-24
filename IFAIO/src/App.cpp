#include "App.h"

void App::setup()
{
	timer.start();
	return onSetup();
}

void App::input()
{
	onInput();
}

void App::render()
{
	return onRender(timer.mark());
}

void App::pollEvents()
{
	bool isEmpty = 0;
	m_RootWindow.processMessages(isEmpty);
	while (isEmpty)
	{
		input();
		auto res = m_RootWindow.processMessages(isEmpty);
		if (res.has_value())
		{
			m_ErrorCode = res.value();
			m_shouldQuit = true;
			return;
		}
			
	}
}

App::App()
	:m_RootWindow(500,500,L"WizardWindow")
{

}

int App::run()
{
	try {
		setup();
		while (!m_shouldQuit)
		{
			render();
			pollEvents();
		}
		return m_ErrorCode;
	}
	catch (const WizardException& e)
	{
		MessageBox(nullptr, e.whatW(), e.GetTypeW(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}

