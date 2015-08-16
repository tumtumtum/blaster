//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "HttpParser.h"
#include "HttpConnection.h"

namespace Blaster
{
	namespace Http
	{
		HttpParser::HttpParser(HttpConnection* connection)
			: HttpMessage(connection), parser(0), connection(connection)
		{
		}

		HttpParser::HttpParser(HttpConnection* connection, http_parser_type parserType)
			: HttpMessage(connection), parser(0), connection(connection), headerParseState(HEADER_PARSE_STATE_NONE)
		{
			settings.on_url = HttpParser::parserUrl;
			settings.on_header_field = HttpParser::parserHeaderField;
			settings.on_header_value = HttpParser::parserHeaderValue;
			settings.on_headers_complete = HttpParser::parserHeadersComplete;
			settings.on_message_begin = HttpParser::parserMessageBegin;
			settings.on_message_complete = HttpParser::parserMessageComplete;
			settings.on_body = HttpParser::parserData;
			settings.on_status = HttpParser::parserStatus;
			
			parser = (http_parser*)malloc(sizeof(http_parser));

			parser->data = this;

			http_parser_init(parser, parserType);
		}

		HttpParser::~HttpParser()
		{
			free(parser);
		}

		int HttpParser::parserData(http_parser* parser, const char* data, size_t length)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			_this->onDataAvailable(data, length);

			return 0;
		}

		int HttpParser::parserStatus(http_parser* parser, const char* data, size_t length)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			_this->onParserStatus(data, length);

			return 0;
		}

		int HttpParser::parserUrl(http_parser* parser, const char* data, size_t length)
		{
			HttpParser* _this = (HttpParser*)parser->data; 
			
			_this->path = string(data, length);
			_this->method = http_method_str((http_method)parser->method);;

			_this->onUrlAvailable();
		
			return 0;
		}

		int HttpParser::parserMessageBegin(http_parser* parser)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			_this->onMessageBegin();

			return 0;
		}

		int HttpParser::parserMessageComplete(http_parser* parser)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			_this->onMessageComplete();

			return 0;
		}

		int HttpParser::parserHeadersComplete(http_parser* parser)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			_this->headers.setValue(_this->fieldBuffer.str(), _this->valueBuffer.str());

			_this->onHeadersAvailable();

			return 0;
		}

		int HttpParser::parserHeaderField(http_parser* parser, const char* data, size_t length)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			switch (_this->headerParseState)
			{
			case HEADER_PARSE_STATE_NONE:
			case HEADER_PARSE_STATE_FIELD:
				_this->fieldBuffer.sputn(data, length);
				break;
			case HEADER_PARSE_STATE_VALUE:
				_this->headers.setValue(_this->fieldBuffer.str(), _this->valueBuffer.str());
				_this->valueBuffer.str("");
				_this->fieldBuffer.str("");
				_this->fieldBuffer.sputn(data, length);
				break;
			}

			_this->headerParseState = HEADER_PARSE_STATE_FIELD;

			return 0;
		}

		int HttpParser::parserHeaderValue(http_parser* parser, const char* data, size_t length)
		{
			HttpParser* _this = (HttpParser*)parser->data;

			switch (_this->headerParseState)
			{
			case HEADER_PARSE_STATE_NONE:
				break;
			case HEADER_PARSE_STATE_FIELD:
				_this->valueBuffer.str("");
			case HEADER_PARSE_STATE_VALUE:
				_this->valueBuffer.sputn(data, length);
				break;
			}

			_this->headerParseState = HEADER_PARSE_STATE_VALUE;

			return 0;
		}

		void HttpParser::onDataAvailable(const char* data, size_t length)
		{
			if (this->delegate != 0)
			{
				this->delegate->onDataAvailable(this, data, length);
			}
		}

		void HttpParser::onParserStatus(const char* data, size_t length)
		{
			if (this->delegate != 0)
			{
				this->delegate->onParserStatus(this, data, length);
			}
		}

		void HttpParser::onMessageBegin()
		{
			if (this->delegate != 0)
			{
				this->delegate->onMessageBegin(this);
			}
		}

		void HttpParser::onMessageComplete()
		{
			if (this->delegate != 0)
			{
				this->delegate->onMessageComplete(this);
			}
		}

		void HttpParser::onUrlAvailable()
		{
			if (this->delegate != 0)
			{
				this->delegate->onUrlAvailable(this);
			}
		}

		void HttpParser::onHeadersAvailable()
		{
			if (this->delegate != 0)
			{
				this->delegate->onHeadersAvailable(this);
			}
		}

		const string& HttpParser::getPath()
		{
			return this->path;
		}

		const string& HttpParser::getMethod()
		{
			return this->method;
		}
	}
}