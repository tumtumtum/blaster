//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "GenericQueue.h"

namespace Blaster
{
	using namespace boost;

	class BytesQueue;

	class BytesQueueDelegate
		: public GenericQueueDelegate<uint8_t>
	{
	public:
		virtual void queueChanged(BytesQueue* queue) = 0;
		virtual void queueTerminated(BytesQueue* queue) = 0;

		virtual void queueChanged(GenericQueue<uint8_t>* queue)
		{
			this->queueChanged((BytesQueue*)queue);
		}

		virtual void queueTerminated(GenericQueue<uint8_t>* queue)
		{
			this->queueTerminated((BytesQueue*)queue);
		}
	};

	class BytesQueue : public GenericQueue<uint8_t>
	{
	public:
		BytesQueue(size_t initialCapacity = 8, bool circular = false)
			: GenericQueue<uint8_t>(initialCapacity, circular)
		{
		}

		virtual ~BytesQueue()
		{
		}
	};
}