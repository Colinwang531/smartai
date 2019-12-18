#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Timer/Timer.h"

namespace framework
{
	namespace base
	{
		Timer::Timer(PostTimeoutCallback callback /* = nullptr */) : postTimeoutCallback{ callback }
		{}

		Timer::~Timer()
		{}

		void Timer::asyncWait(boost::asio::ip::tcp::socket* s /* = NULL */, const unsigned int timeout /* = 0 */)
		{
			if (s && s->is_open() && 0 < timeout)
			{
				DLTimerPtr timerPtr{
					boost::make_shared<boost::asio::deadline_timer>(s->get_io_service(), boost::posix_time::seconds(timeout)) };
				if (timerPtr)
				{
					timerPtr->async_wait(
						boost::bind(
							&Timer::postTimeoutNotificationCallback, 
							boost::enable_shared_from_this<Timer>::shared_from_this(),
							s,
							timerPtr,
							boost::asio::placeholders::error));
				}
			}
		}

		void Timer::postTimeoutNotificationCallback(
			boost::asio::ip::tcp::socket* s, DLTimerPtr timerPtr, boost::system::error_code error)
		{
			if (postTimeoutCallback)
			{
				postTimeoutCallback(s, error);
			}
		}
	}//namespace network
}//namespace framework
