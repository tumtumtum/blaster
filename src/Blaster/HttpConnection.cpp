//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include <string.h>
#include "HttpConnection.h"
#include "IncomingHttpRequest.h"
#include "HttpServer.h"
#include "HttpRequest.h"

namespace Blaster
{
	namespace Http
	{
		HttpConnection::HttpConnection(uv_loop_t* loop)
			: loop(loop), disposed(false), request(0), response(0)
		{
			stream = (uv_tcp_t*)calloc(1, sizeof(*stream));
			memset(stream, 0, sizeof(*stream));
		}

		void HttpConnection::httpStreamOnCloseFromDestructor(uv_handle_t* handle)
		{
			free(handle);
		}

		HttpConnection::~HttpConnection()
		{
			delete this->request;
			delete this->response;

			this->request = NULL;
			this->response = NULL;

			if (!disposed)
			{
				disposed = true;

				uv_close((uv_handle_t*)this->getStream(), httpStreamOnCloseFromDestructor);
			}
			else
			{
				free(stream);
			}
		}

		uv_tcp_t* HttpConnection::getTcp()
		{
			return this->stream;
		}

		uv_stream_t* HttpConnection::getStream()
		{
			return (uv_stream_t*)this->stream;
		}

		void HttpConnection::close()
		{
			if (!disposed)
			{
				disposed = true;

				uv_close((uv_handle_t*)this->getStream(), httpStreamOnClose);
			}
		}

		void HttpConnection::httpStreamOnClose(uv_handle_t* handle)
		{
			auto _this = (HttpConnection*)handle->data;
			
			_this->onConnectionClosed();
		}

		HttpRequest* HttpConnection::getRequest()
		{
			return this->request;
		}

		HttpResponse* HttpConnection::getResponse()
		{
			return this->response;
		}

		void HttpConnection::onConnectionClosed()
		{
			if (this->delegate)
			{
				this->delegate->onConnectionClosed(this);
			}
		}

		void HttpConnection::onRequestCreated(HttpRequest* request)
		{
			if (this->delegate)
			{
				this->delegate->onConnectionRequestCreated(this, request);
			}
		}

		void HttpConnection::onResponseCreated(HttpResponse* response)
		{
			if (this->delegate)
			{
				this->delegate->onConnectionResponseCreated(this, response);
			}
		}

		uv_loop_t* HttpConnection::getLoop()
		{
			return this->loop;
		}

		HttpServer* HttpConnection::getServer()
		{
			return this->server;
		}
	}
}
