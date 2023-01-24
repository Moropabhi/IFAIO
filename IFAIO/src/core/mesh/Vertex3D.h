#pragma once

#include "core/math/Vec2.h"
#include "VertexLayout.h"


namespace IFAIO
{
	struct Vertex3D
	{

		float x, y, z;

		static void setVertexLayout(VertexLayout& vl) noexcept
		{
			vl.Push<float>("Position", 3);
		}
	};

	
}