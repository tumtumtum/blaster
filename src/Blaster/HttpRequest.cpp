//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "Url.h"
#include "HttpRequest.h"
#include "HttpConnection.h"
#include "uv.h"
#include "http_parser.h"

namespace Blaster
{
	namespace Http
	{
		HttpRequest::HttpRequest(HttpConnection* connection)
			: HttpMessage(connection)
		{
		}

		HttpRequest::~HttpRequest()
		{
		}
	}
}