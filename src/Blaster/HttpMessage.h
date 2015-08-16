//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "HttpHeaders.h"

namespace Blaster
{
	namespace Http
	{
		class HttpConnection;
		class HttpConnection;

		class HttpMessage
		{
		protected:
			HttpHeaders headers;
			HttpConnection* connection;

		public:
			HttpMessage(HttpConnection* httpConnection);

			virtual ~HttpMessage();
			
			HttpHeaders* getHeaders();
			HttpConnection* getConnection();
		};
	}
}
