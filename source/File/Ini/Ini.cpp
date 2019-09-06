#include "boost/property_tree/ini_parser.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "error.h"
#include "File/Ini/Ini.h"

NS_BEGIN(file, 1)
 
Ini::Ini()
{}

Ini::~Ini()
{}

FILE* Ini::openFile(const std::string path)
{
	try
	{
		boost::property_tree::ini_parser::read_ini(path, rootNode);
	}
	catch (std::exception& e)
	{}
	
	return NULL;
}

void Ini::closeFile(FILE* fd /* = NULL */)
{}

Int32 Ini::readingIntValueByKey(const std::string key)
{

}

std::string Ini::readingStringValueByKey(const std::string key)
{

}

NS_END
