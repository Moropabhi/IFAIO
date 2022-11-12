#include "App.h"

namespace IFAIO
{

	void App::setup()
	{
		timer.start();
		return onSetup();
	}

	void App::input(EventType e)
	{
		onInput(e);
	}

	void App::render()
	{
		onRender(timer.mark());

		return m_RootWindow.getGraphics().EndFrame();
	}

	void App::pollEvents()
	{
		bool isEmpty = 0;
		EventType e;
		m_RootWindow.processMessages(isEmpty, e);
		while (isEmpty)
		{
			input(e);
			auto res = m_RootWindow.processMessages(isEmpty, e);
			if (res.has_value())
			{
				m_ErrorCode = res.value();
				m_shouldQuit = true;
				return;
			}

		}
	}

	App::App()
		:m_RootWindow(500, 500, L"WizardWindow")
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

}