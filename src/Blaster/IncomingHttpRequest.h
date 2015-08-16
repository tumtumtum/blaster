#pragma once
#include "HttpParser.h"
#include "HttpRequest.h"

namespace Blaster
{
	namespace Http
	{ 
		class IncomingHttpConnection;

		class IncomingHttpRequest
			: public HttpRequest, virtual public HttpParser
		{
		private:
			Url url;
			string userHostAddress;
			uv_buf_t buffer;
			static void httpStreamOnRead(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);
			static void httpStreamOnAlloc(uv_handle_t* client, size_t suggested_size, uv_buf_t* buf);

		protected:
			virtual void onMessageComplete();
			virtual void onHeadersAvailable();

		public:
			IncomingHttpRequest(IncomingHttpConnection* connection);
			virtual ~IncomingHttpRequest();

			virtual const Url& getUrl();
			virtual const string& getPath();
			virtual const string& getUserHostAddress();
		};
	}
}