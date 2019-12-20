#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "boost/thread/mutex.hpp"
#include "error.h"
#include "DataStruct/UnorderedMap.h"
#include "Network/Asio/TCPClient.h"
using TCPClient = framework::network::TCPClient;
using TCPClientPtr = boost::shared_ptr<TCPClient>;
#include "MediaSession/TCPClientSession.h"
using TCPClientSession = framework::network::TCPClientSession;
using TCPClientSessionPtr = boost::shared_ptr<TCPClientSession>;
#include "MediaStreamClient.h"

boost::mutex mtx;
int sessionID = 0;
UnorderedMap<const int, TCPClientSessionPtr> TCPClientSessionGroup;
TCPClientPtr TCPClientServicePtr;

int MEDIASTREAMCLIENT_OpenStream(
	const char* url/* = NULL*/, MEDIASTREAMCLIENT_MediaStreamDataCallback callback/* = NULL*/, void* userData/* = NULL*/)
{
	int status{ url ? ERR_OK : ERR_INVALID_PARAM };

	if (!TCPClientServicePtr && ERR_OK == status)
	{
		TCPClientPtr tcpclientPtr{ boost::make_shared<TCPClient>() };
		if (tcpclientPtr && ERR_OK == tcpclientPtr->startClient())
		{
			TCPClientServicePtr.swap(tcpclientPtr);
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}

	if (ERR_OK == status)
	{
		mtx.lock();
		++sessionID;
		mtx.unlock();

		TCPClientSessionPtr sessionPtr{ 
			boost::make_shared<TCPClientSession>(sessionID, boost::bind(callback, _1, _2, _3, _4, _5), userData) };
		if (sessionPtr)
		{
			status = sessionPtr->createNewSession(TCPClientServicePtr->getIdleIO(), url);

			if (ERR_OK == status)
			{
				TCPClientSessionGroup.insert(sessionID, sessionPtr);
			}
		}
	}

	return ERR_OK == status ? 0 : 0;
}

int MEDIASTREAMCLIENT_CloseStream(const int streamID/* = 0*/)
{
	int status{ 0 < streamID ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		TCPClientSessionPtr sessionPtr{ TCPClientSessionGroup.at(streamID) };
		if (sessionPtr)
		{
			sessionPtr->destroySession();
			TCPClientSessionGroup.remove(streamID);

			if (!TCPClientSessionGroup.size() && TCPClientServicePtr)
			{
				TCPClientServicePtr->stopClient();
				TCPClientServicePtr.reset();
			}
		}
		else
		{
			status = ERR_NOT_FOUND;
		}
	}

	return status;
}
