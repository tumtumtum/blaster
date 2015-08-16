//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include <string>
#include <sstream>
#include "uv.h"
#include "Url.h"
#include "HttpParser.h"
#include "HttpHeaders.h"
#include "HttpMessage.h"
#include "ISupportsNotifications.h"

namespace Blaster
{
	namespace Http
	{
		using namespace std;

		class HttpParser;
		class HttpConnection;

		class HttpParserDelegate
		{
		public:
			virtual void onParserStatus(HttpParser* parser, const char* data, size_t length) = 0;
			virtual void onDataAvailable(HttpParser* parser, const char* data, size_t length) = 0;
			virtual void onMessageBegin(HttpParser* parser) = 0;
			virtual void onMessageComplete(HttpParser* parser) = 0;
			virtual void onUrlAvailable(HttpParser* parser) = 0;
			virtual void onHeadersAvailable(HttpParser* parser) = 0;
		};

		class HttpParser 
			: public virtual HttpMessage, public ISupportsNotifications<HttpParserDelegate>
		{
			enum HEADER_PARSE_STATE
			{
				HEADER_PARSE_STATE_NONE,
				HEADER_PARSE_STATE_FIELD,
				HEADER_PARSE_STATE_VALUE
			};

		private:
			stringbuf fieldBuffer;
			stringbuf valueBuffer;
			HEADER_PARSE_STATE headerParseState;

			static int parserData(http_parser* parser, const char* data, size_t length);
			static int parserStatus(http_parser* parser, const char* data, size_t length);
			static int parserMessageBegin(http_parser* parser);
			static int parserMessageComplete(http_parser* parser);
			static int parserHeadersComplete(http_parser* parser);
			static int parserUrl(http_parser* parser, const char* data, size_t length);
			static int parserHeaderField(http_parser* parser, const char* data, size_t length);
			static int parserHeaderValue(http_parser* parser, const char* data, size_t length);

		protected:
			string path;
			string method;
			http_parser* parser; 
			uv_stream_t* stream;
			HttpConnection* connection;
			http_parser_settings settings;
			
			virtual void onDataAvailable(const char* data, size_t length);
			virtual void onParserStatus(const char* data, size_t length);
			virtual void onMessageBegin(); 
			virtual void onMessageComplete();
			virtual void onUrlAvailable();
			virtual void onHeadersAvailable();

		public:

			inline http_parser* getParser()
			{
				return this->parser;
			}

			inline http_parser_settings* getParserSettings()
			{
				return &this->settings;
			}

			const string& getPath();
			const string& getMethod();;
			HttpParser(HttpConnection* connection);
			HttpParser(HttpConnection* connection, http_parser_type parserType);
			virtual ~HttpParser();
		};
	}
}