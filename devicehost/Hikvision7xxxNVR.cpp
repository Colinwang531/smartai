#include"error.h"
#include "Hikvision7xxxNVR.h"

Hikvision7xxxNVR::Hikvision7xxxNVR(DigitCameraParametersNotifyHandler handler /* = NULL */)
	: HikvisionNVR(), digitCameraParametersNotifyHandler{ handler }
{}

Hikvision7xxxNVR::~Hikvision7xxxNVR()
{}

int Hikvision7xxxNVR::gotDigitCameraParameters(DigitCameraParameters& parameters)
{
	int status{ HikvisionNVR::gotDigitCameraParameters(parameters) };

	if (ERR_OK == status && digitCameraParametersNotifyHandler)
	{
		for (DigitCameraParameters::const_iterator it = parameters.cbegin(); it != parameters.cend(); ++it)
		{
			digitCameraParametersNotifyHandler(userID, it->first.c_str(), it->second);
//			break;
		}

		status = ERR_OK;
	}

	return status;
}
