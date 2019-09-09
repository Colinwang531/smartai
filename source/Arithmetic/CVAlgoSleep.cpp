#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoSleep.h"

NS_BEGIN(algo, 1)

CVAlgoSleep::CVAlgoSleep(
	FIFOList* fqueue /* = NULL */, CVAlgoDetectNotifyHandler handler /* = NULL */)
	: CVAlgo(fqueue, handler)
{}

CVAlgoSleep::~CVAlgoSleep()
{}

bool CVAlgoSleep::initializeWithParameter(void* parameter /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };

	if (initParames)
	{
		status = sleep.InitAlgoriParam(imageWidth, imageHeight, channelNumber, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void CVAlgoSleep::mainWorkerProcess()
{
	while (1)
	{
		BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(frameQueue->getFront()) };

		if (frame)
		{
			frameQueue->popFront();
			FeedBackSleep feedback;
			if (sleep.MainProcFunc((unsigned char*)frame->frameData, feedback))
			{
				DetectNotify detectNotify;
				std::vector<DetectNotify> detectNotifies;

				for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
				{
					detectNotify.type = ALGO_SLEEP;
					detectNotify.x = feedback.vecShowInfo[i].rRect.x;
					detectNotify.y = feedback.vecShowInfo[i].rRect.y;
					detectNotify.w = feedback.vecShowInfo[i].rRect.width;
					detectNotify.h = feedback.vecShowInfo[i].rRect.height;
					detectNotify.status = feedback.vecShowInfo[i].nLabel;
					detectNotifies.push_back(detectNotify);
				}

				if (0 < detectNotifies.size() && cvAlgoDetectNotifyHandler)
				{
					cvAlgoDetectNotifyHandler(frame, detectNotifies);
				}
			}

			boost::checked_array_delete(frame->frameData);
			boost::checked_array_delete(frame->jpegData);
			boost::checked_array_delete(frame->NVRIp);
			boost::checked_delete(frame);
		}
	}
}

NS_END