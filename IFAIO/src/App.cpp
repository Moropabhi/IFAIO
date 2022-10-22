#include "App.h"

void App::setup()
{
	timer.start();
	return onSetup();
}

void App::render()
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	return onRender(timer.mark());
}

App::App()
	:m_RootWindow(500,500,L"WizardWindow")
{

}

void App::onSetup()
{
}

void App::onRender(float delta)
{
	std::wstring w = L"FrameRate: ";
	w += std::to_wstring(delta);

	m_RootWindow.setName(w.c_str());
}

void App::onInput()
{
}

int App::run()
{
	try {
		setup();

		while (GetMessageW(&msg, NULL, 0, 0))
		{
			render();
		}
		return 0;
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
#include "Wizardwin32.h"
