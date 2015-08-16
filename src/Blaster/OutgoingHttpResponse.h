//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "HttpParser.h"
#include "HttpResponse.h"

namespace Blaster
{ 
	namespace Http
	{
		class OutgoingHttpResponse
			: public HttpResponse
		{
		private:
			static void streamHeaderWritten(uv_write_t* req, int status);

		protected:
			void writeResponseHeader();
			virtual void onWrittenHeader();

		public:
			OutgoingHttpResponse(HttpConnection* connection);
			virtual ~OutgoingHttpResponse();
		};
	}
}