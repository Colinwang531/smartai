//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				ʵʱ����
//
//		History:						Author										Date													Description
//											������										2019-07-22									����
//

#ifndef LIVE_STREAM_H
#define LIVE_STREAM_H

#include "AVStream.h"
#include "EnablePictureCapture.h"

NS_BEGIN(stream, 1)

class Livestream : public AVStream, protected EnablePictureCapture
{
public:
	Livestream(const unsigned short idx = -1);
	virtual ~Livestream(void);

protected:
	virtual void captureVideoStreamProcess(
		const unsigned char* data = NULL, const long long dataBytes = 0);
 	virtual void captureAudioStreamProcess(
		const unsigned char* data = NULL, const long long dataBytes = 0);

protected:
	long streamID;
};//class Livestream

NS_END

#endif//LIVE_STREAM_H
