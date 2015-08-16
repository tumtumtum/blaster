//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "OutgoingHttpRequest.h"

namespace Blaster
{ 
	namespace Http
	{
		struct WriteInfo
		{
			uv_buf_t buf;
			function<void()> callback;
		};

		OutgoingHttpRequest::OutgoingHttpRequest(HttpConnection* connection)
			: HttpRequest(connection), method("GET"), HttpMessage(connection)
		{
		}

		OutgoingHttpRequest::~OutgoingHttpRequest()
		{
		}

		void OutgoingHttpRequest::socketAfterWrite(uv_write_t* req, int status)
		{
			WriteInfo* writeInfo = *(WriteInfo**)(req + 1);

			writeInfo->callback();

			delete writeInfo;

			free(req);
		}

		void OutgoingHttpRequest::write(uv_tcp_t* socket, function<void()> callback)
		{
			stringstream requeststring; 
			auto query = url.getQuery().empty() ? "" : "?" + url.getQuery();

			requeststring << this->method << " " << url.getPath() << query << " HTTP/1.1" << "\r\n";
			requeststring << "Host: " + this->getUrl().getHost() << "\r\n";
			requeststring << "Accept: */*" << "\r\n";
			requeststring << "User-Agent: Blaster Server" << "\r\n";

			this->getHeaders()->writeTo(requeststring);

			requeststring << "\r\n";

			uv_write_t* req;
			WriteInfo* writeInfo;
			string s = requeststring.str();

			req = (uv_write_t*)malloc(sizeof(uv_write_t) + sizeof(WriteInfo*) + s.length() + 1);

			*(WriteInfo**)(req + 1) = writeInfo = new WriteInfo();

			writeInfo->callback = callback;
			writeInfo->buf.base = ((char*)(req)) + sizeof(uv_write_t) + sizeof(WriteInfo*);
			writeInfo->buf.base[s.length()] = 0;
			writeInfo->buf.len = (int)s.length();
			memcpy(writeInfo->buf.base, s.c_str(), s.length());
			
			uv_write(req, (uv_stream_t*)socket, &writeInfo->buf, 1, socketAfterWrite);
		}

		const string& OutgoingHttpRequest::getMethod()
		{
			return method;
		}

		void OutgoingHttpRequest::setMethod(const string& value)
		{
			method = value;
		}

		void OutgoingHttpRequest::setUrl(const Url& value)
		{
			this->url = value;
		}

		const Url& OutgoingHttpRequest::getUrl()
		{
			return this->url;
		}

		const string& OutgoingHttpRequest::getPath()
		{
			return this->url.getPath();
		}
	}
}