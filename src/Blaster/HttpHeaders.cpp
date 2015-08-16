//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "HttpHeaders.h"

namespace Blaster
{
	namespace Http
	{ 
		HttpHeaders::HttpHeaders()
		{
		}

		HttpHeaders::~HttpHeaders()
		{
		}

		int HttpHeaders::getContentLength() const
		{
			const string& value = this->getValue("Content-Length");

			if (value == string())
			{
				return -1;
			}

			return std::stoi(value);
		}

		void HttpHeaders::writeTo(ostream& stream)
		{
			for (auto keyValue : this->getUnderlyingMap())
			{
				stream << keyValue.first << ": " << keyValue.second << "\r\n";
			}
		}
	}
}