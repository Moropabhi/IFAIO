#include "Transformation2D.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Transformation2D& Transformation2D::setTranslation(Vector2& t)
{
	translation = t;
	return *this;
}

Transformation2D& Transformation2D::setRotation(float r)
{
	rotation = r;
	return *this;
}

Transformation2D& Transformation2D::setScale(Vector2& s)
{
	scalar = s;
	return *this;
}

Transformation2D& Transformation2D::translate(Vector2& t)
{
	translation += t;
	return *this;
}

Transformation2D& Transformation2D::rotate(float r)
{
	rotation += r;
	return *this;
}

Transformation2D& Transformation2D::scale(Vector2& s)
{
	scalar += s;
	return *this;
}

Vector2 Transformation2D::getTranslation() const
{
	return translation;
}

float Transformation2D::getRotation() const
{
	return rotation;
}

Vector2 Transformation2D::getScale() const
{
	return scalar;
}

glm::mat3 Transformation2D::getTransformation() const
{
	glm::mat3 m(1.0);
	m = glm::translate(m, translation);
	m = glm::rotate(m, rotation);
	m = glm::scale(m, scalar);
	return m;
}

glm::mat4 Transformation2D::getTransformationMat4() const
{
	glm::mat4 transformation(1.0);
	transformation = glm::translate(transformation, glm::vec3{ translation.x,translation.y,1 });
	transformation = glm::rotate(transformation, rotation, glm::vec3{0,0,1});
	transformation = glm::scale(transformation, glm::vec3{ scalar.x,scalar.y,1 });
	return transformation;
}
