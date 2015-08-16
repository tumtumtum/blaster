//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include <iostream>
#include <assert.h>
#include "HttpServer.h"
#include "IncomingHttpConnection.h"

namespace Blaster
{
	namespace Http
	{
		using namespace std;

		HttpServer::HttpServer(int port, uv_loop_t* loop)
			: HttpServer(ServerConfig(port), loop)
		{	
		}

		HttpServer::HttpServer(ServerConfig config, uv_loop_t* loop)
			: config(config), loop((loop == NULL ? uv_default_loop() : loop)), port(config.getPort()), connectionCount(0)
		{
			tcpServer = (uv_tcp_t*)malloc(sizeof(*tcpServer));
			memset(tcpServer, 0, sizeof(*tcpServer));
		}

		HttpServer::~HttpServer()
		{
			free(tcpServer);
		}

		void HttpServer::start()
		{
			uv_tcp_init(loop, tcpServer);

			tcpServer->data = this;

			uv_ip4_addr("0.0.0.0", port, &listen_address);
			uv_tcp_bind(tcpServer, (const struct sockaddr*)&listen_address, 0);

			if (uv_listen((uv_stream_t*)tcpServer, 128, httpStreamOnConnect) != 0)
			{
				cerr << "Error binding to port " << port << "\n";
			}
			else
			{
				cout << "HttpServer started on port " << port << "\n";
			}
		}

		uv_loop_t* HttpServer::getLoop()
		{
			return this->loop;
		}

		void HttpServer::onConnectionClosed(HttpConnection* connection)
		{
			connection->setDelegate(0);

			cout << "Disconnected: " << ((IncomingHttpRequest*)connection->getRequest())->getUserHostAddress() << " " << connection->getRequest()->getHeaders()->getValue("User-Agent") << "\n";
			connectionCount--;
			cout << "Total connections: " << connectionCount << "\n";

			delete connection;
		}

		void HttpServer::onConnectionRequestCreated(HttpConnection* connection, HttpRequest* request)
		{
			cout << "Connected: " << " GET " << ((IncomingHttpRequest*)connection->getRequest())->getUrl().getPath() << " " << ((IncomingHttpRequest*)connection->getRequest())->getUserHostAddress() << " " << connection->getRequest()->getHeaders()->getValue("User-Agent") << "\n";
			cout << "Total connections: " << connectionCount << "\n";
		}

		void HttpServer::onConnectionResponseCreated(HttpConnection* connection, HttpResponse* response)
		{	
		}

		void HttpServer::httpStreamOnConnect(uv_stream_t* stream, int status)
		{
			assert(static_cast<HttpServer*>(stream->data)->getStream() == stream);

			auto _this = (HttpServer*)stream->data;
			auto connection = static_cast<HttpServer*>(stream->data)->createConnection();

			connection->setDelegate(_this);
			connection->attach();

			_this->connectionCount++;
		}
	}
}
