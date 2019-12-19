#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "Url/Url.h"
using URL = framework::wrapper::URL;
#include "Network/Asio/TCPConnector.h"
using TCPConnector = framework::network::TCPConnector;
using TCPConnectorPtr = boost::shared_ptr<TCPConnector>;
#include "MediaSession/TCPClientSession.h"

namespace framework
{
	namespace network
	{
		TCPClientSession::TCPClientSession(
			const int ID /* = 0 */, PostMediaStreamDataCallback callback /* = NULL */, void* ctx /* = NULL */)
			: sessionID{ ID }, postMediaStreamDataCallback{ callback }, userData{ ctx }
		{}

		TCPClientSession::~TCPClientSession()
		{}

		int TCPClientSession::createNewSession(boost::asio::io_service& service, const std::string url)
		{
			int status{ !service.stopped() && !url.empty() ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				TCPConnectorPtr connectorPtr{ 
					boost::make_shared<TCPConnector>(boost::bind(&TCPClientSession::postConnectedCallback, this, _1, _2)) };
				if (connectorPtr)
				{
					URL streamUrl{ url };
					std::vector<std::string> serverAddrPort;
					boost::split(serverAddrPort, streamUrl.getParameter("sms"), boost::is_any_of(":"));

					if (2 == serverAddrPort.size() && !serverAddrPort[0].empty() && !serverAddrPort[1].empty())
					{
						this->streamUrl.append(url);
						connectorPtr->asyncConnect(
							service, serverAddrPort[0].c_str(), static_cast<unsigned short>(atoi(serverAddrPort[1].c_str())));
					}
					else
					{
						status = ERR_INVALID_ADDRESS;
					}
				}
				else
				{
					status = ERR_BAD_ALLOC;
				}
			}

			return status;
		}

		int TCPClientSession::destroySession()
		{
			return 0;
		}

		void TCPClientSession::postConnectedCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e)
		{
			if (s && !e)
			{

			}

			if (postMediaStreamDataCallback)
			{
				postMediaStreamDataCallback(
					sessionID, static_cast<unsigned char>(TCPClientEvent::TCP_CLIENT_EVENT_CONNECTED), NULL, 0, userData);
			}
		}

// 		void TCPClientSession::receivingData(
// 			boost::system::error_code error, std::size_t transferBytes, const char* transferData /* = nullptr */)
// 		{
// 			if (afterReadDataNotificationCallback)
// 			{
// 				afterReadDataNotificationCallback(tcpSocket, error.value(), transferData, transferBytes);
// 			}
// 
// 			TCPSession::receivingData(error, transferBytes, transferData);
// 		}
// 
// 		void TCPClientSession::settingTimeout(const UInt32 timeo /*= 0*/)
// 		{
// 			if (0 < timeo && tcpSocket)
// 			{
// 				boost::posix_time::seconds tempTimeoutSeconds{ timeo };
// 				TimerPtr tempTimeoutPtr{
// 					boost::make_shared<boost::asio::deadline_timer>(tcpSocket->get_io_service(), tempTimeoutSeconds) };
// 
// 				if (tempTimeoutPtr)
// 				{
// 					tempTimeoutPtr->async_wait(
// 						boost::bind(
// 							&TCPReceiver::afterReadDataTimeoutNotificationCallback,
// 							boost::enable_shared_from_this<TCPReceiver>::shared_from_this(),
// 							tcpSocket,
// 							boost::asio::placeholders::error,
// 							tempTimeoutPtr));
// 				}
// 			}
// 		}
	}//namespace network
}//namespace framework
