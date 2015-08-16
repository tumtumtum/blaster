//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "HttpConnection.h"
#include "OutgoingHttpResponse.h"

namespace Blaster
{
	namespace Http
	{
		OutgoingHttpResponse::OutgoingHttpResponse(HttpConnection* connection)
			: HttpResponse(connection), HttpMessage(connection)
		{
			this->getHeaders()->setValue("Content-Type", "audio/aacp");
		}
		
		OutgoingHttpResponse::~OutgoingHttpResponse()
		{
		}

		void OutgoingHttpResponse::streamHeaderWritten(uv_write_t* req, int status)
		{
			auto _this = (OutgoingHttpResponse*)req->data;

			free(req);

			_this->onWrittenHeader();
		}

		void OutgoingHttpResponse::onWrittenHeader()
		{
		}

		void OutgoingHttpResponse::writeResponseHeader()
		{
			stringstream requeststring;

			requeststring << "HTTP/1.0 200 OK" << "\r\n";
			requeststring << "Server: Blaster Server" << "\r\n";

			this->getHeaders()->writeTo(requeststring);

			requeststring << "\r\n";

			uv_buf_t* buf;
			uv_write_t* req;
			string s = requeststring.str();

			auto tempbuffer = (char *)malloc(sizeof(uv_write_t) + sizeof(uv_buf_t) + s.length());
			req = (uv_write_t*)tempbuffer;
			buf = (uv_buf_t*)(tempbuffer + sizeof(uv_write_t));
			buf->base = (tempbuffer + sizeof(uv_write_t) + sizeof(uv_buf_t));
			buf->len = (unsigned long)s.length();

			req->data = this;

			memcpy(buf->base, s.c_str(), s.length());

			uv_write(req, this->connection->getStream(), buf, 1, streamHeaderWritten);
		}
	}
}