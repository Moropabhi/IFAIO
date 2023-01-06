#pragma once
#include <iostream>

struct Vec2 {
public:

	static const int AXIS_X;
	static const int AXIS_Y;
	static const Vec2 ZERO;
	static const Vec2 ONE;
	static const Vec2 UP;
	static const Vec2 LEFT;
	static const Vec2 RIGHT;
	static const Vec2 DOWN;

	Vec2()
		:x(0.0f), y(0.0f)
	{
	}
	Vec2(float x)
		: Vec2(x, x)
	{
	}
	Vec2(float X, float Y)
		: x(X), y(Y)
	{
	}
	Vec2(int X, int Y)
		: x(X), y(Y)
	{
	}

	float lengthSquared() const
	{
		return x * x + y * y;
	}
	float length() const
	{
		return sqrt(lengthSquared());
	}
	Vec2 normalise()
	{
		return Vec2(x / length(), y / length());
	}
	bool is_normalised() const
	{
		return lengthSquared() == 1;
	}
	Vec2 abs() const
	{
		return Vec2(std::abs(x), std::abs(y));
	}

	Vec2 operator+=(Vec2 v)
	{
		x += v.x;
		y += v.y;
		return *this;

	}

	Vec2 operator-=(Vec2 v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2 operator*=(Vec2 v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vec2 operator/=(Vec2 v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	float& operator[](int i)
	{

		return ((float*)this)[i];
	}

	const float& operator[](int i) const
	{
		return ((float*)this)[i];
	}

	bool operator<(Vec2 v)

	{
		return x < v.x&& y < v.y;
	}

	bool operator>(Vec2 v)

	{
		return x > v.x && y > v.y;
	}

	bool operator<=(Vec2 v)

	{
		return x <= v.x && y <= v.y;
	}

	bool operator>=(Vec2 v)

	{
		return x >= v.x && y >= v.y;
	}
	
	float x, y;
};

inline std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
	return out << "Vec2 ( " << v[0] << " , " << v[1] << " )";
}
Vec2 operator+(const Vec2& v, float c);
Vec2 operator-(const Vec2& v, float c);
Vec2 operator*(const Vec2& v, float c);
Vec2 operator/(const Vec2& v, float c);
Vec2 operator+(const Vec2& v1, const Vec2& v2);
Vec2 operator-(const Vec2& v1, const Vec2& v2);
Vec2 operator*(const Vec2& v1, const Vec2& v2);
Vec2 operator/(const Vec2& v1, const Vec2& v2);