//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include "uv.h"
#include "HttpHeaders.h"
#include "HttpMessage.h"

namespace Blaster
{
	namespace Http
	{
		class HttpConnection;

		class HttpResponse
			: public virtual HttpMessage
		{
		protected:
			int statusCode;
			HttpConnection* connection;

		public:
			HttpResponse(HttpConnection* connection);
			virtual ~HttpResponse();

			int getStatusCode();
			void setStatusCode(int value);
		};
	}
}