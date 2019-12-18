// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of TCP listener.
//

#ifndef TCP_LISTENER_H
#define TCP_LISTENER_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		typedef boost::function<void(boost::asio::ip::tcp::socket*, boost::system::error_code)> PostRemoteConnectedCallback;

		class TCPListener : public boost::enable_shared_from_this<TCPListener>
		{
		public:
			TCPListener(PostRemoteConnectedCallback callback = NULL);
			virtual ~TCPListener(void);

		public:
			int asyncAccept(boost::asio::io_service& service, const unsigned short port = 27820);

		private:
			void postAsyncAcceptNotificationCallback(
				boost::asio::ip::tcp::socket* s,
				boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor,
				boost::system::error_code e);

		private:
			PostRemoteConnectedCallback postRemoteConnectedCallback;
		};//class TCPListener
	}//namespace network
}//namespace framework

#endif//TCP_LISTENER_H
