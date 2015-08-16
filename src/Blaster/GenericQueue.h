#pragma once
#include <queue>
#include "ISupportsNotifications.h"
#include <boost/circular_buffer.hpp>

namespace Blaster
{
	using namespace std;
	using namespace boost;	

	template<typename T>
	class GenericQueue;

	template<typename T>
	class GenericQueueDelegate
	{
	public:
		virtual void queueChanged(GenericQueue<T>* queue) = 0;
		virtual void queueTerminated(GenericQueue<T>* queue) = 0;
	};

	template <typename UNIT>
	class GenericQueue : public ISupportsNotifications < GenericQueueDelegate<UNIT> >
	{
	protected:
		int depth;
		bool circular;
		uint64_t serial;
		uint64_t beginSerial;
		circular_buffer<UNIT> units;

		virtual void onQueueChanged()
		{
			if (this->delegate)
			{
				this->delegate->queueChanged(this);
			}
		}

		virtual void onTerminated()
		{
			if (this->delegate)
			{
				this->delegate->queueTerminated(this);
			}
		}


		virtual void checkCapacity(size_t countMore)
		{
			assert(countMore >= 0);

			if (circular)
			{
				return;
			}

			if (units.reserve() < countMore)
			{
				auto target = units.size() + countMore;
				auto newCapacity = units.capacity() * 2;

				while (newCapacity < target)
				{
					newCapacity *= 2;
				}

				units.set_capacity(newCapacity);
			}
		}

	public:
		GenericQueue(size_t initialCapacity = 8, bool circular = false)
			: units(initialCapacity), serial(0), beginSerial(-1), depth(0), circular(circular)
		{
		}

		virtual ~GenericQueue()
		{
		}

		virtual size_t size() const
		{
			return units.size();
		}

		inline bool shouldNotify()
		{
			return beginSerial == -1;
		}

		virtual size_t reserve() const
		{
			return units.reserve();
		}

		virtual size_t capacity() const
		{
			return units.capacity();
		}

		virtual void compact()
		{
			units.set_capacity(units.size());
		}
		
		virtual void enqueue(queue<UNIT>& queue)
		{
			if (units.capacity() == 0 && circular)
			{
				return;
			}

			checkCapacity(queue.size());

			while (!queue.empty())
			{
				this->enqueue(queue.front());

				queue.pop();
			}

			if (shouldNotify())
			{
				onQueueChanged();
			}
		}

		virtual void beginChanges()
		{
			if (depth == 0)
			{
				beginSerial = serial;
			}

			depth++;
		}

		virtual bool commitChanges()
		{
			depth--;

			if (depth == 0)
			{
				if (beginSerial != serial)
				{
					beginSerial = -1;

					onQueueChanged();

					return true;
				}

				beginSerial = -1;
			}

			return false;
		}

		virtual void enqueue(GenericQueue<UNIT>& queue)
		{
			this->enqueue(queue, 0, -1);
		}

		virtual void enqueue(GenericQueue<UNIT>& queue, int skip, int take)
		{
			if (units.capacity() == 0 && circular)
			{
				return;
			}

			checkCapacity(take >= 0 ? take : queue.size() - skip);

			auto begin = queue.units.begin() + skip;
			auto end = take < 0 ? queue.units.end() : queue.units.begin() + skip + take;

			serial++;

			units.insert(units.end(), begin, end);

			if (beginSerial == -1)
			{
				onQueueChanged();
			}
		}

		virtual void enqueue(const UNIT& frame)
		{
			if (units.capacity() == 0 && circular)
			{
				return;
			}

			checkCapacity(units.size() + 1);

			serial++;
			units.push_back(frame);

			if (beginSerial == -1)
			{
				onQueueChanged();
			}
		}

		virtual void clear()
		{
			serial++;
			units.clear();

			if (this->delegate != 0)
			{
				this->delegate->queueChanged(this);
			}
		}

		const UNIT& peek() const
		{
			return units.front();
		}

		const UNIT dequeue()
		{
			UNIT retval = units.front();

			serial++;
			units.pop_front();

			return retval;
		}

		circular_buffer<UNIT>& getBuffer()
		{
			return units;
		}

		virtual void terminate()
		{
			this->onTerminated();
		}
	};
}