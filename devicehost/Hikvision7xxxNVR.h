#ifndef HIKVISION_7XXX_NVR_H
#define HIKVISION_7XXX_NVR_H

#include "boost/function.hpp"
#include "Device/Hikvision/HikvisionNVR.h"
using HikvisionNVR = NS(device, 1)::HikvisionNVR;
using DigitCamera = NS(device, 1)::DigitCamera;

class Hikvision7xxxNVR final : public HikvisionNVR
{
public:
	Hikvision7xxxNVR(void);
    ~Hikvision7xxxNVR(void);

	int getDigitCameras(
		const int userID, const char* NVRIp, std::vector<DigitCamera>& cameras) override;
};//class Hikvision7xxxNVR

#endif//HIKVISION_7XXX_NVR_H