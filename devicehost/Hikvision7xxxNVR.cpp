#include"error.h"
#include "Hikvision7xxxNVR.h"

Hikvision7xxxNVR::Hikvision7xxxNVR() : HikvisionNVR()
{}

Hikvision7xxxNVR::~Hikvision7xxxNVR()
{}

int Hikvision7xxxNVR::getDigitCameras(
	const int userID, const char* NVRIp, std::vector<DigitCamera>& cameras)
{
	return HikvisionNVR::getDigitCameras(userID, NVRIp, cameras);
}
