//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include <stdio.h>
#include <uv.h>
#include <iostream>
#include <string>
#include <chrono>
#include "Url.h"
#include "HttpServer.h"
#include "ServerConfig.h"
#include "HttpConnection.h"
#include "SavingBytesConsumer.h"
#include "ProducingIncomingHttpResponse.h"
#include <boost/circular_buffer.hpp>
#include <boost/program_options.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

using namespace std;
using namespace boost;
using namespace boost::log;
using namespace Blaster;
using namespace Blaster::Http;

namespace cli = boost::program_options;

int main(int argc, const char** argv)
{
	cli::options_description desc("blaster options");

	desc.add_options()
		("help", "help message")
		("preserveads", "don't replace ads")
		("port", cli::value<int>(), "set listen port (default 80)");

	cli::variables_map vm;
	cli::store(cli::parse_command_line(argc, argv, desc), vm);
	cli::notify(vm);

	if (vm.count("static"))
	{
		ServerConfig::getInstance()->setStaticStreamsPath(vm["static"].as<string>());
	}

	if (vm.count("zones"))
	{
		ServerConfig::getInstance()->setCommercialZones(vm["zones"].as<string>());
	}

	if (vm.count("upstream"))
	{
		ServerConfig::getInstance()->setUpstreamUrlPrefix(vm["upstream"].as<string>());
	}

	if (vm.count("mountpoints"))
	{
		ServerConfig::getInstance()->setUpstreamMountpoints(vm["mountpoints"].as<string>());
	}

	if (vm.count("help"))
	{
		cout << desc << "\n";

		return 1;
	}
	
	if (vm.count("port"))
	{
		ServerConfig::getInstance()->setPort(vm["port"].as<int>());
	}

	if (vm.count("preserveads"))
	{
		ServerConfig::getInstance()->setReplaceAds(false);
	}

	
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	return 0;
}
