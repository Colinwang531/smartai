//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					��ý������ͼ�������
//
//		History:						Author									Date														Description
//											������									2019-10-13										����
//

#ifndef MEDIA_GRAPH_H
#define MEDIA_GRAPH_H

#include "boost/shared_ptr.hpp"
#include "MediaFilter/MediaFilter.h"
using MediaFilterPtr = boost::shared_ptr<NS(filter, 1)::MediaFilter>;

NS_BEGIN(graph, 1)

class MediaGraph
{
public:
	MediaGraph(void);
	virtual ~MediaGraph(void);

public:
	virtual int addFilter(
		const std::string filterID, MediaFilterPtr filterPtr);
	virtual int removeFilter(const std::string filterID);
	virtual int inputData(MediaFilterPtr filterPtr);
};//class MediaGraph

NS_END

#endif//MEDIA_GRAPH_H
