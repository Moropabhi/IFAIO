#include "App.h"

class WizardsApp :public IFAIO::App
{
	float r=1, g=1, b=1;
	int mul = 1;
	unsigned int inputs = 0;

	void onSetup() override
	{
		m_RootWindow.setName(L"Click on the white!");
	}

	void onRender(float delta) override
	{
		m_RootWindow.getGraphics().ClearBuffer(r, g, b);
	}

	void onInput(IFAIO::EventType e) override
	{
		using namespace IFAIO;

		static std::wstring w = L"Text: ";

		
		switch (e)
		{
		case MOUSE_LEFT_BUTTON_PRESSED:
			r -= 0.1;
			b -= 0.1;
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

