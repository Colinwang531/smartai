#ifndef HIKVISION_7XXX_NVR_H
#define HIKVISION_7XXX_NVR_H

#include "boost/function.hpp"
#include "Device/Hikvision/HikvisionNVR.h"
using HikvisionNVR = NS(device, 1)::HikvisionNVR;

typedef boost::function<void(const int, const char*, const int)> DigitCameraParametersNotifyHandler;

class Hikvision7xxxNVR final : public HikvisionNVR
{
public:
	Hikvision7xxxNVR(
		DigitCameraParametersNotifyHandler handler = NULL);
    ~Hikvision7xxxNVR(void);

	int gotDigitCameraParameters(DigitCameraParameters& parameters) override;

private:
	DigitCameraParametersNotifyHandler digitCameraParametersNotifyHandler;
};//class Hikvision7xxxNVR

#endif//HIKVISION_7XXX_NVR_H