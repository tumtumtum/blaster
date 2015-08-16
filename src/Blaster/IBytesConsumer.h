//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include <stdint.h>
#include <stddef.h>

namespace Blaster
{
	class IBytesConsumer
	{
	public:
		virtual void consume(void* sender, const char* data, size_t length) = 0;
	};
}