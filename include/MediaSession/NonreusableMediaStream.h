//
//		Copyright :					@2018, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-06-27
//		Description:					不可复用流
//
//		History:						Author									Date													Description
//											王科威									2018-06-27									创建
//

#ifndef NONREUSABLE_MEDIA_STREAM_H
#define NONREUSABLE_MEDIA_STREAM_H

#include "boost/shared_ptr.hpp"
//#include "MediaStream.h"
#include "Defs.h"

NS_BEGIN(media, 2)

class NonreusableMediaStream// : protected MediaStream
{
//	using TargetMediaStream = boost::shared_ptr<MediaStream>;

public:
	NonreusableMediaStream(void);
	virtual ~NonreusableMediaStream(void);

protected:
	//唯一的目的端
//	TargetMediaStream targetMediaStream;
};//class NonreusableMediaStream

NS_END

#endif//NONREUSABLE_MEDIA_STREAM_H
