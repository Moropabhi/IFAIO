#pragma once
#include "core/window/Window.h"
#include "core/Input/InputSystem.h"
#include "core/Utilities/WizardTimer.h"
#include "core/debug/WizardException.h"

namespace IFAIO
{
	class App
	{
	public:
		//App(AppSpecification& spec);
		App();
		//~App();
		App(const App& app) = delete;
		App& operator=(const App& app) = delete;
		virtual void onSetup() {}
		virtual void onRender(float delta) {}
		virtual void onInput(EventType e) {}
		int run();
	public:
		Window m_RootWindow;
	private:
		void setup();
		void input(EventType e);
		void render();
		void pollEvents();

	private:
		WizardTimer timer;
		bool m_shouldQuit = false;
		int m_ErrorCode = 0;
	};

}
