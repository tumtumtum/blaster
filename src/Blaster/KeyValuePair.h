//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

namespace Blaster
{
	template<class K, class V> class KeyValuePair
	{
	private:
		K key;
		V value;
	public:
		KeyValuePair()
		{
		}

		KeyValuePair(K key, V value)
		{
			this->key = key;
			this->value = value;
		}

		virtual ~KeyValuePair()
		{
		}

		inline K& getKey()
		{
			return this->key;
		}

		inline V& getValue()
		{
			return this->value;
		}

		inline void setKey(K& key)
		{
			this->k = key;
		}

		inline void setValue(V& value)
		{
			this->v = value;
		}
	};
}