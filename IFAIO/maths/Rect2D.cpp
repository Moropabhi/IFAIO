#include "Rect2D.h"

bool Rect2D::isInside(Vector2 point)
{
    return minimum<point && point < maximum;
}
