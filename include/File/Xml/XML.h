//
//		Copyright :				@2018, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2018-05-23
//		Description:				XML
//
//		History:						Author									Date											Description
//											王科威									2018-05-23									创建
//

#ifndef XML_H
#define XML_H

#include "tinyxml/tinyxml.h"
#include "Defs.h"

NS_BEGIN(xml, 1)

class XML
{
public:
	XML(void);
	~XML(void);

	//	功能 :				解析XML字符串
	//
	//	参数 :				content													[in]																	XML字符串
	//
	//	返回值 :			错误码
	//
	//	备注 :
	//
	Int32 Parse(const std::string& content);

	//	功能 :				获取各类节点
	//
	//	参数 :
	//
	//	返回值 :			节点
	//
	//	备注 :
	//
 	const TiXmlElement* getRootElement(void);
 	const TiXmlElement* getFirstChildElement(const TiXmlElement* element = nullptr);
	const TiXmlElement* getNextSiblingElement(const TiXmlElement* element = nullptr);

	//	功能 :				获取节点名称或值
	//
	//	参数 :
	//
	//	返回值 :			节点名称或值
	//
	//	备注 :
	//
 	std::string getElementName(const TiXmlElement* element = nullptr);
	std::string getElementValue(const TiXmlElement* element = nullptr);

private:
	TiXmlDocument document;
	TiXmlElement* rootElement;
};//class TinyXml

NS_END

#endif//XML_H