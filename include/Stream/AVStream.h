//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				��������࣬��������ͨ��������ֱ�Ӽ̳��ڴ�
//
//		History:						Author										Date													Description
//											������										2019-07-22									����
//

#ifndef AV_STREAM_H
#define AV_STREAM_H

#include "predef.h"

NS_BEGIN(stream, 1)

class AVStream
{
public:
	AVStream(const unsigned short idx = -1);
	virtual ~AVStream(void);

public:
	virtual int openStream(void) = 0;
	virtual int closeStream(void) = 0;
	//Return : bytes of picture captured.
	virtual unsigned long long capturePicture(
		const char* data = NULL, const unsigned long long dataBytes = 0) = 0;

protected:
	const unsigned short streamIndex;
};//class AVStream

NS_END

#endif//AV_STREAM_H
