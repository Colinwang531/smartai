#include "HCNetSDK.h"
#include "error.h"
#include "Device/Hikvision/HikvisionNVR.h"

NS_BEGIN(device, 1)

HikvisionNVR::HikvisionNVR() : HikvisionDevice(), Configing()
{}

HikvisionNVR::~HikvisionNVR()
{}

int HikvisionNVR::login(
	const char* name /* = NULL */, const char* password /* = NULL */, 
	const char* ip /* = NULL */, const unsigned short port /* = 0 */,
	const bool sync /* = true */)
{
	NET_DVR_USER_LOGIN_INFO userLoginInfo{ 0 };
	NET_DVR_DEVICEINFO_V40 deviceInfoV40{ 0 };

#ifdef _WINDOWS
	strncpy_s(userLoginInfo.sDeviceAddress, ip, MAX_IPV6_LEN);
	strncpy_s(userLoginInfo.sUserName, name, MAX_NAME_LEN);
	strncpy_s(userLoginInfo.sPassword, password, MAX_PASSWD_LEN_EX);
#else
	strncpy(userLoginInfo.sDeviceAddress, ip, MAX_IPV6_LEN);
	strncpy(userLoginInfo.sUserName, name, MAX_NAME_LEN);
	strncpy(userLoginInfo.sPassword, password, MAX_PASSWD_LEN_EX);
#endif//_WINDOWS
	userLoginInfo.wPort = port;
	userLoginInfo.bUseAsynLogin = sync ? TRUE : FALSE;
	userLoginInfo.cbLoginResult = NULL;
	userLoginInfo.pUser = this;
//	userID = NET_DVR_Login_V40(&userLoginInfo, &deviceInfoV40);

	NET_DVR_DEVICEINFO_V30 deviceInfoV30{ 0 };
	userID = NET_DVR_Login_V30((char*)ip, port, (char*)name, (char*)password, &deviceInfoV30);
	return userID;
}

int HikvisionNVR::logout()
{
	return NET_DVR_Logout(userID);
}

int HikvisionNVR::getDigitCameras(
	const int userID, const char* NVRIp, std::vector<DigitCamera>& cameras)
{
	int status{ ERR_BAD_OPERATE };
	LONG groupNo = 0;
	DWORD dwReturned = 0;
	//Get digital channel parameters
	NET_DVR_IPPARACFG_V40 IPAccessCfgV40{ 0 };
//	memset(&IPAccessCfgV40, 0, sizeof(NET_DVR_IPPARACFG_V40));

	if (NET_DVR_GetDVRConfig(userID, NET_DVR_GET_IPPARACFG_V40, groupNo, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
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

						DigitCamera camera;
						camera.NVRIp = NVRIp;
						camera.cameraIp = IPAccessCfgV40.struIPDevInfo[iDevInfoIndex].struIP.sIpV4;
						camera.cameraIndex = (int)(IPAccessCfgV40.dwStartDChan + i);
						cameras.push_back(camera);
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
	}

	return status;
}

NS_END
