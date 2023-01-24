#pragma once
#include "App.h"
#include"Box.h"
class WizardsApp :public IFAIO::App
{
	float r = 1, g = 1, b = 1;
	int mul = 1;
	unsigned int inputs = 0;

	void onSetup() override
	{
		using namespace IFAIO;
		Ref<Object> o = std::make_shared<Box>();

		Bind(o);

		
	}

	void onRender(float delta) override
	{
	}

	void onInput(IFAIO::EventType e) override
	{
	}
};
