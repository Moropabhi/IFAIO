#pragma once

#include "Vec2.h"

class Rect {
public:
	Vec2 minimum = Vec2::ZERO;
	Vec2 maximum = Vec2::ONE;

	Rect() = default;
	Rect(Vec2 position, Vec2 size)
		:minimum(position), maximum(position + size)
	{}
	Rect(float x0, float y0, float x1, float y1)
		: minimum(x0, y0), maximum(x1, y1)
	{ }

	bool isInside(Vec2 point)
	{
		return minimum < point&& point < maximum;
	}

};


inline std::ostream& operator<<(std::ostream& out, const Rect& r)

{
	return out << "Rect2 { \nminimum :" << r.minimum << " ,\nmaximum :" << r.maximum << "\n}";
}