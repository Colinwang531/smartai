#include "zmq.h"
#include "error.h"
#include "Zmq/Context.h"

namespace framework
{
	namespace network
	{
		ZMQContext::ZMQContext() : ctx{ NULL }
		{}

		ZMQContext::~ZMQContext()
		{}

		int ZMQContext::createNewContext(const unsigned char threadNumber /*= 1*/)
		{
			int status{ 0 < threadNumber ? ERR_OK : ERR_INVALID_PARAM };

			if (!ctx)
			{
				ctx = zmq_ctx_new();
				status = zmq_ctx_set(ctx, ZMQ_IO_THREADS, threadNumber) ? ERR_BAD_OPERATE : ERR_OK;
			}
			else
			{
				status = ERR_EXISTED;
			}

			return status;
		}

		int ZMQContext::destroyContext(void)
		{
			int status{ ERR_BAD_OPERATE };

			if (ctx)
			{
				status = zmq_ctx_destroy(ctx) ? ERR_BAD_OPERATE : ERR_OK;
			}

			return status;
		}

		void* ZMQContext::createNewSocket(const SocketType type /* = SocketType::SOCKET_TYPE_NONE */)
		{
			void* s{ NULL };

			if (ctx && SocketType::SOCKET_TYPE_NONE != type)
			{
				s = zmq_socket(ctx, static_cast<int>(type));
			}

			return s;
		}

		int ZMQContext::closesocket(void* s /* = NULL */)
		{
			return zmq_close(s) ? ERR_BAD_OPERATE : ERR_OK;
		}
	}//namespace network
}//namespace framework
