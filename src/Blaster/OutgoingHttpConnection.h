//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "HttpConnection.h"
#include "OutgoingHttpRequest.h"
#include "IncomingHttpResponse.h"

namespace Blaster
{
	namespace Http
	{ 
		class OutgoingHttpConnection
			: public HttpConnection
		{
		private:
			uv_connect_t conn;
			uv_getaddrinfo_t resolver;

			static void httpStreamAfterWrite(uv_write_t* req, int status);
			static void httpStreamOnConnect(uv_connect_t* connect, int status);
			static void httpOnResolved(uv_getaddrinfo_t* resolver, int status, struct addrinfo* res);
			void onStreamConnect();
			void onStreamConnectFailed();

		protected:
			virtual OutgoingHttpRequest* createHttpRequest();
			virtual IncomingHttpResponse* createHttpResponse();
			virtual OutgoingHttpRequest* doCreateHttpRequest() = 0;
			virtual IncomingHttpResponse* doCreateHttpResponse() = 0;

		public:
			virtual void connect();
			OutgoingHttpConnection(uv_loop_t* loop);
			virtual ~OutgoingHttpConnection();

			virtual HttpRequest* getRequest();
		};

		template <class REQUEST, class RESPONSE>
		class TemplatedOutgoingHttpConnection
			: public OutgoingHttpConnection
		{
		protected:
			virtual OutgoingHttpRequest* doCreateHttpRequest()
			{
				return new REQUEST(this);
			}

			virtual IncomingHttpResponse* doCreateHttpResponse()
			{
				return new RESPONSE(this);
			}

		public:
			TemplatedOutgoingHttpConnection(uv_loop_t* loop)
				: OutgoingHttpConnection(loop)
			{
			}

			virtual REQUEST* getRequest()
			{
				return (REQUEST*)OutgoingHttpConnection::getRequest();
			}

			virtual RESPONSE* getResponse()
			{
				return (RESPONSE*)OutgoingHttpConnection::getResponse();
			}
		};
	}
}