#pragma once

#include "core/math/Vec2.h"
#include "VertexLayout.h"


namespace IFAIO
{
	struct Vertex2D 
	{

		Vec2 Position;
		float r, g, b;

		static void setVertexLayout(VertexLayout& vl ) noexcept
		{
			vl.Push<float>("Position", 2);
			vl.Push<float>("Colour", 2);
		}
	};
}