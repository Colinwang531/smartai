//
//		Copyright :				@2018, HHJT, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@hotmail.com
//		Date :							2018-05-23
//		Description:				XML
//
//		History:						Author									Date											Description
//											������									2018-05-23									����
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

	//	���� :				����XML�ַ���
	//
	//	���� :				content													[in]																	XML�ַ���
	//
	//	����ֵ :			������
	//
	//	��ע :
	//
	Int32 Parse(const std::string& content);

	//	���� :				��ȡ����ڵ�
	//
	//	���� :
	//
	//	����ֵ :			�ڵ�
	//
	//	��ע :
	//
 	const TiXmlElement* getRootElement(void);
 	const TiXmlElement* getFirstChildElement(const TiXmlElement* element = nullptr);
	const TiXmlElement* getNextSiblingElement(const TiXmlElement* element = nullptr);

	//	���� :				��ȡ�ڵ����ƻ�ֵ
	//
	//	���� :
	//
	//	����ֵ :			�ڵ����ƻ�ֵ
	//
	//	��ע :
	//
 	std::string getElementName(const TiXmlElement* element = nullptr);
	std::string getElementValue(const TiXmlElement* element = nullptr);

private:
	TiXmlDocument document;
	TiXmlElement* rootElement;
};//class TinyXml

NS_END

#endif//XML_H