#ifndef DEVICE_HOST_H
#define DEVICE_HOST_H

#include <string>
#include <vector>
#include "boost/shared_ptr.hpp"
#include <Camera/Camera.h>
using Camera = NS(device, 1)::Camera;
#include "MediaFrame/MediaFrame.h"
using MediaImage = NS(frame, 1)::MediaImage;
using MediaImagePtr = boost::shared_ptr<MediaImage>;

int createNewNVRDevice(
	const std::string address, const unsigned short port,
	const std::string name, const std::string password,
	std::vector<Camera>& digitCameras);
int destroyNVRDevice(const std::string address);
int createNewDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0, const unsigned int abilities = 0);
int destroyDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0, const unsigned int abilities = 0);
MediaImagePtr captureDigitCameraLivePicture(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0);
int setAutoCheckSailOrPort(const int autoCheck = 1);
int setSailingStatus(const int status = 0);
int getSailingStatus(void);
int createNewFacePicture(
	const char* imageData = NULL, const int imageBytes = 0, const char* name = NULL, const long long uuid = 0);
int queryFacePicture(const long long uuid, char*& imageData, int& imageBytes);

#endif//DEVICE_HOST_H