//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-22
//		Description:					�ļ�������������ඨ��
//
//		History:						Author									Date											Description
//											������									2019-07-22							����
//

#ifndef READING_H
#define READING_H

#include <string>
#include "predef.h"

NS_BEGIN(file, 1)

class Reading
{
public:
	Reading(void) {}
	virtual ~Reading(void){}

public:
	virtual Int32 readingIntValueByKey(const std::string key) = 0;
	virtual std::string readingStringValueByKey(const std::string key) = 0;
};//class Reading

NS_END

#endif//READING_H
