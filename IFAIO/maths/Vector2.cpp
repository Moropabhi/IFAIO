#include "Vector2.h"

const int Vector2::AXIS_X=0;
const int Vector2::AXIS_Y=1;
const Vector2 Vector2::ZERO={};
const Vector2 Vector2::ONE={1,1};
const Vector2 Vector2::UP={0,-1};
const Vector2 Vector2::LEFT={-1,0};
const Vector2 Vector2::RIGHT={1,0};
const Vector2 Vector2::DOWN={0,1};


Vector2::Vector2()
    :glm::vec2()
{
}
Vector2::Vector2(double x)
    :Vector2(x,x)
{
}
Vector2::Vector2(glm::vec2 v)
    :glm::vec2(v)
{
}

Vector2::Vector2(double X, double Y)
    :glm::vec2(X,Y)
{
}


double Vector2::lengthSquared() const
{
    return x * x + y * y;
}

Vector2 Vector2::normalise()
{
    return Vector2(x/length(),y/length());
}

bool Vector2::is_normalised()  const
{
    return lengthSquared() == 1;
}

Vector2 Vector2::abs() const
{
    return Vector2(std::abs(x),std::abs(y));
}

Vector2 operator+(const Vector2 v, double c)
{
    return Vector2(v.x + c, v.y + c);
}
Vector2 operator-(const Vector2 v, double c)
{
    return Vector2(v.x - c, v.y - c);
}
Vector2 operator*(const Vector2 v, double c)
{
    return Vector2(v.x * c, v.y * c);
}
Vector2 operator/(const Vector2 v, double c)
{
    return Vector2(v.x / c, v.y / c);
}