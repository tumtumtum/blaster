//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "IncomingHttpRequest.h"
#include "IncomingHttpConnection.h"

namespace Blaster
{ 
	namespace Http
	{ 
		IncomingHttpRequest::IncomingHttpRequest(IncomingHttpConnection* connection)
			: HttpRequest(connection), HttpParser(connection, HTTP_REQUEST), HttpMessage(connection)
		{
			const size_t bufferSize = 1024 * 64;

			buffer.base = new char[bufferSize];
			buffer.len = bufferSize;

			 uv_read_start(connection->getStream(), httpStreamOnAlloc, httpStreamOnRead);
		}

		IncomingHttpRequest::~IncomingHttpRequest()
		{
			delete[] buffer.base;
		}

		void IncomingHttpRequest::httpStreamOnAlloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf)
		{
			auto connection = ((HttpConnection*)client->data);
			auto request = static_cast<IncomingHttpRequest*>(connection->getRequest());

			*buf = request->buffer;
		}

		void IncomingHttpRequest::httpStreamOnRead(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf)
		{
			size_t parsed;
			auto connection = ((HttpConnection*)tcp->data);
			auto request = static_cast<IncomingHttpRequest*>(connection->getRequest());

			if (nread > 0)
			{
				parsed = http_parser_execute(request->getParser(), request->getParserSettings(), buf->base, nread);
			}
			else
			{
				connection->close();
			}
		}

		void IncomingHttpRequest::onHeadersAvailable()
		{
			((IncomingHttpConnection*)connection)->onRequestCreated(this);
		}

		void IncomingHttpRequest::onMessageComplete()
		{
			HttpParser::onMessageComplete();

			((IncomingHttpConnection*)this->connection)->requestParsed();
		}

		const Url& IncomingHttpRequest::getUrl()
		{
			url = Url("http://" + this->getHeaders()->getValue("Host") + HttpParser::getPath());

			return url;
		}

		const string& IncomingHttpRequest::getPath()
		{
			return HttpParser::path;
		}

		const string& IncomingHttpRequest::getUserHostAddress()
		{
			if (this->userHostAddress.empty())
			{
				struct sockaddr addr;
				char buffer[3 * 4 + 3] = {};
				int len = sizeof(struct sockaddr);

				uv_tcp_getpeername(this->getConnection()->getTcp(), &addr, &len);

				uv_ip4_name((const sockaddr_in*)&addr, buffer, sizeof(buffer));

				this->userHostAddress = buffer;
			}

			return this->userHostAddress;
		}
	}
}