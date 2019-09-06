//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2018-02-08
//		Description:					使用INI格式的配置文件类定义
//
//		History:						Author										Date													Description
//											王科威										2018-02-08									创建
//

#ifndef INI_H
#define INI_H

#include "boost/property_tree/ptree.hpp"
using PTREE = boost::property_tree::ptree;
#include "File/File.h"
#include "File/Reading.h"

NS_BEGIN(file, 1)

class Ini : public File, public Reading
{
public:
	Ini(void);
	virtual ~Ini(void);

public:
	//There is nothing should be returned by using openFile method.
	FILE* openFile(const std::string path) override;
	void closeFile(FILE* fd = NULL) override;
	Int32 readingIntValueByKey(const std::string key) override;
	std::string readingStringValueByKey(const std::string key) override;

protected:
	virtual Int32 readingIntValueByNodeAndKey(PTREE node, const std::string key) = 0;
	virtual std::string readingStringValueByNodeAndKey(PTREE node, const std::string key) = 0;

private:
	PTREE rootNode;
};//class Ini

NS_END

#endif//INI_H
