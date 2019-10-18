#ifndef ALARM_MESSAGE_H
#define ALARM_MESSAGE_H

#include <vector>
#include "Arithmetic/CVAlgo.h"

class AlarmMessage
{
public:
    AlarmMessage(const int messageBytes = 10 * 1024 * 1024);
    ~AlarmMessage(void);

public:
    int setAlarmMessageData(
        const int type, const int w, const int h,
		const char* NVRIp, const int channelIndex,
		const std::vector<NS(algo, 1)::AlarmInfo> alarmInfos, 
		const char* jpeg, const int jpegBytes);
	int setFaceMessageData(
		const int type, const int w, const int h,
		const char* NVRIp, const int channelIndex,
		const std::vector<NS(algo, 1)::FaceInfo> faceInfos,
		const char* jpeg, const int jpegBytes);
    inline char* getMessageData(void) const
    {
        return message;
    }
    inline int getMessageBytes(void) const
    {
        return messageBytes;
    }

private:
    char* message;
    int messageBytes;
	static long long sequenceNumber;
};//class AlarmMessage

#endif//ALARM_MESSAGE_H