//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include "uv.h"
#include <vector>
#include "ServerConfig.h"
#include "HttpConnection.h"
#include <boost/shared_ptr.hpp>

namespace Blaster
{
	namespace Http
	{
		using namespace std;
		class HttpConnection;
		class IncomingHttpConnection;
		
		class HttpServer : protected HttpConnectionDelegate
		{
		private:
			int port;
			uv_loop_t* loop;
			uv_tcp_t* tcpServer;
			ServerConfig config;
			uint64_t connectionCount;
			struct sockaddr_in listen_address;
			vector<boost::shared_ptr<HttpConnection>> connections;
		
			static void httpStreamOnConnect(uv_stream_t* stream, int status);

		protected:
			virtual IncomingHttpConnection* createConnection() = 0;

			virtual void onConnectionClosed(HttpConnection* connection);
			virtual void onConnectionRequestCreated(HttpConnection* connection, HttpRequest* request);
			virtual void onConnectionResponseCreated(HttpConnection* connection, HttpResponse* response);

		public:
			HttpServer(int port = 80, uv_loop_t* loop = NULL);
			HttpServer(ServerConfig config, uv_loop_t* loop = NULL);

			virtual ~HttpServer();

			inline uv_tcp_t* getTcp()
			{
				return this->tcpServer;
			}

			inline uv_stream_t* getStream()
			{
				return (uv_stream_t*)this->tcpServer;
			}

			ServerConfig* getConfig()
			{
				return &this->config;
			}

			int getPort();
			void setPort(int value);
			uv_loop_t* getLoop();

			void start();
			void stop();
		};

		template <class CONNECTION>
		class TemplatedHttpServer
			: public HttpServer
		{
		protected:
			virtual IncomingHttpConnection* createConnection()
			{
				return new CONNECTION(this);
			}

		public:
			TemplatedHttpServer(int port)
				: HttpServer(port)
			{
			}

			TemplatedHttpServer(ServerConfig config)
				: HttpServer(config)
			{
			}
		};
	}
}
