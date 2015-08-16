//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "IncomingHttpResponse.h"
#include "HttpConnection.h"

namespace Blaster
{
	namespace Http
	{
		IncomingHttpResponse::IncomingHttpResponse(HttpConnection* connection)
			: HttpResponse(connection), HttpMessage(connection), HttpParser(connection, HTTP_RESPONSE)
		{
			const size_t bufferSize = 1024 * 64;

			buffer.base = (char*)calloc(1, bufferSize);
			buffer.len = bufferSize;

			uv_read_start(this->HttpResponse::connection->getStream(), httpStreamOnAlloc, httpStreamOnRead);
		}

		IncomingHttpResponse::~IncomingHttpResponse()
		{
			uv_read_stop(this->HttpResponse::connection->getStream());

			free(buffer.base);
		}

		void IncomingHttpResponse::httpStreamOnAlloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf)
		{
			auto connection = ((HttpConnection*)client->data);
			auto response = static_cast<IncomingHttpResponse*>(connection->getResponse());

			*buf = response->buffer;
		}

		void IncomingHttpResponse::httpStreamOnRead(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf)
		{
			if (nread == 0)
			{
				return;
			}
			 
			size_t parsed;
			auto connection = ((HttpConnection*)tcp->data);
			auto response = static_cast<IncomingHttpResponse*>(connection->getResponse());

			if (nread > 0)
			{
				parsed = http_parser_execute(response->getParser(), response->getParserSettings(), buf->base, nread);

				if (parsed == 0)
				{
					parsed = 0;
				}
			}
			else
			{
				connection->close();
			}
		}
	}
}