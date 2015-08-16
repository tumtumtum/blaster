//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "UrlPathUtils.h"

namespace Blaster
{
	string UrlPathUtils::combine(initializer_list<string> parts)
	{
		int i = 0;
		string retval;

		for (auto part : parts)
		{
			if (part.length() == 0)
			{
				continue;
			}

			auto start = part.begin();

			if (part != *parts.begin())
			{
				while (*start == '/')
				{
					start++;
				}
			}

			auto end = part.end() - 1;

			while (*end == '/')
			{
				end--;
			}

			retval.append(start, end + 1);
			
			if (part != *(parts.end() - 1))
			{
				retval.append("/");
			}

			i++;
		}

		return retval;
	}

	string UrlPathUtils::combine(const string& p1, const string& p2)
	{
		return combine({ p1, p2});
	}

	string UrlPathUtils::combine(const string& p1, const string& p2, const string& p3)
	{
		return combine({ p1, p2, p3 });
	}

	string UrlPathUtils::combine(const string& p1, const string& p2, const string& p3, const string& p4)
	{
		return combine({ p1, p2, p3, p4 });
	}

	string UrlPathUtils::combine(const string& p1, const string& p2, const string& p3, const string& p4, const string& p5)
	{
		return combine({ p1, p2, p3, p4, p5 });
	}
}