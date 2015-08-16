//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include <string.h>
#include <string>

template <typename T>
struct CaseInsensitiveComparer
{
	bool operator() (const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}
};

#ifdef _MSC_VER /* MSVC */
#define stricmp _stricmp
#else
#define stricmp strcasecmp
#endif

template<>
struct CaseInsensitiveComparer<std::string>
{
	bool operator() (const std::string& lhs, const std::string& rhs) const
	{
		return stricmp(lhs.c_str(), rhs.c_str()) < 0;
	}
};