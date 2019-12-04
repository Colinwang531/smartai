#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "DataStruct/UnorderedMap.h"
#include "Arithmetic/CVAlgoFace.h"
using CVAlgoFace = framework::arithmetic::CVAlgoFace;
#include "Arithmetic/CVAlgoFight.h"
using CVAlgoFight = framework::arithmetic::CVAlgoFight;
#include "Arithmetic/CVAlgoHelmet.h"
using CVAlgoHelmet = framework::arithmetic::CVAlgoHelmet;
#include "Arithmetic/CVAlgoPhone.h"
using CVAlgoPhone = framework::arithmetic::CVAlgoPhone;
#include "Arithmetic/CVAlgoSleep.h"
using CVAlgoSleep = framework::arithmetic::CVAlgoSleep;
#include "Arithmetic.h"

using ArithmeticPtr = boost::shared_ptr<framework::arithmetic::CVAlgo>;
UnorderedMap<const AlarmType, ArithmeticPtr> arithmeticGroup;
UnorderedMap<const AlarmType, std::pair<ARITHMETIC_AlarmInfoNotifyCallback, void*>> alarmInfoNotifyCallbackGroup;

static void postDetectAlarmInfoCallback(
	const framework::arithmetic::AlarmInfo alarmInfo, const unsigned char* bgr24Image, const int bgr24ImageBytes)
{
	AlarmInfo info;
	info.type = (AlarmType)alarmInfo.type;
	info.x = alarmInfo.x;
	info.y = alarmInfo.y;
	info.w = alarmInfo.w;
	info.h = alarmInfo.h;
	info.status = alarmInfo.status;
	info.faceID = alarmInfo.faceID;
	info.similarity = alarmInfo.similarity;
	std::pair<ARITHMETIC_AlarmInfoNotifyCallback, void*> callback{ alarmInfoNotifyCallbackGroup.at(info.type) };
	if (callback.first)
	{
		callback.first(info, bgr24Image, bgr24ImageBytes, callback.second);
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
