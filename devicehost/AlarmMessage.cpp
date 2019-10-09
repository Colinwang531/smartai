#include <exception>
#include <string>
#include"error.h"
#include "Time/Time.h"
using Time = NS(time, 1)::Time;
#include "AlarmMessage.h"

extern long long clockUTCTime;
long long AlarmMessage::sequenceNumber = 0;

AlarmMessage::AlarmMessage(const int messageBytes/* = 10 * 1024 * 1024 */) : message{NULL}, messageBytes{0}
{
    message = new(std::nothrow) char[messageBytes];

	if (message)
	{
		memset(message, 0, messageBytes);
	}
}

AlarmMessage::~AlarmMessage()
{
    if(message)
    {
        delete[] message;
    }
}

int AlarmMessage::setMessageData(
	const int type, const int w, const int h, 
	const char* NVRIp, const int channelIndex, 
	const std::vector<NS(algo, 1)::AlarmInfo> alarmInfos,
	const char* jpeg, const int jpegBytes)
{
    int ret{ERR_BAD_OPERATE};

	if ((int)NS(algo, 1)::AlarmType::ALARM_TYPE_FACE == type)
	{
		return setFaceMessageData(type, w, h, NVRIp, channelIndex, alarmInfos, jpeg, jpegBytes);
	}

    if(message)
    {
		++sequenceNumber;
		char* messageData{ (char*)message };
		int pos = 0;

		long long* sequenceNo{ (long long*)messageData };
		*sequenceNo = sequenceNumber;
		pos += 8;
		int* messageID{ (int*)(messageData + pos) };
		*messageID = 0;
		pos += 8;
		*((int*)(messageData + pos)) = w;
		pos += 4;
		*((int*)(messageData + pos)) = h;
		pos += 4;
		//Local time
		*((long long*)(messageData + pos)) = Time().expiredMilliseconds();
		pos += 8;
		int iplen{ (int)strlen(NVRIp) };
		*((int*)(messageData + pos)) = iplen;
		pos += 4;
		memcpy_s(messageData + pos, iplen, NVRIp, iplen);
		pos += iplen;
		*((int*)(messageData + pos)) = channelIndex;
		pos += 4;
		const int rectNum{ (int)(alarmInfos.size()) };
		memcpy_s(messageData + pos, 4, &rectNum, 4);
		pos += 4;

		for (int i = 0; i != alarmInfos.size(); ++i)
		{
			int type{ (int)alarmInfos[i].type }, x{ alarmInfos[i].x }, y{ alarmInfos[i].y }, w{ alarmInfos[i].w }, h{ alarmInfos[i].h }, status{ alarmInfos[i].status };
			memcpy_s(messageData + pos, 4, &type, 4);
			memcpy_s(messageData + pos + 4, 4, &x, 4);
			memcpy_s(messageData + pos + 8, 4, &y, 4);
			memcpy_s(messageData + pos + 12, 4, &w, 4);
			memcpy_s(messageData + pos + 16, 4, &h, 4);
			memcpy_s(messageData + pos + 20, 4, &status, 4);
			pos += 24;
		}
		memcpy_s(messageData + pos, 4, &jpegBytes, 4);
		pos += 4;
        memcpy_s(messageData + pos, jpegBytes, jpeg, jpegBytes);
		pos += jpegBytes;
		int packageBytes{ pos - 16 };
		memcpy_s(messageData + 12, 4, &packageBytes, 4);

        messageBytes = pos;
		ret = ERR_OK;
    }

    return ret;
}

int AlarmMessage::setFaceMessageData(
	const int type, const int w, const int h,
	const char* NVRIp, const int channelIndex,
	const std::vector<NS(algo, 1)::AlarmInfo> alarmInfos,
	const char* jpeg, const int jpegBytes)
{
	int ret{ ERR_BAD_OPERATE };

	if (message)
	{
		++sequenceNumber;
		char* messageData{ (char*)message };
		int pos = 0;

		long long* sequenceNo{ (long long*)messageData };
		*sequenceNo = sequenceNumber;
		pos += 8;
		int* messageID{ (int*)(messageData + pos) };
		*messageID = 18;
		pos += 8;
		int iplen{ (int)strlen(NVRIp) };
		*((int*)(messageData + pos)) = iplen;
		pos += 4;
		memcpy_s(messageData + pos, iplen, NVRIp, iplen);
		pos += iplen;
		*((int*)(messageData + pos)) = channelIndex;
		pos += 4;
		//Local time
		*((long long*)(messageData + pos)) = Time().expiredMilliseconds();
		pos += 8;
		//UTC Time
		*((long long*)(messageData + pos)) = clockUTCTime;
		pos += 8;
		*((int*)(messageData + pos)) = jpegBytes;
		pos += 4;
		memcpy_s(messageData + pos, jpegBytes, jpeg, jpegBytes);
		pos += jpegBytes;
		const int faceNum{ (int)(alarmInfos.size()) };
		memcpy_s(messageData + pos, 4, &faceNum, 4);
		pos += 4;

		for (int i = 0; i != alarmInfos.size(); ++i)
		{
			long long faceid{ alarmInfos[i].face.faceID };
			memcpy_s(messageData + pos, 8, &faceid, 8);
			memcpy_s(messageData + pos + 8, 4, &alarmInfos[i].face.imageBytes, 4);
			memcpy_s(messageData + pos + 12, alarmInfos[i].face.imageBytes, alarmInfos[i].face.imageData, alarmInfos[i].face.imageBytes);
			pos += (12 + alarmInfos[i].face.imageBytes);
		}

		int packageBytes{ pos - 16 };
		memcpy_s(messageData + 12, 4, &packageBytes, 4);

		messageBytes = pos;
		ret = ERR_OK;
	}

	return ret;
}
