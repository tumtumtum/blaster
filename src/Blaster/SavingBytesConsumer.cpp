//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include "SavingBytesConsumer.h"

namespace Blaster
{
	namespace IO
	{
		SavingBytesConsumer::SavingBytesConsumer(ostream* stream)
			: stream(stream), bytesSaved(0)
		{
		}

		SavingBytesConsumer::SavingBytesConsumer(const string& filename)
			: stream(new ofstream(filename, std::ofstream::binary)), bytesSaved(0)
		{
		}

		SavingBytesConsumer::~SavingBytesConsumer()
		{
			delete stream;
		}

		void SavingBytesConsumer::consume(void* sender, const char* data, size_t length)
		{
			stream->write(data, length);
			stream->flush();

			if (this->callback)
			{
				this->callback(this, data, length);
			}
		}

		void SavingBytesConsumer::setCallback(function<void(SavingBytesConsumer* saver, const char* data, size_t length)> callback)
		{
			this->callback = std::move(callback);
		}

		function<void(SavingBytesConsumer* saver, const char* data, size_t length)> SavingBytesConsumer::getCallback()
		{
			return this->callback;
		}

		size_t SavingBytesConsumer::getTotalBytesSaved() const
		{
			return this->bytesSaved;
		}
	}
}