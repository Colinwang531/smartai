// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// Base class of timer.
//

#ifndef ASYNC_TIMER_H
#define ASYNC_TIMER_H

#include "boost/asio.hpp"
using DLTimerPtr = boost::shared_ptr<boost::asio::deadline_timer>;
#include "boost/function.hpp"
#include "boost/smart_ptr/enable_shared_from_this.hpp"

namespace framework
{
	namespace base
	{
		typedef boost::function<void(boost::asio::ip::tcp::socket*, boost::system::error_code)> PostTimeoutCallback;

		class Timer : public boost::enable_shared_from_this<Timer>
		{
		public:
			Timer(PostTimeoutCallback callback = NULL);
			~Timer(void);

		public:
			// @s : SOCKET句柄.
			// @timeout : 超时时间(单位:秒),默认不超时.
			// 
			void asyncWait(boost::asio::ip::tcp::socket* s = NULL, const unsigned int timeout = 0);

		private:
			void postTimeoutNotificationCallback(
				boost::asio::ip::tcp::socket* s, DLTimerPtr timerPtr, boost::system::error_code error);

		private:
			PostTimeoutCallback postTimeoutCallback;
		};//class Timer
	}//namespace network
}//namespace framework

#endif//ASYNC_TIMER_H
