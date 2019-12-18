#include "Hardware/Cpu.h"
using CPU = framework::hardware::Cpu;
#include "Network/Asio/Service.h"

namespace framework
{
	namespace network
	{
		ASIOService::ASIOService() : idleIndex{ 0 }
		{}

		ASIOService::~ASIOService()
		{}

		int ASIOService::startService(const unsigned char threadNumber /* = 1 */)
		{
			const unsigned char totalThreadNumber = 0 < threadNumber ? threadNumber : 1;
			IOContexts ioctxs{ totalThreadNumber };

			for (int i = 0; i != totalThreadNumber; ++i)
			{
				boost::thread* thread{
					threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, boost::ref(ioctxs[i]))) };
				CPU().setThreadAffinityMask(thread->native_handle(), i);
				thread->detach();
				works.push_back(boost::asio::io_service::work(ioctxs[i]));
			}

			ctxs.swap(ioctxs);
			return totalThreadNumber;
		}

		void ASIOService::stopService()
		{
			std::for_each(ctxs.begin(), ctxs.end(), boost::bind(&boost::asio::io_service::stop, _1));
			threadGroup.join_all();
			works.clear();
			ctxs.clear();
		}

		boost::asio::io_service& ASIOService::getIdleIO()
		{
			WriteLock wl(mtx);
			return ctxs.at(idleIndex++ % ctxs.size());
		}
	}//namespace network
}//namespace framework
