#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "DataStruct/UnorderedMap.h"
#include "Arithmetic/CVAlgoFace.h"
using CVAlgoFace = NS(algo, 1)::CVAlgoFace;
#include "Arithmetic/CVAlgoFight.h"
using CVAlgoFight = NS(algo, 1)::CVAlgoFight;
#include "Arithmetic/CVAlgoHelmet.h"
using CVAlgoHelmet = NS(algo, 1)::CVAlgoHelmet;
#include "Arithmetic/CVAlgoPhone.h"
using CVAlgoPhone = NS(algo, 1)::CVAlgoPhone;
#include "Arithmetic/CVAlgoSleep.h"
using CVAlgoSleep = NS(algo, 1)::CVAlgoSleep;
#include "Arithmetic.h"

using ArithmeticPtr = boost::shared_ptr<NS(algo, 1)::CVAlgo>;
UnorderedMap<const AlarmType, ArithmeticPtr> arithmeticGroup;
UnorderedMap<const AlarmType, std::pair<ARITHMETIC_AlarmInfoNotifyCallback, void*>> alarmInfoNotifyCallbackGroup;

static void postDetectAlarmInfoCallback(
	const std::vector<NS(algo, 1)::AlarmInfo> alarmInfo, const unsigned char* bgr24Image, const unsigned long long bgr24ImageBytes)
{
	AlarmInfo alarmInfoCallback[64];
	unsigned int alarmInfoCount{ static_cast<unsigned int>(alarmInfo.size()) };

	for (int i = 0; i != alarmInfoCount; ++i)
	{
		alarmInfoCallback[i].type = (AlarmType)alarmInfo[i].type;
		alarmInfoCallback[i].x = alarmInfo[i].x;
		alarmInfoCallback[i].y = alarmInfo[i].y;
		alarmInfoCallback[i].w = alarmInfo[i].w;
		alarmInfoCallback[i].h = alarmInfo[i].h;
		alarmInfoCallback[i].status = alarmInfo[i].status;
		alarmInfoCallback[i].faceID = alarmInfo[i].faceID;
		alarmInfoCallback[i].trackID = alarmInfo[i].trackID;
		alarmInfoCallback[i].similarity = alarmInfo[i].similarity;
		alarmInfoCallback[i].faceImage = alarmInfo[i].faceImage;
		alarmInfoCallback[i].faceImageBytes = alarmInfo[i].imageBytes;
	}

	std::pair<ARITHMETIC_AlarmInfoNotifyCallback, void*> callback{ alarmInfoNotifyCallbackGroup.at(alarmInfoCallback[0].type) };
	if (callback.first)
	{
		callback.first(alarmInfoCallback, alarmInfoCount, bgr24Image, bgr24ImageBytes, callback.second);
	}
}

int ARITHMETIC_RegisterAlarmNotifyCallback(const AlarmType alarmType , ARITHMETIC_AlarmInfoNotifyCallback callback, void*userData)
{
	ArithmeticPtr arithmeticPtr;
	int status{ AlarmType::ALARM_TYPE_NONE == alarmType ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		if (AlarmType::ALARM_TYPE_FACE == alarmType)
		{
			arithmeticPtr = boost::make_shared<CVAlgoFace>();
		}
		else if (AlarmType::ALARM_TYPE_FIGHT == alarmType)
		{
			arithmeticPtr = boost::make_shared<CVAlgoFight>();
		}
		else if (AlarmType::ALARM_TYPE_HELMET == alarmType)
		{
			arithmeticPtr = boost::make_shared<CVAlgoHelmet>();
		} 
		else if (AlarmType::ALARM_TYPE_PHONE == alarmType)
		{
			arithmeticPtr = boost::make_shared<CVAlgoPhone>();
		} 
		else if (AlarmType::ALARM_TYPE_SLEEP == alarmType)
		{
			arithmeticPtr = boost::make_shared<CVAlgoSleep>();
		}

		if (arithmeticPtr)
		{
			status = arithmeticPtr->initialize();
			if (ERR_OK == status)
			{
				arithmeticPtr->setPostDetectAlarmInfoCallback(boost::bind(&postDetectAlarmInfoCallback, _1, _2, _3));
				arithmeticGroup.insert(alarmType, arithmeticPtr);
				alarmInfoNotifyCallbackGroup.insert(alarmType, std::make_pair(callback, userData));
			}
		}
	}

	return ERR_OK == status ? 1 : 0;
}

int ARITHMETIC_AddFaceImage(const char* filePath /*= NULL*/, const int faceID /*= 0*/)
{
	int status{ ERR_NOT_SUPPORT };
	ArithmeticPtr faceArithmeticPtr{ arithmeticGroup.at(AlarmType::ALARM_TYPE_FACE) };

	if (faceArithmeticPtr)
	{
		boost::shared_ptr<CVAlgoFace> facePtr{ boost::dynamic_pointer_cast<CVAlgoFace>(faceArithmeticPtr) };
		status = facePtr->addFacePicture(filePath, faceID);
	}

	return status;
}

int ARITHMETIC_SetSleepTimeInterval(const int interval /* = 15 */)
{
	int status{ ERR_NOT_SUPPORT };
	ArithmeticPtr sleepArithmeticPtr{ arithmeticGroup.at(AlarmType::ALARM_TYPE_SLEEP) };

	if (sleepArithmeticPtr)
	{
		boost::shared_ptr<CVAlgoSleep> sleepPtr{ boost::dynamic_pointer_cast<CVAlgoSleep>(sleepArithmeticPtr) };
		status = sleepPtr->setTimeSleepInterval(interval) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}
 
int ARITHMETIC_InputImageData(
	const AlarmType alarmType /* = AlarmType::ALARM_TYPE_NONE */, const unsigned char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	int status{ AlarmType::ALARM_TYPE_NONE == alarmType || !data || !dataBytes ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		ArithmeticPtr arithmeticPtr{ arithmeticGroup.at(alarmType) };
		if (arithmeticPtr)
		{
			status = arithmeticPtr->inputImageData(data, dataBytes);
		}
	}

	return ERR_OK == status ? 1 : 0;
}
