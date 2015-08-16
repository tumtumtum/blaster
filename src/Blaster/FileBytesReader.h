//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#pragma once
#include "uv.h"
#include <string>
#include "ISupportsNotifications.h"
#include "IBytesConsumer.h"

namespace Blaster
{
	namespace IO
	{
		using namespace std;

		class FileBytesReader;

		class FileBytesReader
		{
		private:
			bool* indicator;
			uv_loop_t* loop;
			string filepath;
			IBytesConsumer* consumer;
			uv_fs_t open_req;
			uv_fs_t close_req;
			uv_fs_t read_req;
			uv_fs_t write_req;
			uv_buf_t input_buffer;
			uint64_t totalread;
			char buffer[32 * 1024];

			static void onOpen(uv_fs_t* req);
			static void onRead(uv_fs_t* req);
			static void onClose(uv_fs_t* req);

		protected:
			virtual void onClosed();

		public:
			FileBytesReader(uv_loop_t* loop, const string& filepath, IBytesConsumer* consumer);
			virtual ~FileBytesReader();

			bool dispose();
		};
	}
}