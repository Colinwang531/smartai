// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of TCP client.
//

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "boost/shared_ptr.hpp"
#include "Network/Asio/Service.h"
using ASIOServicePtr = boost::shared_ptr<framework::network::ASIOService>;

namespace framework
{
	namespace network
	{
		class TCPClient
		{
		public:
			TCPClient(void);
			virtual ~TCPClient(void);

		public:
			int startClient(void);
			int stopClient(void);
			boost::asio::io_service& getIdleIO(void);

		private:
			ASIOServicePtr asioServicePtr;
		};//class TCPServer
	}//namespace network
}//namespace framework

#endif//TCP_SERVICE_H
