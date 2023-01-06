#include "App.h"
#include "core/mesh/MeshSystem.h"

namespace IFAIO
{

	void App::setup()
	{
		timer.start();
		onSetup();
		//auto& gfx = m_RootWindow.getGraphics();
		//MeshSystem::Create(gfx.getDevice());
	}

	void App::input(EventType e)
	{
		onInput(e);
	}

	void App::render()
	{
		onRender(timer.mark());
		//auto& gfx = m_RootWindow.getGraphics();
		//MeshSystem::Draw(gfx.getDevice(), gfx.getContext(), gfx.getTarget());
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
		:m_RootWindow(800, 600, L"WizardWindow")
	{

	}

	int App::run()
	{
		try
		{
			setup();
			while (!m_shouldQuit)
			{
				render();
				pollEvents();
			}
			return m_ErrorCode;
		}

		catch (const IFAIO::WizardException& e)
		{
			LOG(e.what())
			MessageBoxA(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
		}
		catch (const std::exception& e)
		{
			LOG(e.what())
			MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (...)
		{
			MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
		}
		return -1;

	}
}