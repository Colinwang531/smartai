//
//		Copyright :					@2018, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-06-27
//		Description:				TCP服务端会话
//
//		History:						Author									Date													Description
//											王科威										2018-06-27									创建
//

#ifndef TCP_SERVER_SESSION_H
#define TCP_SERVER_SESSION_H

#include "boost/function.hpp"
#include "ASIO/TCPSession.h"
using TCPSession = NS(asio, 2)::TCPSession;

NS_BEGIN(session, 2)

typedef boost::function<void(boost::asio::ip::tcp::socket*, const char*, const Int32, const Int32)> AfterServerReadDataNotificationCallback;

class TCPServerSession : public TCPSession
{
public:
	TCPServerSession(
		boost::asio::ip::tcp::socket* so = nullptr, AfterServerReadDataNotificationCallback callback = nullptr);
	virtual ~TCPServerSession(void);

protected:
	void receivingData(
		const char* data = nullptr, const Int32 bytes = 0, const Int32 error = 0) override;

private:
	void afterAsyncWaitNotificationCallback(const Int32 error);

private:
	AfterServerReadDataNotificationCallback afterServerReadDataNotificationCallback;
	Int32 transferredDataBytes;
};//class TCPServerSession

NS_END

#endif//TCP_SERVER_SESSION_H
