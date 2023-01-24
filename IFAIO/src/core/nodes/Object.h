#pragma once
#include "core/debug/WizardException.h"
#include "core/Input/InputSystem.h"

namespace IFAIO {
	class Object
	{
	public:
		Object() = default;
		virtual ~Object() = default;
		virtual void onSetup() {}
		virtual void onRender(float delta) {}
		virtual void onInput(EventType e) {}
	};
}
