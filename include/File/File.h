//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-09-28
//		Description:					文件抽象基类定义，并非所有的文件实例都可以保存文件打开的句柄值和文件内容
//
//		History:						Author									Date											Description
//											王科威									2017-09-28							创建
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
