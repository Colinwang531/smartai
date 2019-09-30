#include "HCNetSDK.h"
#include "error.h"
#include "Device/Hikvision/HikvisionNVR.h"

NS_BEGIN(device, 1)

HikvisionNVR::HikvisionNVR(
	const char* userName /* = NULL */, const char* userPassword /* = NULL */, 
	const char* deviceIP /* = NULL */, const unsigned short devicePort /* = 0 */)
	: HikvisionDevice(userName, userPassword, deviceIP, devicePort), EnableDeviceConfig()
{}

HikvisionNVR::~HikvisionNVR()
{}

int HikvisionNVR::loginDevice()
{
	int status{ ERR_BAD_OPERATE };

	if (!loginUserName.empty() && !loginUserPassword.empty() && !loginDeviceIP.empty() && 
		minPortNumber < loginDevicePort && maxPortNumber > loginDevicePort)
	{
		NET_DVR_DEVICEINFO_V30 deviceInfoV30{ 0 };
		loginUserID = NET_DVR_Login_V30(
			const_cast<char*>(loginDeviceIP.c_str()), loginDevicePort, 
			const_cast<char*>(loginUserName.c_str()), const_cast<char*>(loginUserPassword.c_str()),
			&deviceInfoV30);

		if (-1 < loginUserID)
		{
			status = getNVRDigitCameras();
		}
	}

	return status;
}

int HikvisionNVR::getNVRDigitCameras()
{
	int status{ ERR_BAD_OPERATE };
	LONG groupNo = 0;
	DWORD dwReturned = 0;
	NET_DVR_IPPARACFG_V40 IPAccessCfgV40{ 0 };

	if (NET_DVR_GetDVRConfig(loginUserID, NET_DVR_GET_IPPARACFG_V40, groupNo, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
	{
		status = ERR_OK;
		for (DWORD i = 0; i < IPAccessCfgV40.dwDChanNum; i++)
		{
			switch (IPAccessCfgV40.struStreamMode[i].byGetStreamType)
			{
				case 0: //Get stream from device
				{
					if (IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byEnable)
					{
						BYTE byIPID{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPID };
						BYTE byIPIDHigh{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh };
						int iDevInfoIndex{ byIPIDHigh * 256 + byIPID - 1 - groupNo * 64 };

// 						DigitCamera camera;
// 						camera.NVRIp = NVRIp;
// 						camera.cameraIp = IPAccessCfgV40.struIPDevInfo[iDevInfoIndex].struIP.sIpV4;
// 						camera.cameraIndex = (int)(IPAccessCfgV40.dwStartDChan + i);
// 						cameras.push_back(camera);
					}

					break;
				}
				case 1: //Get stream from stream media server
				{
					if (IPAccessCfgV40.struStreamMode[i].uGetStream.struPUStream.struStreamMediaSvrCfg.byValid)
					{
// 						printf("IP channel %d connected with the IP device by stream server.\n", i + 1);
// 						printf("IP of stream server: %s, IP of IP device: %s\n", IPAccessCfgV40.struStreamMode[i].uGetStream.\
// 							struPUStream.struStreamMediaSvrCfg.struDevIP.sIpV4, IPAccessCfgV40.struStreamMode[i].uGetStream.\
// 							struPUStream.struDevChanInfo.struIP.sIpV4);
					}

					break;
				}
				default:
					break;
			}
		}

		status = ERR_OK;
	}

	return status;
}

NS_END
