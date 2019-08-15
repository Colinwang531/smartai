#include"error.h"
#include "Hikvision7xxxNVR.h"

Hikvision7xxxNVR::Hikvision7xxxNVR(DigitCameraParametersNotifyHandler handler /* = NULL */)
	: HikvisionNVR(), digitCameraParametersNotifyHandler{ handler }
{}

Hikvision7xxxNVR::~Hikvision7xxxNVR()
{}

int Hikvision7xxxNVR::getDigitCameraParameters(
	const std::string NVRIp, std::vector<DigitCameraParameters>& parameters)
{
	int status{ HikvisionNVR::getDigitCameraParameters(NVRIp, parameters) };

	if (ERR_OK == status && digitCameraParametersNotifyHandler)
	{
		for (std::vector<DigitCameraParameters>::const_iterator it = parameters.cbegin(); it != parameters.cend(); ++it)
		{
			digitCameraParametersNotifyHandler(parameters, NVRIp.c_str());
		}

		status = ERR_OK;
	}

	return status;
}
