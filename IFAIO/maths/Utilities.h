#pragma once

double clamp(double value, double low, double high)
{
	if (value < low)
	{
		return low;
	}
	else {
		return value > high ? high : value;
	}
}