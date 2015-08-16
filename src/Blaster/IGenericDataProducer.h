//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include "Url.h"
#include "GenericQueue.h"
#include "ISupportsNotifications.h"

namespace Blaster
{
	template <typename T>
	class IGenericDataProducer;

	template <typename T>
	class IGenericDataProducerDelegate
	{
	public:
		virtual void onProducerEndReached(IGenericDataProducer<T>* producer) = 0; 
		virtual void onProducerDataAvailable(IGenericDataProducer<T>* producer) = 0;
	};

	template <typename T>
	class IGenericDataProducer
		: public ISupportsNotifications<IGenericDataProducerDelegate<T>>
	{
	public:
		virtual const Url& getUrl() = 0;
		virtual GenericQueue<T>* getQueue() = 0;
	};
}