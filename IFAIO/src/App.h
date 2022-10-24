#pragma once
#include "Core/window/Window.h"
#include "Utilities/WizardTimer.h"
#include "Utilities/WizardException.h"

struct AppSpecification 
{

};




class App
{
public:
	//App(AppSpecification& spec);
	App();
	//~App();
	App(const App& app) = delete;
	App&  operator=(const App& app) = delete;
	virtual void onSetup(){}
	virtual void onRender(float delta){}
	virtual void onInput(){}
	int run();
public:
	Window m_RootWindow;
private:
	void setup();
	void input();
	void render();
	void pollEvents();

private:
	WizardTimer timer;
	bool m_shouldQuit = false;
	int m_ErrorCode = 0;
};

