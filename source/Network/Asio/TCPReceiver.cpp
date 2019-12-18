#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Timer/Timer.h"
using TimerPtr = boost::shared_ptr<framework::base::Timer>;
#include "Network/Asio/TCPReceiver.h"

namespace framework
{
	namespace network
	{
		TCPReceiver::TCPReceiver(PostRemoteReadCallback callback /* = NULL */)
			: postRemoteReadCallback{ callback }, readDataBytes{ 0 }
		{}

		TCPReceiver::~TCPReceiver()
		{}

		void TCPReceiver::asyncRead(boost::asio::ip::tcp::socket* s /* = NULL */, const unsigned int timeout /* = 0 */)
		{
			if (s && s->is_open())
			{
				s->async_read_some(
					boost::asio::buffer(readData, MTU),
					boost::bind(
						&TCPReceiver::postReadDataCallback,
						boost::enable_shared_from_this<TCPReceiver>::shared_from_this(),
						s,
						boost::asio::placeholders::bytes_transferred,
						boost::asio::placeholders::error));

				if (0 < timeout)
				{
					TimerPtr timerPtr{
						boost::make_shared<framework::base::Timer>(
							boost::bind(&TCPReceiver::postTimeoutCallback, this, _1, _2)) };
					if (timerPtr)
					{
						timerPtr->asyncWait(s, timeout);
					}
				}
			}
		}

		void TCPReceiver::postReadDataCallback(
			boost::asio::ip::tcp::socket* s, const unsigned long long transfferedBytes, boost::system::error_code e)
		{
			readDataBytes += transfferedBytes;

			if (postRemoteReadCallback)
			{
				postRemoteReadCallback(s, readData, transfferedBytes, e);
			}

			if (s && !e)
			{
				asyncRead(s);
			}
		}

		void TCPReceiver::postTimeoutCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e)
		{
			if (!readDataBytes && postRemoteReadCallback)
			{
				postRemoteReadCallback(s, NULL, 0, e);
			}
		}
	}//namespace network
}//namespace framework
