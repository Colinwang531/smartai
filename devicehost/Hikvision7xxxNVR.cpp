#include"error.h"
#include "Hikvision7xxxNVR.h"

Hikvision7xxxNVR::Hikvision7xxxNVR(
	const char* userName /* = NULL */, const char* userPassword /* = NULL */, 
	const char* deviceIP /* = NULL */, const unsigned short devicePort /* = 0 */)
	: HikvisionNVR(userName, userPassword, deviceIP, devicePort)
{}

Hikvision7xxxNVR::~Hikvision7xxxNVR()
{}
