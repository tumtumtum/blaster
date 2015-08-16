//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <string>
#include <initializer_list>

namespace Blaster
{
	using namespace std;

	class UrlPathUtils
	{
	public:
		static string combine(initializer_list<string> parts);
		static string combine(const string& p1, const string& p2);
		static string combine(const string& p1, const string& p2, const string& p3);
		static string combine(const string& p1, const string& p2, const string& p3, const string& p4);
		static string combine(const string& p1, const string& p2, const string& p3, const string& p4, const string& p5);
	};
}