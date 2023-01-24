#pragma once

#include "App.h"
#include "core/debug/LogSystem.h"

class Box : public IFAIO::Object
{
public:
	Box()
	{
		LOG("Box created");
	}
	void onRender(float delta) override
	{
		LOG_VAR(delta);
	}

	~Box()

	{
		LOG("Box destroyed");
	}
};