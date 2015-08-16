//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <functional>
#include "IncomingHttpResponse.h"
#include "IBytesConsumer.h"

namespace Blaster
{
	namespace Http
	{	
		using namespace std;

		class HttpConnection;
		
		class ProducingIncomingHttpResponse
			: public IncomingHttpResponse
		{
		private:
			IBytesConsumer* bytesConsumer;
			
		protected:
			void onDataAvailable(const char* data, size_t length);
			
		public:
			ProducingIncomingHttpResponse(HttpConnection* connection, IBytesConsumer* bytesConsumer);
			virtual ~ProducingIncomingHttpResponse();
		};
	}
}