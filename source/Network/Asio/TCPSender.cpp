#include "boost/bind.hpp"
#include "Network/Asio/TCPSender.h"

namespace framework
{
	namespace network
	{
		TCPSender::TCPSender()
		{}

		TCPSender::~TCPSender()
		{}

		void TCPSender::asyncSend(
			boost::asio::ip::tcp::socket* s /* = NULL */, const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
		{
			if (s && s->is_open())
			{
				boost::asio::async_write(
					*s, 
					boost::asio::buffer(data, dataBytes), 
					boost::bind(
						&TCPSender::postAsyncSendCallback, 
						boost::enable_shared_from_this<TCPSender>::shared_from_this(), _1, _2));
			}
		}

		void TCPSender::postAsyncSendCallback(boost::system::error_code e, std::size_t transferredBytes)
		{}
	}
}
