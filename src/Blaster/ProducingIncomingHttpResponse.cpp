#include "ProducingIncomingHttpResponse.h"
#include "IBytesConsumer.h"

namespace Blaster
{
	namespace Http
	{
		ProducingIncomingHttpResponse::ProducingIncomingHttpResponse(HttpConnection* connection, IBytesConsumer* bytesConsumer)
			: bytesConsumer(bytesConsumer), IncomingHttpResponse(connection), HttpParser(connection, HTTP_RESPONSE), HttpMessage(connection)
		{
		}

		ProducingIncomingHttpResponse::~ProducingIncomingHttpResponse()
		{
		}

		void ProducingIncomingHttpResponse::onDataAvailable(const char* data, size_t length)
		{
			bytesConsumer->consume(this, (const char*)data, length);
		}
	}
}