#include "Vec2.h"

const int Vec2::AXIS_X = 0;
const int Vec2::AXIS_Y = 1;
const Vec2 Vec2::ZERO = {};
const Vec2 Vec2::ONE = { 1,1 };
const Vec2 Vec2::UP = { 0,-1 };
const Vec2 Vec2::LEFT = { -1,0 };
const Vec2 Vec2::RIGHT = { 1,0 };
const Vec2 Vec2::DOWN = { 0,1 };




Vec2 operator+(const Vec2& v, float c)
{
    return Vec2(v.x + c, v.y + c);
}
Vec2 operator-(const Vec2& v, float c)
{
    return Vec2(v.x - c, v.y - c);
}
Vec2 operator*(const Vec2& v, float c)
{
    return Vec2(v.x * c, v.y * c);
}
Vec2 operator/(const Vec2& v, float c)
{
    return Vec2(v.x / c, v.y / c);
}

Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x + v2.x, v1.y + v2.y);
}

Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 operator*(const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x * v2.x, v1.y * v2.y);
}
Vec2 operator/(const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x / v2.x, v1.y / v2.y);
}
