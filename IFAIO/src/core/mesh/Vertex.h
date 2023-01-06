#pragma once

#include "core/dataStructs/Utilities.h"




namespace IFAIO
{
	template<size_t Attrib>
	struct Vertex
	{
		using Attribute_type = std::tuple<std::string, uint32_t>;
		constexpr static size_t Attribute_size = Attrib;

		virtual std::array<Attribute_type, Attrib> getAttributes() const noexcept =0;

	};
}