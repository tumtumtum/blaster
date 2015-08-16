//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

#include <map>
#include <vector>
#include "Collections.h"

namespace Blaster
{
	using namespace std;

	template<class K, class V> class KeyValueCollection
	{
	protected:
		map<K, V, CaseInsensitiveComparer<K>> valuesByKey;

	public:
		KeyValueCollection()
		{
		}

		KeyValueCollection(const KeyValueCollection<K, V>& other)
		{
			valuesByKey = other.valuesByKey;
		}

		virtual ~KeyValueCollection()
		{
		}

		const map<K, V, CaseInsensitiveComparer<K>>& getUnderlyingMap()
		{
			return valuesByKey;
		}

		bool exists(const K& key) const
		{
			auto iterator = valuesByKey.find(key);

			return iterator != valuesByKey.end();
		}

		void setValue(const K& key, const V& value)
		{
			valuesByKey[key] = value;
		}

		V getValue(const K& key) const
		{
			auto iterator = valuesByKey.find(key);
			
			if (iterator != valuesByKey.end())
			{
				return iterator->second;
			}

			return V();
		}

		std::vector<K> getKeys() const
		{
			std::vector<K> retval;

			for (auto const value : valuesByKey)
			{
				retval.push_back(value.first);
			}

			return retval;
		}

		std::vector<V> getValues() const
		{
			std::vector<V> retval;

			for (auto const value : valuesByKey)
			{
				retval.push_back(value.second);
			}

			return retval;
		}
	};
}