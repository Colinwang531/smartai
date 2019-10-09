#include "Camera/Camera.h"

NS_BEGIN(device, 1)

Camera::Camera(
	const std::string parent, const std::string address, const unsigned long long idx /* = 0 */)
	: parentIP{ parent }, cameraIP{ address }, cameraIndex{ idx }
{}

Camera::~Camera()
{}

NS_END
