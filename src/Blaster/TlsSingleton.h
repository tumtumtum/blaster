//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "uv.h"

namespace Blaster
{
	template <typename T>
	class TlsSingleton
	{
	private:
		static uv_key_t tls_key;

	public:
		TlsSingleton()
		{
		}

		virtual ~TlsSingleton()
		{
		}

		static uv_key_t init()
		{
			uv_key_t value;

			uv_key_create(&value);

			return value;
		}

		static T* getInstance()
		{
			auto instance = (T*)uv_key_get(&tls_key);

			if (instance == 0)
			{
				instance = T::createSingleton();

				uv_key_set(&tls_key, instance);
			}

			return instance;
		}
	};

	template <typename T>
	uv_key_t TlsSingleton<T>::tls_key = TlsSingleton<T>::init();
}