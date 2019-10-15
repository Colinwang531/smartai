//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					��ý��Ŀ�Ĺ�������
//
//		History:						Author									Date														Description
//											������									2019-10-13										����
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
