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
	const NS(algo, 1)::AlarmInfo alarmInfo, const unsigned char* bgr24Image, const int bgr24ImageBytes)
{
	AlarmInfo info;
	info.type = (AlarmType)alarmInfo.type;
	info.x = alarmInfo.x;
	info.y = alarmInfo.y;
	info.w = alarmInfo.w;
	info.h = alarmInfo.h;
	info.status = alarmInfo.status;
	info.bgr24 = bgr24Image;
	info.bgr24Bytes = bgr24ImageBytes;
	std::pair<ARITHMETIC_AlarmInfoNotifyCallback, void*> callback{ alarmInfoNotifyCallbackGroup.at(info.type) };
	if (callback.first)
	{
		callback.first(info, callback.second);
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
