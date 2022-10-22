#pragma once
#include "Window.h"
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

	virtual void onSetup();
	virtual void onRender(float delta);
	virtual void onInput();
	int run();
private:
	void setup();
	void render();

private:
	Window m_RootWindow;
	WizardTimer timer;
	MSG msg;
};

