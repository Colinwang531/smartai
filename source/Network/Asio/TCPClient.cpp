#include "boost/make_shared.hpp"
#include "error.h"
#include "Hardware/Cpu.h"
using CPU = framework::hardware::Cpu;
#include "Network/Asio/TCPClient.h"

namespace framework
{
	namespace network
	{
		TCPClient::TCPClient()
		{}

		TCPClient::~TCPClient()
		{}

		int TCPClient::startClient()
		{
			int status{ asioServicePtr ? ERR_EXISTED : ERR_OK };

			if (ERR_OK == status)
			{
				const unsigned char cores{ static_cast<unsigned char>(CPU().getCoreNumber()) };
				ASIOServicePtr servicePtr{ boost::make_shared<ASIOService>() };
				if (servicePtr && 0 < servicePtr->startService(cores))
				{
					asioServicePtr.swap(servicePtr);
				}
			}

			return status;
		}

		int TCPClient::stopClient(void)
		{
			int status{ asioServicePtr ? ERR_OK : ERR_BAD_OPERATE };

			if (ERR_OK == status)
			{
				asioServicePtr->stopService();
			}

			return status;
		}

		boost::asio::io_service& TCPClient::getIdleIO(void)
		{
			return asioServicePtr->getIdleIO();
		}
	}//namespace network
}//namespace framework
