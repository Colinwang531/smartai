#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "Url/Url.h"
using URL = framework::wrapper::URL;
#include "Network/Asio/TCPConnector.h"
using TCPConnector = framework::network::TCPConnector;
using TCPConnectorPtr = boost::shared_ptr<TCPConnector>;
#include "Network/Asio/TCPSender.h"
using TCPSender = framework::network::TCPSender;
using TCPSenderPtr = boost::shared_ptr<TCPSender>;
#include "Network/Asio/TCPReceiver.h"
using TCPReceiver = framework::network::TCPReceiver;
using TCPReceiverPtr = boost::shared_ptr<TCPReceiver>;
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
				// Get ready for reading data first.
				TCPReceiverPtr receiverPtr{ 
					boost::make_shared<TCPReceiver>( boost::bind(&TCPClientSession::postRemoteReadCallback, this, _1, _2, _3, _4)) };
				if (receiverPtr)
				{
					receiverPtr->asyncRead(s);
				}

				// Post request message to server.
				TCPSenderPtr senderPtr{ boost::make_shared<TCPSender>() };
				if (senderPtr)
				{
					senderPtr->asyncSend(s, (const unsigned char*)streamUrl.c_str(), streamUrl.length());
				}
			}

			if (postMediaStreamDataCallback)
			{
				postMediaStreamDataCallback(
					sessionID, static_cast<unsigned char>(TCPClientEvent::TCP_CLIENT_EVENT_CONNECTED), NULL, 0, userData);
			}
		}

		void TCPClientSession::postRemoteReadCallback(
			boost::asio::ip::tcp::socket* s, const unsigned char* data, const unsigned long long dataByte, boost::system::error_code e)
		{
			if (postMediaStreamDataCallback)
			{
				postMediaStreamDataCallback(
					sessionID, static_cast<unsigned char>(TCPClientEvent::TCP_CLIENT_EVENT_READ), data, dataByte, userData);
			}
		}
	}//namespace network
}//namespace framework
