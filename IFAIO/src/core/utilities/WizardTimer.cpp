#include "WizardTimer.h"

namespace IFAIO
{

	WizardTimer::WizardTimer()
	{

	}

	void WizardTimer::start()
	{
		m_Last = std::chrono::high_resolution_clock::now();
		m_IsWorking = true;
	}

	void WizardTimer::stop()
	{
		m_IsWorking = false;
	}

	float WizardTimer::mark()
	{
		if (!m_IsWorking)
		{
			return 0.0;
		}
		auto now = std::chrono::high_resolution_clock::now();
		auto time = now - m_Last;
		m_Last = now;
		return time.count() * 1e-9;

	}

	float WizardTimer::peak()
	{
		if (!m_IsWorking)
		{
			return 0.0;
		}
		return (std::chrono::high_resolution_clock::now() - m_Last).count();
	}

}


