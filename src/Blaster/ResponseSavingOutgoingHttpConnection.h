//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "uv.h"
#include "OutgoingHttpRequest.h"
#include "OutgoingHttpConnection.h"
#include "SavingBytesConsumer.h"
#include "ProducingIncomingHttpResponse.h"

namespace Blaster
{
	namespace Http
	{
		class ResponseSavingOutgoingHttpConnection
			: public OutgoingHttpConnection
		{
		private:
			IBytesConsumer* savingBytesConsumer;

		protected:
			virtual OutgoingHttpRequest* doCreateHttpRequest();
			virtual IncomingHttpResponse* doCreateHttpResponse();

		public:
			ResponseSavingOutgoingHttpConnection(uv_loop_t* loop, IBytesConsumer* savingBytesConsumer);
			virtual ~ResponseSavingOutgoingHttpConnection();

		};
	}
}