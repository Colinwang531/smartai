// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// Base class of TCP server.
//

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "Network/Asio/Service.h"
using ASIOServicePtr = boost::shared_ptr<framework::network::ASIOService>;

namespace framework
{
	namespace network
	{
		typedef enum class tagTCPEvent_t : unsigned char
		{
			TCP_EVENT_NONE = 0,
			TCP_EVENT_ACCEPT,
			TCP_EVENT_READ
		}TCPEvent;

		typedef struct tagTCPPacket_t
		{
			TCPEvent event;
			const char* ID;
			const char* data;
			unsigned long long bytes;
			int error;
		}TCPPacket;

		// TCP事件通知回调函数.
		//
		// @const TCPEvent : TCP事件.
		// @const char* : 数据缓存.
		// @const unsigned int : 数据字节数.
		// @const int : 事件错误码.
		// 
		typedef boost::function<void(const TCPPacket&)> TCPEventNotificationCallback;

		class TCPServer
		{
		public:
			TCPServer(TCPEventNotificationCallback callback = NULL);
			virtual ~TCPServer(void);

			int startServer(const unsigned short port = 27820);
			int stopServer(void);

		private:
			void postRemoteConnectedCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e);
			void postRemoteReadCallback(
				boost::asio::ip::tcp::socket* s, const char* data, const unsigned int dataByte, boost::system::error_code e);

		private:
			ASIOServicePtr asioServicePtr;
			TCPEventNotificationCallback tcpEventNotificationCallback;
		};//class TCPServer
	}//namespace network
}//namespace framework

#endif//TCP_SERVICE_H
