//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include <sstream>
#include "Url.h"
#include "HttpParser.h"
#include "HttpHeaders.h"
#include "HttpMessage.h"

namespace Blaster
{
	namespace Http
	{
		using namespace std;
		
		class HttpConnection;

		class HttpRequest : public virtual HttpMessage
		{
		public:
			HttpRequest(HttpConnection* connection);
			virtual ~HttpRequest();

			virtual const string& getPath() = 0;
			virtual const Url& getUrl() = 0;
		};
	}
}