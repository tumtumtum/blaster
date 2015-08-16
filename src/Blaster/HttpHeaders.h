//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include "KeyValueCollection.h"
#include <sstream>
#include "uv.h"
#include "http_parser.h"

namespace Blaster
{
	namespace Http
	{
		class HttpHeaders
			: public KeyValueCollection<string, string>
		{
		public:
			HttpHeaders();
			virtual ~HttpHeaders();

			int getContentLength() const;

			void writeTo(ostream& stream);
		};
	}
}