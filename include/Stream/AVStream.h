//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				流抽象基类，所有物理通道都可以直接继承于此
//
//		History:						Author										Date													Description
//											王科威										2019-07-22									创建
//

#ifndef AV_STREAM_H
#define AV_STREAM_H

#include "predef.h"

NS_BEGIN(stream, 1)

class AVStream
{
public:
	AVStream(void);
	virtual ~AVStream(void);
};//class AVStream

NS_END

#endif//AV_STREAM_H
