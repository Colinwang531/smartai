#ifndef ALARM_MESSAGE_H
#define ALARM_MESSAGE_H

#include <vector>
#include "predef.h"

typedef struct tagAlarmInfo_t
{
	int type : 32;
    int x : 32;
    int y : 32;
    int w : 32;
    int h :32;
	int label : 32;
}AlarmInfo;

// typedef enum tagAlarmType_t
// {
// 	HELMET = 0,
// 	PHONE,
// 	SLEEP
// }AlarmType;

class AlarmMessage
{
public:
    AlarmMessage(const int messageBytes = 10 * 1024 * 1024);
    ~AlarmMessage(void);

    int setMessageData(
        const int type, const int w, const int h, const char* name, 
		const std::vector<AlarmInfo> alarmInfos, const char* jpeg, const int jpegBytes);
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
};//class AlarmMessage

#endif//ALARM_MESSAGE_H