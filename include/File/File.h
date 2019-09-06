//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-09-28
//		Description:					�ļ�������ඨ�壬�������е��ļ�ʵ�������Ա����ļ��򿪵ľ��ֵ���ļ�����
//
//		History:						Author									Date											Description
//											������									2017-09-28							����
//

#ifndef FILE_H
#define FILE_H

#include <string>
#include "predef.h"

NS_BEGIN(file, 1)

class File
{
public:
	File(void);
	virtual ~File(void);

public:
	FILE* open(const std::string path);
	void close(FILE* fd = NULL);

protected:
	virtual FILE* openFile(const std::string path);
	virtual void closeFile(FILE* fd = NULL);
};//class File

NS_END

#endif//FILE_H
