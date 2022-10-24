#include "App.h"

class WizardsApp :public App
{
	void onRender(float delta) override
	{
		std::wstring w = L"FrameTime: ";
		w += std::to_wstring(delta);
		w += L" seconds";

		m_RootWindow.setName(w.c_str());
	}
};

int main()
{
	WizardsApp application;
	return application.run();
}

