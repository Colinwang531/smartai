#include "boost/functional/factory.hpp"
#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "Network/Asio/TCPConnector.h"

namespace framework
{
	namespace network
	{
		TCPConnector::TCPConnector(PostConnectedCallback callback /* = nullptr */)
			: postConnectedCallback{ callback }
		{}

		TCPConnector::~TCPConnector()
		{}

		void TCPConnector::asyncConnect(
			boost::asio::io_service& service, const char* address /* = nullptr */, const unsigned short port /* = 27820 */)
		{
			boost::asio::ip::tcp::socket* s{ boost::factory<boost::asio::ip::tcp::socket*>()(service) };
			if (s)
			{
				s->async_connect(
					boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port),
					boost::bind(
						&TCPConnector::postAsyncConnectCallback,
						boost::enable_shared_from_this<TCPConnector>::shared_from_this(),
						s,
						boost::asio::placeholders::error));
			}
		}

		void TCPConnector::postAsyncConnectCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e)
		{
			boost::asio::ip::tcp::socket* so{ 0 == e.value() && s ? s : NULL };

			if (postConnectedCallback)
			{
				postConnectedCallback(so, e);
			}

			if (0 < e.value())
			{
				s->close();
				boost::checked_delete(s);
			}
		}

	}//namespace network
}//namespace framework
