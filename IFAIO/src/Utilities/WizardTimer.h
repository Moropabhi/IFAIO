#pragma once

#include <chrono>

class WizardTimer
{
public:
	WizardTimer();

	void start();
	void stop();

	float mark();
	float peak();
private:
	std::chrono::high_resolution_clock::time_point m_Last;
	bool m_IsWorking=false;
};