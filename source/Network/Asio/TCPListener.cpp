#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "boost/make_shared.hpp"
#include "predef.h"
#include "error.h"
#include "Network/Asio/TCPListener.h"

namespace framework
{
	namespace network
	{
		TCPListener::TCPListener(PostRemoteConnectedCallback callback /* = NULL */)
			: postRemoteConnectedCallback{ callback }
		{}

		TCPListener::~TCPListener()
		{}

		int TCPListener::asyncAccept(boost::asio::io_service& service, const unsigned short port /* = 27820 */)
		{
			int status{ !service.stopped() && minPortNumber < port && maxPortNumber > port ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				boost::asio::ip::tcp::socket* s{ boost::factory<boost::asio::ip::tcp::socket*>()(service) };
				boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor{
					boost::make_shared<boost::asio::ip::tcp::acceptor>(
						service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) };

				if (s && acceptor)
				{
					//After the acceptor has been initialized, listen() is called to make the acceptor start listening.
					//Then async_accept() is called to accept the first connection attempt.
					//A socket has to be passed as a first parameter to async_accept(), which will be used to send and receive data on a new connection.
					acceptor->listen();
					acceptor->async_accept(*s,
						boost::bind(
							&TCPListener::postAsyncAcceptNotificationCallback,
							boost::enable_shared_from_this<TCPListener>::shared_from_this(),
							s,
							acceptor,
							boost::asio::placeholders::error));
				}
				else
				{
					boost::checked_delete(s);
					status = ERR_BAD_ALLOC;
				}
			}

			return status;
		}

		void TCPListener::postAsyncAcceptNotificationCallback(
			boost::asio::ip::tcp::socket* s, boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor, boost::system::error_code e)
		{
			if (!e)
			{
				boost::asio::ip::tcp::socket* so{
					boost::factory<boost::asio::ip::tcp::socket*>()(acceptor->get_io_service()) };

				if (so)
				{
					acceptor->async_accept(*so,
						boost::bind(
							&TCPListener::postAsyncAcceptNotificationCallback,
							boost::enable_shared_from_this<TCPListener>::shared_from_this(),
							so,
							acceptor,
							boost::asio::placeholders::error));
				}
			}

			if (postRemoteConnectedCallback)
			{
				postRemoteConnectedCallback(s, e);
			}
		}
	}//namespace network
}//namespace framework
