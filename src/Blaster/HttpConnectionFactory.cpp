//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "HttpRequest.h"
#include "OutgoingHttpResponse.h"
#include "OutgoingHttpConnection.h"
#include "HttpConnectionFactory.h"
#include <uv.h>

namespace Blaster
{
	namespace Http
	{
		HttpConnectionFactory* HttpConnectionFactory::defaultFactory = NULL;

		HttpConnectionFactory::HttpConnectionFactory(uv_loop_t* loop)
			: loop(loop)
		{
		}

		HttpConnectionFactory::~HttpConnectionFactory()
		{
		}

		HttpConnectionFactory& HttpConnectionFactory::getDefault()
		{
			if (defaultFactory == NULL)
			{
				defaultFactory = new HttpConnectionFactory(uv_default_loop());
			}

			return *defaultFactory;
		}

		HttpConnection* HttpConnectionFactory::createConnection(const string& url)
		{
			return createConnection(Url(url));
		}

		HttpConnection* HttpConnectionFactory::createConnection(const Url& url)
		{
			auto connection = new TemplatedOutgoingHttpConnection<OutgoingHttpRequest, IncomingHttpResponse>(this->loop);

			connection->getRequest()->setUrl(url);

			return connection;
		}
	}
}