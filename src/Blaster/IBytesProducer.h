//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "Url.h"
#include "BytesQueue.h"
#include "ISupportsNotifications.h"

namespace Blaster
{
	class IBytesProducer;

	class IBytesProducerDelegate
	{
	public:
		virtual void onBytesAvailable(IBytesProducer* producer) = 0;
	};

	class IBytesProducer
		: public ISupportsNotifications < IBytesProducerDelegate >
	{
	public:
		virtual const Url& getUrl() = 0;
		virtual BytesQueue* getAdtsQueue() = 0;
	};
}