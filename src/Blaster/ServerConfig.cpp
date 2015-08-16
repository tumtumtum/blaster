//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include <sstream>
#include <iostream>
#include "ServerConfig.h"
#include <boost/algorithm/string.hpp>

namespace Blaster
{
	ServerConfig* ServerConfig::instance;

	ServerConfig::ServerConfig()
		: ServerConfig(80)
	{
	}

	ServerConfig::ServerConfig(int port)
		: downstreamUserBuffer(0), replaceAds(true), commercialZones(3), port(80)
	{
		this->setStaticStreamsPath("./static");
	}

	ServerConfig::~ServerConfig()
	{
	}

	ServerConfig* ServerConfig::getInstance()
	{
		if (instance == 0)
		{
			instance = new ServerConfig(80);
		}

		return instance;
	}

	bool ServerConfig::getReplaceAds()
	{
		return replaceAds;
	}

	void ServerConfig::setReplaceAds(bool value)
	{
		replaceAds = value;
	}

	int ServerConfig::getPort()
	{
		return this->port;
	}

	void ServerConfig::setPort(int value)
	{
		this->port = value;
	}

	string ServerConfig::getUpstreamUrlPrefix()
	{
		return this->upstreamUrlPrefix;
	}

	void ServerConfig::setUpstreamUrlPrefix(const string& value)
	{
		this->upstreamUrlPrefix = value;
	}

	string ServerConfig::getStaticStreamsPath()
	{
		return this->staticStreamsPath;
	}

	void ServerConfig::setStaticStreamsPath(const string& value)
	{
		this->staticStreamsPath = value;
	}

	int ServerConfig::getDownstreamUserBuffer()
	{
		return this->downstreamUserBuffer;
	}

	void ServerConfig::setDownstreamUserBuffer(int value)
	{
		this->downstreamUserBuffer = value;
	}

	uint64_t ServerConfig::getUpstreamBuffer()
	{
		return this->upstreamBuffer;
	}

	void ServerConfig::setUpstreamBuffer(uint64_t value)
	{
		this->upstreamBuffer = value;
	}

	const vector<string> ServerConfig::getUpstreamMountpoints()
	{
		return mountPoints;
	}

	void ServerConfig::setUpstreamMountpoints(const string& value)
	{
		string s;
		vector<string> strings;
		istringstream stream(value);

		while (getline(stream, s, ','))
		{
			strings.push_back(s);
		}

		mountPoints = strings;
	}

	void ServerConfig::setCommercialZones(const string& value)
	{
		string s;
		vector<string> strings;
		istringstream stream(value);

		while (getline(stream, s, ','))
		{
			strings.push_back(s);
		}

		commercialZones = strings;
	}
	
	const vector<string> ServerConfig::getCommercialZones()
	{
		return commercialZones;
	}

	void ServerConfig::setUpstreamMountpoints(const vector<string>& value)
	{
		mountPoints = value;
	}
}