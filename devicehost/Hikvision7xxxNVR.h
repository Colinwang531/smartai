#ifndef HIKVISION_7XXX_NVR_H
#define HIKVISION_7XXX_NVR_H

#include "Device/Hikvision/HikvisionNVR.h"
using HikvisionNVR = NS(device, 1)::HikvisionNVR;

class Hikvision7xxxNVR final : public HikvisionNVR
{
public:
	Hikvision7xxxNVR(
		const char* userName = NULL, const char* userPassword = NULL,
		const char* deviceIP = NULL, const unsigned short devicePort = 0);
    ~Hikvision7xxxNVR(void);
};//class Hikvision7xxxNVR

#endif//HIKVISION_7XXX_NVR_H