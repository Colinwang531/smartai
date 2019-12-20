// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of TCP connector.
//

#ifndef TCP_CONNECTOR_H
#define TCP_CONNECTOR_H

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		typedef boost::function<void(boost::asio::ip::tcp::socket*, boost::system::error_code)> PostConnectedCallback;

		class TCPConnector : public boost::enable_shared_from_this<TCPConnector>
		{
		public:
			TCPConnector(PostConnectedCallback callback = nullptr);
			virtual ~TCPConnector(void);

		public:
			void asyncConnect(
				boost::asio::io_service& service, const char* address = nullptr, const unsigned short port = 27820);

		private:
			void postAsyncConnectCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e);

		private:
			PostConnectedCallback postConnectedCallback;
		};//class TCPConnector
	}//namespace network
}//namespace framework

#endif//TCP_CONNECTOR_H
