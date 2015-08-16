//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <string>
#include <stdint.h>
#include <vector>
#include "ServerConfig.h"

namespace Blaster
{
	using namespace std;

	class ServerConfig
	{
	private:
		static ServerConfig* instance;

		int port;
		bool replaceAds;
		vector<string> commercialZones;
		vector<string> mountPoints;
		string upstreamUrlPrefix;
		string staticStreamsPath;
		int downstreamUserBuffer;
		int64_t upstreamBuffer;
		ServerConfig* serverConfig;
		

	public:
		ServerConfig();
		ServerConfig(int port);
		virtual ~ServerConfig();

		static ServerConfig* getInstance();

		const vector<string> getCommercialZones();
		void setCommercialZones(const string& value);

		int getPort();
		void setPort(int value);

		bool getReplaceAds();
		void setReplaceAds(bool value);

		const vector<string> getUpstreamMountpoints();
		void setUpstreamMountpoints(const string& value);
		void setUpstreamMountpoints(const vector<string>& value);

		string getUpstreamUrlPrefix();
		void setUpstreamUrlPrefix(const string& value);

		string getStaticStreamsPath();
		void setStaticStreamsPath(const string& value);

		int getDownstreamUserBuffer();
		void setDownstreamUserBuffer(int value);

		uint64_t getUpstreamBuffer();
		void setUpstreamBuffer(uint64_t value);
	};
}