//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					多媒体源过滤器类
//
//		History:						Author									Date														Description
//											王科威									2019-10-13										创建
//

#ifndef SOURCE_MEDIA_FILTER_H
#define SOURCE_MEDIA_FILTER_H

#include "MediaFilter.h"

NS_BEGIN(filter, 1)

class SourceMediaFilter : public MediaFilter
{
public:
	SourceMediaFilter(void);
	virtual ~SourceMediaFilter(void);

public:
	bool isSourceFilter(void) const override;
};//class SourceMediaFilter

NS_END

#endif//SOURCE_MEDIA_FILTER_H
