#pragma once
#include "Vector2.h"
class Rect2D {
public:
	Vector2 minimum=Vector2::ZERO;
	Vector2 maximum=Vector2::ONE;

	Rect2D(){}
	Rect2D(Vector2 position,Vector2 size)
		:minimum(position), maximum(position + size)
	{ }
	Rect2D(float x0,float y0,float x1,float y1)
		:minimum(x0, y0), maximum(x1,y1)
	{ }

	bool isInside(Vector2 point);

};


inline std::ostream& operator<<(std::ostream& out, const Rect2D& r)
{
	return out << "Rect2 { \nminimum :" << r.minimum << " ,\nmaximum :" << r.maximum << "\n}";
}