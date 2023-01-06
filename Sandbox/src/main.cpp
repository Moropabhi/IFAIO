#include "App.h"
#include "core/Input/InputSystem.h"
#include "core/dataStructs/Storage.h"

class WizardsApp :public IFAIO::App
{
	float r=0.5, g=0, b=0.5;
	float mul = 0;
	unsigned int inputs = 0;

	void onSetup() override
	{
		m_RootWindow.setName(L"Click on the white!");
		auto& gfx = m_RootWindow.getGraphics();
		
	}

	void onRender(float delta) override
	{
		using namespace IFAIO;
		auto& gfx=m_RootWindow.getGraphics();
		m_RootWindow.setName(L"FPS : "+std::to_wstring(1/delta));
		gfx.ClearBuffer(r, g, b);
		mul += delta;
		auto v = InputSystem::getMousePos(this);
		v /= {m_RootWindow.getWidth(),m_RootWindow.getHeight()};
		v *= 2;
		v -= Vec2::ONE;
		gfx.DrawTestCube(mul, v.x,-v.y);
	}

	void onInput(IFAIO::EventType e) override
	{
		using namespace IFAIO;

		static std::wstring w = L"Text: ";

		
		switch (e)
		{
		case MOUSE_LEFT_BUTTON_PRESSED:
			
			inputs += 1;
			break;
		case KEY_RELEASE:
			w += InputSystem::getTextW(this);
			m_RootWindow.setName(w);
		}
		
	}
};

int main()
{
		WizardsApp application;

		return application.run();
	

	
}