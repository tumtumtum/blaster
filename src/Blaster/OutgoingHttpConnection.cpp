//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include <string.h>
#include <iostream>
#include "OutgoingHttpConnection.h"

namespace Blaster
{
	namespace Http
	{
		using namespace std;

		OutgoingHttpConnection::OutgoingHttpConnection(uv_loop_t* loop)
			: HttpConnection(loop)
		{
			memset(&conn, 0, sizeof(conn));
			memset(&resolver, 0, sizeof(resolver));

			this->conn.data = this;
			this->stream->data = this;
		}

		OutgoingHttpConnection::~OutgoingHttpConnection()
		{
			uv_cancel((uv_req_t*)&resolver);
			uv_cancel((uv_req_t*)&conn);

			resolver.data = 0;
			conn.data = 0;
		}

		void OutgoingHttpConnection::connect()
		{
			struct addrinfo hints;

			hints.ai_family = PF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = 0;

			resolver.data = this;

			auto result = uv_getaddrinfo(loop, &resolver, OutgoingHttpConnection::httpOnResolved, request->getUrl().getHost().c_str(), std::to_string(request->getUrl().getPort()).c_str(), &hints);
		}

		void OutgoingHttpConnection::httpStreamAfterWrite(uv_write_t* req, int status)
		{
		}

		void OutgoingHttpConnection::httpStreamOnConnect(uv_connect_t* connect, int status)
		{
			auto _this = (OutgoingHttpConnection*)connect->data;

			if (_this == 0)
			{
				return;
			}

			if (status == 0)
			{
				_this->onStreamConnect();
			}
			else
			{
				_this->onConnectionClosed();
			}
		}

		void OutgoingHttpConnection::httpOnResolved(uv_getaddrinfo_t* resolver, int status, struct addrinfo* res)
		{
			auto _this = (OutgoingHttpConnection*)resolver->data;

			if (_this == 0)
			{
				return;
			}

			if (status == 0)
			{
				uv_tcp_init(_this->loop, _this->stream);

				uv_tcp_connect(&_this->conn, (uv_tcp_t*)_this->stream, (const struct sockaddr*)res->ai_addr, httpStreamOnConnect);

				uv_freeaddrinfo(res);
			}
			else
			{
				_this->onConnectionClosed();

				uv_freeaddrinfo(res);
			}
		}

		void OutgoingHttpConnection::onStreamConnect()
		{
			((OutgoingHttpRequest*)this->request)->write(this->getTcp(), [this]() -> void
			{
				this->response = this->createHttpResponse();
			});
		}

		HttpRequest* OutgoingHttpConnection::getRequest()
		{
			if (this->request == 0)
			{
				this->request = this->createHttpRequest();
			}

			return this->request;
		}

		OutgoingHttpRequest* OutgoingHttpConnection::createHttpRequest()
		{
			auto retval = this->doCreateHttpRequest();

			this->onRequestCreated(retval);

			return retval;
		}

		IncomingHttpResponse* OutgoingHttpConnection::createHttpResponse()
		{
			auto retval = this->doCreateHttpResponse();

			this->onResponseCreated(retval);

			return retval;
		}
	}
}