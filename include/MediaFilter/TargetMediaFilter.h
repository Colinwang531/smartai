//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					多媒体目的过滤器类
//
//		History:						Author									Date														Description
//											王科威									2019-10-13										创建
//

#ifndef TARGET_MEDIA_FILTER_H
#define TARGET_MEDIA_FILTER_H

#include "MediaFilter.h"

NS_BEGIN(filter, 1)

class TargetMediaFilter : public MediaFilter
{
public:
	TargetMediaFilter(void);
	virtual ~TargetMediaFilter(void);

public:
	bool isTargetFilter(void) const override;
};//class TargetMediaFilter

NS_END

#endif//TARGET_MEDIA_FILTER_H
