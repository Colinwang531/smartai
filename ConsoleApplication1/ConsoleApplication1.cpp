// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Url/Url.h"
using URL = framework::wrapper::URL;

int main(int argc, char* argv[])
{
	URL url;
	url.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url.addAuthentication("admin", "eaton12345");
	url.setAddressPort("192.168.3.20", 65530);
	url.addParameter("channel", "1");
	url.addParameter("stream", "2");
	printf_s("%s\r\n", url.getUrl().c_str());

	URL url1;
	url1.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url1.setAddressPort("192.168.30.220", 8000);
	url1.addParameter("channel", "1");
	url1.addAuthentication("admin", "eaton");
	printf_s("%s\r\n", url1.getUrl().c_str());

	URL url2;
	url2.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url2.setAddressPort("192.168.30.220", 8000);
	url2.addAuthentication("admin", "eaton");
	url2.addParameter("channel", "1");
	printf_s("%s\r\n", url2.getUrl().c_str());

	URL url3;
	url3.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url3.addAuthentication("admin", "eaton");
	url3.addParameter("channel", "1");
	url3.setAddressPort("192.168.30.220", 8000);
	printf_s("%s\r\n", url3.getUrl().c_str());

	URL url4;
	url4.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url4.addParameter("channel", "1");
	url4.addAuthentication("admin", "eaton");
	url4.setAddressPort("192.168.30.220", 8000);
	printf_s("%s\r\n", url4.getUrl().c_str());

	URL url5;
	url5.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url5.addParameter("channel", "1");
	url5.setAddressPort("192.168.30.220", 8000);
	url5.addAuthentication("admin", "eaton");
	printf_s("%s\r\n", url5.getUrl().c_str());

	URL url6;
	url6.addParameter("channel", "1");
	url6.setProtocol(framework::wrapper::URLType::URL_TYPE_LIVESTREAM);
	url6.setAddressPort("192.168.30.220", 8000);
	url6.addAuthentication("admin", "eaton");
	printf_s("%s\r\n", url6.getUrl().c_str());

	const framework::wrapper::URLType type{ url.getProtocol() };
	std::string address;
	unsigned short port;
	url.getAddressPort(address, port);
	std::string name, password;
	url.getAuthentication(name, password);
	url.getParameter("channel");

	getchar();

	return 0;
}

