//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <string>

namespace Blaster
{
	using namespace std;

	class Url
	{
	private:
		size_t hashValue;
		size_t pathHashValue;;
		string scheme;
		string host;
		int port;
		string path;
		string query;
		string fragment;
		string userinfo;
		string s;

	public:
		Url();
		Url(const char* url);
		Url(const char* url, size_t len);
		Url(const string& url);
		~Url();

		bool operator==(const Url& url) const;
		bool operator!=(const Url& url) const;
		bool operator<(const Url& other) const;
		bool operator>(const Url& other) const;
		operator const string&() const;
		operator string() const;

		size_t getHash() const;
		size_t getPathHash() const;
		int getPort() const;
		const string& getScheme() const;
		const string& getHost() const;
		const string& getPath() const;
		const string& getQuery() const;
		const string& getFragment() const;
		const string& getUserInfo() const;
		const string& toString() const;

		struct equal
		{
			size_t operator()(const Url& a, const Url& b) const
			{
				return a.hashValue == b.hashValue && a == b;
			}
		};

		struct pathEqual
		{
			size_t operator()(const Url& a, const Url& b) const
			{
				return a.pathHashValue == b.pathHashValue && a.path == b.path;
			}
		};

		struct hash
		{
			size_t operator()(const Url &o) const
			{
				return o.getHash();
			}
		};

		struct pathHash
		{
			size_t operator()(const Url &o) const
			{
				return o.getPathHash();
			}
		};
	};
}