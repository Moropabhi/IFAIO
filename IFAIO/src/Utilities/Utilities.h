#pragma once

#include <sstream>
#include <string>

#ifdef UNICODE
typedef std::wstring Str;
typedef wchar_t Char;
typedef LPWSTR LPString;

#else
typedef std::string Str;
typedef char_t Char;
typedef LPSTR LPString;
#define STR(x) x
#endif