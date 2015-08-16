//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "HttpResponse.h"
#include "HttpParser.h"

namespace Blaster
{ 
	namespace Http
	{
		class IncomingHttpResponse
			: public HttpResponse, virtual public HttpParser
		{
		private:
			uv_buf_t buffer;

			static void httpStreamOnRead(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);
			static void httpStreamOnAlloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf);

		public:
			IncomingHttpResponse(HttpConnection* connection);
			virtual ~IncomingHttpResponse();
		};
	}
}