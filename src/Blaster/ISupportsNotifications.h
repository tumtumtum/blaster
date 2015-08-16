//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once

namespace Blaster
{
	template <class T>
	class ISupportsNotifications
	{
	protected:
		T* delegate;

	public:
		ISupportsNotifications()
			: delegate(0)
		{
		}

		void setDelegate(T* value)
		{
			delegate = value;
		}

		T* getDelegate()
		{
			return delegate;
		}
	};
}