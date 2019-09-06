//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				串口通信基类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef COM_PORT_H
#define COM_PORT_H  

#include "boost/asio.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "predef.h"

NS_BEGIN(com, 1)

typedef boost::function<void(const char*, const int)> AsyncReadDataNotifyHandler;

class ComPort
{
public:
	ComPort(AsyncReadDataNotifyHandler handler = NULL);
	~ComPort(void);

public:
	int initPort(const char portNumber = 0, const unsigned int baudrate = 9600);

private:
	void asyncReadDataHandler(
		const boost::system::error_code err, const char* data = NULL, const int dataBytes = 0);
	static unsigned int __stdcall ioWorkerThread(void* ctx = NULL);

private:
	boost::asio::io_service ios;
	boost::shared_ptr<boost::asio::serial_port> comPortPtr;
	AsyncReadDataNotifyHandler asyncReadDataNotifyHandler;
	BOOST_STATIC_CONSTANT(int, recvDataBytes = 1);
	char recvDataBuffer[recvDataBytes];
};//class ComPort

NS_END

#endif//COM_PORT_H 