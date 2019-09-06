#include "boost/locale.hpp"
#include "File/XML.h"

NS_BEGIN(xml, 1)

XML::XML() : rootElement{ nullptr }
{}

XML::~XML()
{}

Int32 XML::Parse(const std::string& content)
{
	std::string utf8 = boost::locale::conv::to_utf<char>(content.c_str(), "UTF-8");
	if (document.Parse(utf8.c_str()))
	{
		rootElement = document.RootElement();
	}

	return rootElement ? ERR_OK : ERR_PARSE_FAILED;
}

const TiXmlElement* XML::getRootElement(void)
{
	return rootElement;
}

const TiXmlElement* XML::getFirstChildElement(const TiXmlElement* element /*= nullptr*/)
{
	return element ? element->FirstChildElement() : nullptr;
}

const TiXmlElement* XML::getNextSiblingElement(const TiXmlElement* element /*= nullptr*/)
{
	return element ? element->NextSiblingElement() : nullptr;
}

std::string XML::getElementName(const TiXmlElement* element /* = nullptr */)
{
	return element ? element->Value() : "";
}

std::string XML::getElementValue(const TiXmlElement* element /*= nullptr*/)
{
	return element ? element->GetText() : "";
}

XML XML::setDeclarationElement(const std::string& version, const std::string& encoding)
{
	document.LinkEndChild(TiXmlDeclaration("1.0", "UTF-8", ""));
}

NS_END
