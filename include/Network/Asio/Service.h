// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of ASIO service.
//

#ifndef ASIO_SERVICE_H
#define ASIO_SERVICE_H

#include <vector>
#include "boost/asio.hpp"
using IOContexts = std::vector<boost::asio::io_service>;
using IOWorks = std::vector<boost::asio::io_service::work>;
#include "boost/thread.hpp"
#include "Mutex/RWLock.h"

namespace framework
{
	namespace network
	{
		class ASIOService
		{
		public:
			ASIOService(void);
			virtual ~ASIOService(void);

			// Return : Total number of threads were created.
			//
			int startService(const unsigned char threadNumber = 1);
			void stopService(void);
			boost::asio::io_service& getIdleIO(void);

		protected:
			IOContexts ctxs;
			IOWorks works;
			boost::thread_group threadGroup;
			unsigned char idleIndex;
			SharedMutex mtx;
		};//class ASIOService
	}//namespace network
}//namespace framework

#endif//ASIO_SERVICE_H
