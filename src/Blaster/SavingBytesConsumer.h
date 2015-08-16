//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "IBytesConsumer.h"
#include <fstream>
#include <iostream>
#include <functional>

namespace Blaster
{
	namespace IO
	{
		using namespace std;

		class SavingBytesConsumer
			: public IBytesConsumer
		{
		private:
			ostream* stream;
			size_t bytesSaved;
			std::function<void(SavingBytesConsumer* saver, const char* data, size_t length)> callback;

		public:
			SavingBytesConsumer(ostream* stream);
			SavingBytesConsumer(const string& filename);
			virtual ~SavingBytesConsumer();

			virtual void consume(void* sender, const char* data, size_t length);

			size_t getTotalBytesSaved() const;
			void setCallback(std::function<void(SavingBytesConsumer* saver, const char* data, size_t length)> callback);
			std::function<void(SavingBytesConsumer* saver, const char* data, size_t length)> getCallback();
		};
	}
}