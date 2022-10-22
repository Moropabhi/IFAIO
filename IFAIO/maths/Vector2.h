#pragma once

#include "glm/glm.hpp"
#include "iostream"
struct Vector2 :public glm::vec2{
public:

	static const int AXIS_X;
	static const int AXIS_Y;
	static const Vector2 ZERO;
	static const Vector2 ONE;
	static const Vector2 UP;
	static const Vector2 LEFT;
	static const Vector2 RIGHT;
	static const Vector2 DOWN;

	Vector2();
	Vector2(double x);
	Vector2(double x, double y);
	Vector2(glm::vec2 v);

	double lengthSquared() const;
	Vector2 normalise();
	bool is_normalised() const;
	Vector2 abs() const;

	Vector2 operator+=(Vector2 v)
	{
		return glm::vec2(*this) += glm::vec2(v);
	}
	Vector2 operator-=(Vector2 v)
	{
		return glm::vec2(*this) -= glm::vec2(v);
	}
	Vector2 operator*=(Vector2 v)
	{
		return glm::vec2(*this) *= glm::vec2(v);
	}
	Vector2 operator/=(Vector2 v)
	{
		return glm::vec2(*this) /= glm::vec2(v);
	}
	

	bool operator<(Vector2 v)
	{
		return x < v.x&& y < v.y;
	}
	bool operator>(Vector2 v)
	{
		return x > v.x&& y > v.y;
	}
	bool operator<=(Vector2 v)
	{
		return x <= v.x&& y <= v.y;
	}
	bool operator>=(Vector2 v)
	{
		return x >= v.x&& y >= v.y;
	}
	
	operator glm::vec2()
	{
		return glm::vec2(x, y);
	}
};

inline std::ostream& operator<<(std::ostream& out, const Vector2& v)
{
	return out << "Vector2 ( " << v[0] << " , " << v[1] << " )";
}
Vector2 operator+(const Vector2 v, double c);
Vector2 operator-(const Vector2 v, double c);
Vector2 operator*(const Vector2 v, double c);
Vector2 operator/(const Vector2 v, double c);