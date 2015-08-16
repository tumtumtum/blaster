//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "HttpMessage.h"

namespace Blaster
{
	namespace Http
	{
		HttpMessage::HttpMessage(HttpConnection* connection)
			: connection(connection)
		{
		}

		HttpMessage::~HttpMessage()
		{
		}

		HttpHeaders* HttpMessage::getHeaders()
		{
			return &this->headers;
		}

		HttpConnection* HttpMessage::getConnection()
		{
			return this->connection;
		}
	}
}
