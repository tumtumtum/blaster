//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <string>
#include <functional>
#include "Url.h"
#include "HttpRequest.h"

namespace Blaster
{ 
	namespace Http
	{ 
		using namespace std;

		class HttpConnection;

		class OutgoingHttpRequest
			: public HttpRequest
		{
		private:
			Url url;
			string method;

			static void socketAfterWrite(uv_write_t* req, int status);

		public:
			OutgoingHttpRequest(HttpConnection* connection);
			virtual ~OutgoingHttpRequest();

			const string& getPath();
			const string& getMethod();
			void setMethod(const string& method);
			const Url& getUrl();
			void setUrl(const Url& value);
			virtual void write(uv_tcp_t* socket, std::function<void()> callback);
		};
	}
}