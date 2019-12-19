#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "Timer/Timer.h"
using TimerPtr = boost::shared_ptr<NS(timer, 1)::Timer>;
#include "MediaSession/TCPServerSession.h"

NS_BEGIN(session, 2)

TCPServerSession::TCPServerSession(
	boost::asio::ip::tcp::socket* so /* = nullptr */, AfterServerReadDataNotificationCallback callback /* = nullptr */)
	: TCPSession(so), afterServerReadDataNotificationCallback{ callback }, transferredDataBytes{ 0 }
{}

TCPServerSession::~TCPServerSession()
{}

void TCPServerSession::receivingData(
	const char* data /*= nullptr*/, const Int32 bytes /*= 0*/, const Int32 error /*= 0*/)
{
	// It is more import that calculating data bytes of transferring before invoking callback function.
	transferredDataBytes += bytes;
	if (afterServerReadDataNotificationCallback)
	{
		afterServerReadDataNotificationCallback(tcpSocket, data, bytes, error);
	}

	TCPSession::receivingData(data, bytes, error);
}

void TCPServerSession::waitingTimeout(const UInt32 timeo /*= 0*/)
{
	TimerPtr tempTimerPtr{ 
		boost::make_shared<NS(timer, 1)::Timer>(
			boost::bind(&TCPServerSession::afterAsyncWaitNotificationCallback, this, _1)) };

	if (tempTimerPtr)		
	{
		tempTimerPtr->asyncWait(tcpSocket, timeo);
	}
}

void TCPServerSession::afterAsyncWaitNotificationCallback(const Int32 error)
{
	if (!error && !transferredDataBytes)
	{
		TCPSession::closeSession();
	}

	if (afterServerReadDataNotificationCallback)
	{
		afterServerReadDataNotificationCallback(tcpSocket, nullptr, 0, error);
	}
}

NS_END
