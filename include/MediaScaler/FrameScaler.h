//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					֡���ݸ�ʽת��������࣬����ת���������Լ̳��ڴ�
//
//		History:						Author									Date														Description
//											������									2019-07-25										����
//

#ifndef FRAME_SCALER_H
#define FRAME_SCALER_H

#include "predef.h"

NS_BEGIN(scaler, 1)

class FrameScaler
{
public:
	FrameScaler(void);
	virtual ~FrameScaler(void);

public:
	virtual const char* scale(
		const char* data = NULL, const int dataBytes = 0, const int w = 0, const int h = 0);
};//class FrameScaler

NS_END

#endif//FRAME_SCALER_H
