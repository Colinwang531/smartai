// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// Base class of TCP Reader.
//

#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		typedef boost::function<void(boost::asio::ip::tcp::socket*, const unsigned char*, const unsigned long long, boost::system::error_code)> PostRemoteReadCallback;

		class TCPReceiver : public boost::enable_shared_from_this<TCPReceiver>
		{
		public:
			TCPReceiver(PostRemoteReadCallback callback = NULL);
			virtual ~TCPReceiver(void);

		public:
			// @s : SOCKET句柄.
			// @timeout : 读取超时时间(单位:秒),默认不超时.
			// 
			void asyncRead(boost::asio::ip::tcp::socket* s = NULL, const unsigned int timeout = 0);

		private:
			void postReadDataCallback(
				boost::asio::ip::tcp::socket* s, const unsigned long long transfferedBytes, boost::system::error_code e);
			void postTimeoutCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e);

		private:
			PostRemoteReadCallback postRemoteReadCallback;
			unsigned long long readDataBytes;
			enum
			{
				MTU = 65535
			};
			unsigned char readData[MTU];
		};//class TCPReceiver
	}//namespace network
}//namespace framework

#endif//TCP_RECEIVER_H
