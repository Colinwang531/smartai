#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "File/Com/ComPort.h"  

NS_BEGIN(com, 1)

ComPort::ComPort(AsyncReadDataNotifyHandler handler /* = NULL */) : asyncReadDataNotifyHandler{ handler }
{}
 
ComPort::~ComPort(void)
{
	if (!ios.stopped())
	{
		ios.stop();
	}
}

int ComPort::initPort(const char portNumber /* = 0 */, const unsigned int baudrate /* = 9600 */)
{
	int status{ ERR_BAD_OPERATE };

	if (!comPortPtr)
	{
		boost::shared_ptr<boost::asio::serial_port> serialPortPtr{ 
			boost::make_shared<boost::asio::serial_port>(ios) };
		if (serialPortPtr)
		{
			boost::system::error_code err;
			const std::string portDescription{ (boost::format("COM%d") % (int)portNumber).str() };
			serialPortPtr->open(portDescription, err);
			if (!err)
			{
				serialPortPtr->set_option(boost::asio::serial_port::baud_rate(baudrate), err);
				serialPortPtr->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none), err);
				serialPortPtr->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none), err);
				serialPortPtr->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one), err);
				serialPortPtr->set_option(boost::asio::serial_port::character_size(8), err);

				serialPortPtr->async_read_some(
					boost::asio::buffer(recvDataBuffer, recvDataBytes),
					boost::bind(
						&ComPort::asyncReadDataHandler,
						this,
						boost::asio::placeholders::error,
						recvDataBuffer,
						boost::asio::placeholders::bytes_transferred));

				comPortPtr.swap(serialPortPtr);
				status = ERR_OK;
				HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &ioWorkerThread, this, 0, NULL);
				SetThreadPriority(handle, THREAD_PRIORITY_NORMAL);
			}
		}
	}

	return status;
}

void ComPort::asyncReadDataHandler(
	const boost::system::error_code err, const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	if (!err && asyncReadDataNotifyHandler)
	{
		asyncReadDataNotifyHandler(data, dataBytes);
	}

	if (!err && comPortPtr)
	{
		comPortPtr->async_read_some(
			boost::asio::buffer(recvDataBuffer, recvDataBytes),
			boost::bind(
				&ComPort::asyncReadDataHandler,
				this,
				boost::asio::placeholders::error,
				recvDataBuffer,
				boost::asio::placeholders::bytes_transferred));
	}
}

unsigned int ComPort::ioWorkerThread(void* ctx /* = NULL */)
{
	ComPort* comPort{ reinterpret_cast<ComPort*>(ctx) };

	if (comPort)
	{
		comPort->ios.run();
	}

	return 0;
}

NS_END
