#pragma once

#include "Vector2.h"
#include "glm/gtx/matrix_transform_2d.hpp"

struct Transformation2D {
public:
	Vector2 translation=Vector2::ZERO;
	float rotation=0.0f;
	Vector2 scalar=Vector2::ONE;

	//Transformation func
	Transformation2D& setTranslation(Vector2& t);
	Transformation2D& setRotation(float r);
	Transformation2D& setScale(Vector2& s);
	Transformation2D& setTranslation(Vector2&& t) { return setTranslation(t); };
	Transformation2D& setScale(Vector2&& s) { return setScale(s); };

	Transformation2D& translate(Vector2& t);
	Transformation2D& rotate(float r);
	Transformation2D& scale(Vector2& s);
	Transformation2D& translate(Vector2&& t) { return translate(t); };
	Transformation2D& scale(Vector2&& s) { return scale(s); };

	Vector2 getTranslation() const;
	float getRotation() const;
	Vector2 getScale() const;

	glm::mat3 getTransformation() const;
	glm::mat4 getTransformationMat4() const;

	
};