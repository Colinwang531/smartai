//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-22
//		Description:					�ļ�д����������ඨ��
//
//		History:						Author									Date											Description
//											������									2019-07-22							����
//

#ifndef WRITTING_H
#define WRITTING_H

#include <string>
#include "predef.h"

NS_BEGIN(file, 1)

class Writting
{
public:
	Writting(void) {}
	virtual ~Writting(void) {}

public:
	virtual Int32 writtingWithLevel(const Int32 level, const std::string format, ...) = 0;
};//class Writting

NS_END

#endif//WRITTING_H
