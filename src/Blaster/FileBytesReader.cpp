//
// Author: Thong Nguyen
// Copyright (c) 2015 tum@kingstreetapps.com
//

#include <iostream>
#include <string.h>
#include "FileBytesReader.h"

namespace Blaster
{
	namespace IO
	{
		FileBytesReader::FileBytesReader(uv_loop_t* loop, const string& filepath, IBytesConsumer* consumer)
			: filepath(filepath), consumer(consumer), loop(loop), totalread(0), indicator(0)
		{
			memset(&open_req, 0, sizeof(open_req));
			memset(&read_req, 0, sizeof(read_req));
			memset(&write_req, 0, sizeof(write_req));
			memset(&close_req, 0, sizeof(close_req));

			open_req.data = this;
			read_req.data = this;
			write_req.data = this;
			close_req.data = this;

			string prefix = "/./";

			if (this->filepath.compare(0, prefix.length(), prefix) == 0)
			{
				this->filepath = filepath.substr(prefix.length());
			}

			input_buffer = uv_buf_init(buffer, sizeof(buffer));

			uv_fs_open(loop, &open_req, this->filepath.c_str(), 0, 0, onOpen);
		}

		FileBytesReader::~FileBytesReader()
		{
			if (this->indicator)
			{
				*indicator = true;
			}

			if (this->open_req.result > 0)
			{
				uv_fs_close(this->loop, &this->close_req, (uv_file)this->open_req.result, 0);

				uv_fs_req_cleanup(&open_req);
				uv_fs_req_cleanup(&read_req);
				uv_fs_req_cleanup(&write_req);
				uv_fs_req_cleanup(&close_req);
			}
		}

		void FileBytesReader::onOpen(uv_fs_t* req)
		{
			if (req->result == 0)
			{
				return;
			}

			auto _this = (FileBytesReader*)req->data;

			if (req->result > 0)
			{
				uv_fs_read(_this->loop, &_this->read_req, (uv_file)req->result, &_this->input_buffer, 1, -1, onRead);
			}
			else
			{
				onClose(req);
			}
		}

		void FileBytesReader::onRead(uv_fs_t* req)
		{
			bool terminated = false;
			auto _this = (FileBytesReader*)req->data;

			if (req->result <= 0)
			{
				_this->indicator = &terminated;

				_this->consumer->consume(_this, _this->input_buffer.base, 0);

				if (terminated)
				{
					return;
				}
				else
				{
					_this->indicator = 0;
					uv_fs_close(_this->loop, &_this->close_req, (uv_file)_this->open_req.result, onClose);

					return;
				}
			}

			_this->totalread += req->result;
			_this->indicator = &terminated;
			_this->consumer->consume(_this, _this->input_buffer.base, req->result);

			if (terminated)
			{
				return;
			}
			else
			{
				uv_fs_read(_this->loop, &_this->read_req, (uv_file)_this->open_req.result, &_this->input_buffer, 1, -1, onRead);
			}
		}

		void FileBytesReader::onClosed()
		{
		}

		void FileBytesReader::onClose(uv_fs_t* req)
		{
			auto _this = (FileBytesReader*)req->data;

			_this->onClosed();
		}
	}
}