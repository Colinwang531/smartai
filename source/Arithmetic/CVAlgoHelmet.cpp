#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoHelmet.h"

NS_BEGIN(algo, 1)

CVAlgoHelmet::CVAlgoHelmet(
	FIFOList* fqueue /* = NULL */, CVAlgoDetectNotifyHandler handler /* = NULL */)
	: CVAlgo(fqueue, handler)
{}

CVAlgoHelmet::~CVAlgoHelmet()
{}

bool CVAlgoHelmet::initializeWithParameter(void* parameter /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };

	if (initParames)
	{
		status = helmet.InitAlgoriParam(imageWidth, imageHeight, channelNumber, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void CVAlgoHelmet::mainWorkerProcess()
{
	while (1)
	{
		BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(frameQueue->getFront()) };

		if (frame)
		{
			frameQueue->popFront();
			FeedBackHelmet feedback;
			if (helmet.MainProcFunc((unsigned char*)frame->frameData, feedback))
			{
				DetectNotify dectectNotify;
				std::vector<DetectNotify> detectNotifies;

				for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
				{
					dectectNotify.type = ALGO_HELMET;
					dectectNotify.x = feedback.vecShowInfo[i].rRect.x;
					dectectNotify.y = feedback.vecShowInfo[i].rRect.y;
					dectectNotify.w = feedback.vecShowInfo[i].rRect.width;
					dectectNotify.h = feedback.vecShowInfo[i].rRect.height;
					dectectNotify.status = feedback.vecShowInfo[i].nLabel;
					detectNotifies.push_back(dectectNotify);
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