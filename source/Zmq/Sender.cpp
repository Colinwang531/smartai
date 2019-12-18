#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQSender.h"

NS_BEGIN(mq, 1)

MQSender::MQSender()
{}

MQSender::~MQSender()
{}

int MQSender::send(const char* data /* = NULL */, const int dataBytes /* = 0 */, void* so /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (data && 0 < dataBytes && so)
	{
		//Split
		int totalDataBytes{ dataBytes }, sentBytes{ 0 };

		while (0 < totalDataBytes)
		{
			const int tempDataBytes{ totalDataBytes > maxTransportMTUBytes ? (int)maxTransportMTUBytes : totalDataBytes };
			const bool tempDataMore{ totalDataBytes > maxTransportMTUBytes ? true : false };

			if (ERR_OK == sendData(data + sentBytes, tempDataBytes, so, tempDataMore))
			{
				sentBytes += tempDataBytes;
				totalDataBytes -= tempDataBytes;
			}
		}

		status = ERR_OK;
	}

	return status;
}

int MQSender::sendData(
	const char* data /* = NULL */, const int dataBytes /* = 0 */, void* so /* = NULL */, const bool more /* = false */)
{
	int sentBytes{ 0 }, sendMore = { more ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT };
	zmq_msg_t sentMessage;

	if (!zmq_msg_init_size(&sentMessage, dataBytes))
	{
#ifdef _WINDOWS
		memcpy_s(zmq_msg_data(&sentMessage), dataBytes, data, dataBytes);
#else
		memcpy(zmq_msg_data(&sentMessage), data, dataBytes);
#endif

		sentBytes = zmq_msg_send(&sentMessage, so, sendMore);
		zmq_msg_close(&sentMessage);
	}

	return 0 < sentBytes ? ERR_OK : ERR_SEND_FAILED;
}

NS_END
