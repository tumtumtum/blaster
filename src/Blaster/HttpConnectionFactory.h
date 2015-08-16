//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <string>
#include "Url.h"
#include <uv.h>

namespace Blaster
{
	namespace Http
	{
		using namespace std;

		class HttpConnection;

		class HttpConnectionFactory
		{
		private:
			uv_loop_t* loop;
			static HttpConnectionFactory* defaultFactory;
		public:
			HttpConnectionFactory(uv_loop_t* loop);
			virtual ~HttpConnectionFactory();

			static HttpConnectionFactory& getDefault();

			virtual HttpConnection* createConnection(const Url& url);
			virtual HttpConnection* createConnection(const string& url);
		};
	}
}