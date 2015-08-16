//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include "uv.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "IncomingHttpRequest.h"
#include "ISupportsNotifications.h"

namespace Blaster
{
	namespace Http
	{
		class HttpServer;

		class HttpConnectionDelegate
		{
		public:
			virtual void onConnectionClosed(HttpConnection* connection) = 0;
			virtual void onConnectionRequestCreated(HttpConnection* connection, HttpRequest* request) = 0;
			virtual void onConnectionResponseCreated(HttpConnection* connection, HttpResponse* response) = 0;
		};

		class HttpConnection
			: public ISupportsNotifications<HttpConnectionDelegate>
		{
			friend class IncomingHttpRequest;

		protected:
			bool disposed;
			uv_loop_t* loop;
			uv_tcp_t* stream;
			HttpServer* server; 
			HttpRequest* request;
			HttpResponse* response;
			
			virtual void onConnectionClosed();
			virtual void onRequestCreated(HttpRequest* request);
			virtual void onResponseCreated(HttpResponse* response);

			static void httpStreamOnCloseFromDestructor(uv_handle_t* handle);
			static void httpStreamOnClose(uv_handle_t* handle);
			
		public:
			HttpConnection(uv_loop_t* loop);
			virtual ~HttpConnection();

			uv_loop_t* getLoop();

			uv_tcp_t* getTcp();
			uv_stream_t* getStream();

			void close();

			virtual HttpServer* getServer();
			virtual HttpRequest* getRequest();
			virtual HttpResponse* getResponse();
		};
	}
}