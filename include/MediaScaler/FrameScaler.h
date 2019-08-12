//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					帧数据格式转换抽象基类，所有转换器都可以继承于此
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
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
