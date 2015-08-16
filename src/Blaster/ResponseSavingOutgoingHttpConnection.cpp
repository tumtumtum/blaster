//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "ResponseSavingOutgoingHttpConnection.h"

namespace Blaster
{
	namespace Http
	{
		ResponseSavingOutgoingHttpConnection::ResponseSavingOutgoingHttpConnection(uv_loop_t* loop, IBytesConsumer* savingBytesConsumer)
			: OutgoingHttpConnection(loop), savingBytesConsumer(savingBytesConsumer)
		{
		}

		ResponseSavingOutgoingHttpConnection::~ResponseSavingOutgoingHttpConnection()
		{
		}

		IncomingHttpResponse* ResponseSavingOutgoingHttpConnection::doCreateHttpResponse()
		{
			return new ProducingIncomingHttpResponse(this, savingBytesConsumer);
		}

		OutgoingHttpRequest* ResponseSavingOutgoingHttpConnection::doCreateHttpRequest()
		{
			return new OutgoingHttpRequest(this);
		}
	}
}