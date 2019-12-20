// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of TCP sender.
//

#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace framework
{
	namespace network
	{
		class TCPSender : public boost::enable_shared_from_this<TCPSender>
		{
		public:
			TCPSender(void);
			virtual ~TCPSender(void);

		public:
			void asyncSend(
				boost::asio::ip::tcp::socket* s = NULL, const unsigned char* data = NULL, const unsigned long long dataBytes = 0);

		private:
			void postAsyncSendCallback(boost::system::error_code e, std::size_t transferredBytes = 0);
		};//class TCPSender
	}//namespace network
}//namespace framework

#endif//TCP_SENDER_H
