//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				实时流类
//
//		History:						Author										Date													Description
//											王科威										2019-07-22									创建
//

#ifndef LIVE_STREAM_H
#define LIVE_STREAM_H

#include "AVStream.h"

NS_BEGIN(stream, 1)

class Livestream : protected AVStream
{
public:
	Livestream(void);
	virtual ~Livestream(void);

public:
	virtual int open(
		const int userID = -1, const int streamNo = -1);
	virtual int close(void);
	//Return : Bytes of JPEG data.
	virtual int capture(
		const int userID, const int cameraIndex, char*& jpegData, const int jpegBytes = 1024 * 1024);

protected:
	virtual void captureVideoDataNotifiy(
		const unsigned char* streamData = NULL, const long dataBytes = 0, const long dataType = -1) = 0;
// 	virtual void audioDataCaptureNotifier(const char* data = NULL, const int dataBytes = 0) = 0;

protected:
	long livestreamID;
};//class Livestream

NS_END

#endif//LIVE_STREAM_H
