//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "Url.h"
#include <string.h>
#include "http_parser.h"

namespace Blaster
{ 
	Url::Url()
		: hashValue(0)
	{
	}

	Url::Url(const string& url)
		: Url(url.c_str(), url.length())
	{
	}

	Url::Url(const char* url)
		: Url(url, strlen(url))
	{
	}

	Url::Url(const char* url, size_t len)
		: hashValue(0)
	{
		http_parser_url result;

		memset(&result, 0, sizeof(result));

		http_parser_parse_url(url, strlen(url), 0, &result);

		if (result.field_set &  (1 << UF_SCHEMA))
		{ 
			scheme = string(url + result.field_data[UF_SCHEMA].off, result.field_data[UF_SCHEMA].len);
		}

		if (result.field_set & (1 << UF_HOST))
		{ 
			host = string(url + result.field_data[UF_HOST].off, result.field_data[UF_HOST].len);
		}

		if (result.field_set & (1 << UF_PATH))
		{
			path = string(url + result.field_data[UF_PATH].off, result.field_data[UF_PATH].len);
		}

		if (result.field_set & (1 << UF_QUERY))
		{
			query = string(url + result.field_data[UF_QUERY].off, result.field_data[UF_QUERY].len);
		}

		if (result.field_set & (1 << UF_FRAGMENT))
		{ 
			fragment = string(url + result.field_data[UF_FRAGMENT].off, result.field_data[UF_FRAGMENT].len);
		}

		if (result.field_set & (1 << UF_USERINFO))
		{ 
			userinfo = string(url + result.field_data[UF_USERINFO].off, result.field_data[UF_USERINFO].len);
		}

		if (scheme == "http")
		{
			port = result.port == 0 ? 80 : result.port;
		}
		else if (scheme == "https")
		{
			port = result.port == 0 ? 443 : result.port;
		}
		else if (scheme == "ftp")
		{
			port = result.port == 0 ? 21 : result.port;
		}

		this->s = scheme + "://" + userinfo + host + (result.port == 0 ? "" : ":" + to_string(result.port)) + path + (query.length() > 0 ? "?" + query : "") + fragment;

		hashValue = std::hash<string>()(s);
		pathHashValue = std::hash<string>()(this->getPath());
	}

	Url::~Url()
	{
	}

	const string& Url::getScheme() const
	{
		return this->scheme;
	}

	int Url::getPort() const
	{
		return this->port;
	}

	const string& Url::getHost() const
	{
		return this->host;
	}

	const string& Url::getPath() const
	{
		return this->path;
	}

	const string& Url::getQuery() const
	{
		return this->query;
	}

	const string& Url::getFragment() const
	{
		return this->fragment;
	}

	const string& Url::getUserInfo() const
	{
		return this->userinfo;
	}

	bool Url::operator==(const Url& url) const
	{
		if (this->hashValue != url.hashValue)
		{
			return false;
		}

		return this->s == url.s;
	}

	bool Url::operator!=(const Url& url) const
	{
		if (this->hashValue != url.hashValue)
		{
			return true;
		}

		return this->s != url.s;
	}

	bool Url::operator<(const Url& other) const
	{
		return this->s < other.s;
	}

	bool Url::operator>(const Url& other) const
	{
		return this->s > other.s;
	}

	Url::operator const string&() const
	{
		return s;
	}

	Url::operator string() const
	{
		return s;
	}

	const string& Url::toString() const
	{
		return s;
	}

	size_t Url::getHash() const
	{
		return hashValue;
	}

	size_t Url::getPathHash() const
	{
		return pathHashValue;
	}
}