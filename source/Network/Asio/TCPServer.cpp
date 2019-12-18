#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "Hardware/Cpu.h"
using CPU = framework::hardware::Cpu;
#include "Network/Asio/TCPListener.h"
#include "Network/Asio/TCPReceiver.h"
#include "Network/Asio/TCPServer.h"

namespace framework
{
	namespace network
	{
		TCPServer::TCPServer(TCPEventNotificationCallback callback /* = NULL */)
			: tcpEventNotificationCallback{ callback }
		{}

		TCPServer::~TCPServer()
		{}

		int TCPServer::startServer(const unsigned short port /* = 27820 */)
		{
			int status{ asioServicePtr ? ERR_EXISTED : ERR_OK };

			if (ERR_OK == status)
			{
				const unsigned char cores{ static_cast<unsigned char>(CPU().getCoreNumber()) };
				ASIOServicePtr servicePtr{ boost::make_shared<ASIOService>() };
				if (servicePtr && 0 < servicePtr->startService(cores))
				{
					for (int i = 0; i != cores; ++i)
					{
						boost::shared_ptr<TCPListener> listenerPtr{
							boost::make_shared<TCPListener>(boost::bind(&TCPServer::postRemoteConnectedCallback, this, _1, _2)) };
						if (listenerPtr)
						{
							listenerPtr->asyncAccept(servicePtr->getIdleIO(), port);
						}
					}

					asioServicePtr.swap(servicePtr);
				}
			}

			return status;
		}

		int TCPServer::stopServer(void)
		{
			int status{ asioServicePtr ? ERR_OK : ERR_BAD_OPERATE };

			if (ERR_OK == status)
			{
				asioServicePtr->stopService();
			}

			return status;
		}

		void TCPServer::postRemoteConnectedCallback(boost::asio::ip::tcp::socket* s, boost::system::error_code e)
		{
			if (s && !e)
			{
				boost::asio::ip::tcp::endpoint endpoint = s->remote_endpoint();
				const std::string remoteAddress{ endpoint.address().to_string() };
				const unsigned short remotePort{ endpoint.port() };

				boost::shared_ptr<TCPReceiver> receiverPtr{
							boost::make_shared<TCPReceiver>(boost::bind(&TCPServer::postRemoteReadCallback, this, _1, _2, _3, _4)) };
				if (receiverPtr)
				{
					receiverPtr->asyncRead(s, 3);
				}

				if (tcpEventNotificationCallback)
				{
					const std::string ID{ (boost::format("%s:%d") % remoteAddress % remotePort).str() };
					TCPPacket pkt{ TCPEvent::TCP_EVENT_ACCEPT };
					pkt.ID = ID.c_str();
					pkt.data = remoteAddress.c_str();
					pkt.bytes = remotePort;
					pkt.error = e.value();
					tcpEventNotificationCallback(pkt);
				}
			}
			else
			{
				if (s)
				{
					s->close();
					boost::checked_delete(s);
				}
			}
		}

		void TCPServer::postRemoteReadCallback(
			boost::asio::ip::tcp::socket* s, const char* data, const unsigned int dataByte, boost::system::error_code e)
		{
			if (s && !e)
			{
				boost::asio::ip::tcp::endpoint endpoint = s->remote_endpoint();
				const std::string remoteAddress{ endpoint.address().to_string() };
				const unsigned short remotePort{ endpoint.port() };

				if (tcpEventNotificationCallback)
				{
					const std::string ID{ (boost::format("%s:%d") % remoteAddress % remotePort).str() };
					TCPPacket pkt{ TCPEvent::TCP_EVENT_READ };
					pkt.ID = ID.c_str();
					pkt.data = data;
					pkt.bytes = dataByte;
					pkt.error = e.value();
					tcpEventNotificationCallback(pkt);
				}
			}
			else
			{
				if (s)
				{
					s->close();
					boost::checked_delete(s);
				}
			}
		}
	}//namespace network
}//namespace framework
