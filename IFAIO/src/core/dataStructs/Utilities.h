#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>

#include <cmath>
#include <iostream>
#include <fstream>

#include <wrl.h>
#include <d3d11.h>


#include "core/math/Vec2.h"
#include "core/math/Rect.h"

namespace IFAIO
{
	namespace wrl = Microsoft::WRL;

	typedef  double Decimal;

	using Int = int;
	using Int8 = int8_t;
	using Int16 = int16_t;
	using Int32 = int32_t;
	using Int64 = int64_t;

	using Uint = unsigned int;
	using Uint8 = uint8_t;
	using Uint16 = uint16_t;
	using Uint32 = uint32_t;
	using Uint64 = uint64_t;

	template <typename K, typename T>
	using Dict = std::unordered_map<K, T>;


	template<typename T>
	using  Ref = std::shared_ptr<T>;

	// for Windows API

#ifdef UNICODE
	typedef std::wstring Str;
	typedef wchar_t Char;

#else
	typedef std::string Str;
	typedef wchar_t Char;
#define STR(x) x
#endif



	class Utilities
	{

		std::wstring toWString(const char* str) noexcept
		{
			const size_t n = strlen(str) + 1;
			std::wstring res;
			res.resize(n);

			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, (wchar_t*)res.c_str(), n, str, _TRUNCATE);

			return res;
		}

	};
}