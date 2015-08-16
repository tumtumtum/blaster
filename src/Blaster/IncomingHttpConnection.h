//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "HttpServer.h"
#include "HttpConnection.h"
#include "IncomingHttpRequest.h"
#include "OutgoingHttpResponse.h"

namespace Blaster
{
	namespace Http
	{		
		class HttpServer;

		class IncomingHttpConnection
			: public HttpConnection
		{
			friend class IncomingHttpRequest;

		protected:
			HttpServer* server;

			void requestParsed()
			{
				this->response = this->createHttpResponse();
			}

			IncomingHttpConnection(HttpServer* server)
				: HttpConnection(server->getLoop())
			{
				this->server = server;
			}


			virtual ~IncomingHttpConnection()
			{
			}

			IncomingHttpRequest* createHttpRequest()
			{
				auto retval = doCreateHttpRequest();

				return retval;
			}

			OutgoingHttpResponse* createHttpResponse()
			{
				auto retval = doCreateHttpResponse();

				onResponseCreated(retval);

				return retval;
			}

			virtual IncomingHttpRequest* doCreateHttpRequest() = 0;
			virtual OutgoingHttpResponse* doCreateHttpResponse() = 0;

		public:
			HttpServer* getServer()
			{
				return this->server;
			}

			virtual void attach()
			{
				uv_tcp_init(loop, stream);

				stream->data = this;

				uv_accept(server->getStream(), (uv_stream_t*)stream);

				this->request = dynamic_cast<IncomingHttpRequest*>(this->createHttpRequest());
			}
		};

		template <class REQUEST, class RESPONSE>
		class TemplatedIncomingHttpConnection
			: public IncomingHttpConnection
		{
		protected:
			virtual IncomingHttpRequest* doCreateHttpRequest()
			{
				return new REQUEST(this);
			}

			virtual OutgoingHttpResponse* doCreateHttpResponse()
			{
				return new RESPONSE(this);
			}

		public:
			TemplatedIncomingHttpConnection(HttpServer* server)
				: IncomingHttpConnection(server)
			{
			}
		};
	}
}