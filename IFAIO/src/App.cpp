#include "App.h"
#include "core/mesh/MeshSystem.h"

namespace IFAIO
{
	void App::setup()
	{
		timer.start();
		onSetup();
		for (auto& i : objects)
			i->onSetup();
		auto& gfx = m_RootWindow.getGraphics();

		MeshSystem::Create(gfx.getDevice());
	}

	void App::input(EventType e)
	{
		onInput(e);
		for (auto& i : objects)
			i->onInput(e);
	}

	void App::render()
	{
		auto delta = timer.mark();
		onRender(delta);
		auto& gfx = m_RootWindow.getGraphics();
		for (auto& i : objects)
			i->onRender(delta);
		MeshSystem::Draw(gfx);

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
#ifndef NDEBUG


		try
		{
#endif 
			setup();
			while (!m_shouldQuit)
			{
				render();
				pollEvents();
			}
			return m_ErrorCode;
#ifndef NDEBUG
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
#endif

	}
	void App::Bind(Ref<Object> o)
	{
		objects.push_back(o);
	}
}