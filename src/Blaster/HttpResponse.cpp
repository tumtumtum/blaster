//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "HttpResponse.h"
#include "HttpConnection.h"

namespace Blaster
{
	namespace Http
	{
		HttpResponse::HttpResponse(HttpConnection* connection)
			: HttpMessage(connection), connection(connection)
		{
		}

		HttpResponse::~HttpResponse()
		{
		}

		int HttpResponse::getStatusCode()
		{
			return statusCode;
		}

		void HttpResponse::setStatusCode(int value)
		{
			statusCode = value;
		}
	}
}