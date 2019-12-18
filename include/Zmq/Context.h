// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of ZMQ context.
//

#ifndef ZMQ_CONTEXT_H
#define ZMQ_CONTEXT_H

#include "predef.h"

namespace framework
{
	namespace network
	{
		typedef enum class tagSocketType_t : unsigned char
		{
			SOCKET_TYPE_NONE = 0,
			SOCKET_TYPE_PUBLISHER,
			SOCKET_TYPE_REQ = 3,
			SOCKET_TYPE_REP,
			SOCKET_TYPE_DEALER,
			SOCKET_TYPE_ROUTER,
		}SocketType;

		class ZMQContext
		{
		public:
			ZMQContext(void);
			virtual ~ZMQContext(void);

		public:
			int createNewContext(const unsigned char threadNumber = 1);
			int destroyContext(void);
			void* createNewSocket(const SocketType type = SocketType::SOCKET_TYPE_NONE);
			int closesocket(void* s = NULL);

		protected:
			//Context instance must be created only once.
			void* ctx;
		};//class ZMQContext
	}//namespace network
}//namespace framework

#endif//ZMQ_CONTEXT_H
