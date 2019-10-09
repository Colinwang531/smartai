#ifndef DEVICE_HOST_H
#define DEVICE_HOST_H

#include <string>
#include <vector>
#include <Camera/Camera.h>
using Camera = NS(device, 1)::Camera;

/*static*/ int createNewNVRDevice(
	const std::string address, const unsigned short port,
	const std::string name, const std::string password,
	std::vector<Camera>& digitCameras);
/*static*/ int destroyNVRDevice(const std::string address);
/*static*/ int createNewDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0, const unsigned int abilities = 0);
/*static*/ int destroyDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0);
/*static*/ int setAutoCheckSailOrPort(const int autoCheck = 1);
/*static*/ int setSailingStatus(const int status = 0);
/*static*/ int getSailingStatus(void);

#endif//DEVICE_HOST_H