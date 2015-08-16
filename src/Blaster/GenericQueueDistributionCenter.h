//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include "Url.h"
#include "HttpRequest.h"
#include "ServerConfig.h"
#include "OutgoingHttpConnection.h"
#include "IncomingIcecastResponse.h"
#include <iostream>
#include "ServerConfig.h"
#include <boost/filesystem.hpp>
#include "UrlPathUtils.h"

namespace Blaster
{
	using namespace std;
	using namespace Http;

	template <typename T>
	class GenericQueueDistributionCenter
		: protected IGenericDataProducerDelegate < T >
	{
	protected:
		struct ListenerInfo
		{
			GenericQueue<T>* queue;
			uint64_t framesWritten;

			ListenerInfo()
				: queue(0), framesWritten(0)
			{
			}

			ListenerInfo(GenericQueue<T>* queue)
				: queue(queue), framesWritten(0)
			{
			}

			void incremement(size_t size)
			{
				framesWritten += size;
			}

			struct equal
			{
				size_t operator()(const ListenerInfo& a, const ListenerInfo& b) const
				{
					return a.queue == b.queue;
				}
			};

			struct hash
			{
				size_t operator()(const ListenerInfo& o) const
				{
					return (size_t)o.queue;
				}
			};
		};

		uv_loop_t* loop;
		ServerConfig* config;
		unordered_map<const Url, GenericQueue<T>*, Url::pathHash, Url::pathEqual> burstBuffer;
		unordered_map<const Url, IGenericDataProducer<T>*, Url::pathHash, Url::pathEqual> producers;
		unordered_map<const Url, unordered_map<GenericQueue<T>*, typename GenericQueueDistributionCenter<T>::ListenerInfo, typename GenericQueueDistributionCenter<T>::ListenerInfo::hash, typename GenericQueueDistributionCenter<T>::ListenerInfo::equal>, Url::pathHash, Url::pathEqual> listeners;
		unordered_set<OutgoingHttpConnection*> connectionsToBeReconnected;

		virtual void createProducer(const Url& url)
		{
		}

	public:
		GenericQueueDistributionCenter(ServerConfig* config)
			: config(config)
		{
			loop = uv_default_loop();

			for (auto point : config->getUpstreamMountpoints())
			{
				auto url = Url(UrlPathUtils::combine(config->getUpstreamUrlPrefix(), point));
				this->createProducer(url);
			}
		}

		virtual ~GenericQueueDistributionCenter()
		{
			for (auto burst : burstBuffer)
			{
				delete burst.second;
			}

			burstBuffer.clear();
		}
		
		virtual void attachProducer(IGenericDataProducer<T>* producer)
		{
			const Url& url = producer->getUrl();
			auto existing = this->producers[url];

			if (existing)
			{
				existing->setDelegate(nullptr);
			}

			producers[url] = producer;

			auto burst = burstBuffer.find(url);

			if (burst != burstBuffer.end())
			{
				burst->second->clear();
			}
			else
			{
				burstBuffer[url] = new GenericQueue<T>(config->getDownstreamUserBuffer(), true);
			}

			producer->ISupportsNotifications<IGenericDataProducerDelegate<T>>::setDelegate(this);
		}

		virtual void detatchProducer(IGenericDataProducer<T>* producer)
		{
			auto burst = burstBuffer.find(producer->getUrl());

			if (burst != burstBuffer.end())
			{
				burst->second->clear();
			}

			producer->ISupportsNotifications<IGenericDataProducerDelegate<T>>::setDelegate(nullptr);;
			producers.erase(producer->getUrl());
		}

		void registerQueue(const Url& url, GenericQueue<T>* queue)
		{
			this->listeners[url][queue] = ListenerInfo(queue);

			if (this->producers.find(url) == this->producers.end())
			{
				this->createProducer(url);
			}
			else
			{
				auto burst = burstBuffer.find(url);

				if (burst != burstBuffer.end())
				{
					if (burst->second->reserve() == 0 && burst->second->capacity() > 0)
					{
						auto listener = this->listeners[url].find(queue);

						listener->second.queue->enqueue(*burst->second);
					}
				}
			}
		}

		void deregisterQueue(const Url& url, GenericQueue<T>* queue)
		{
			auto listener = this->listeners.find(url);

			if (listener == listeners.end())
			{
				return;
			}

			listener->second.erase(queue);
		}

	protected:

		virtual void onProducerDataAvailable(IGenericDataProducer<T>* producer)
		{
			this->onProducerDataAvailable(producer->getUrl(), *producer->getQueue());
		}

		virtual void onProducerDataAvailable(const Url& url, GenericQueue<T>& queue)
		{
			auto burstBufferWasPartial = false;
			auto listeners = this->listeners[url];
			auto result = burstBuffer.find(url);

			if (result != burstBuffer.end() && result->second->capacity() > 0)
			{
				if (result->second->reserve() != 0)
				{
					burstBufferWasPartial = true;
				}

				result->second->enqueue(queue);
			}

			auto shouldQueueBurst = burstBufferWasPartial && result->second->reserve() == 0;

			for (auto value : listeners)
			{
				if (shouldQueueBurst)
				{
					value.second.queue->enqueue(*result->second);
				}

				value.second.queue->enqueue(queue);
				value.second.incremement(queue.size());
			}

			queue.clear();
		}

		virtual void onProducerEndReached(IGenericDataProducer<T>* producer)
		{
			auto url = producer->getUrl();
			auto listeners = this->listeners[url];

			for (auto listener : listeners)
			{
				listener.second.queue->terminate();
			}

			producer->setDelegate(nullptr);
		}
	};
}