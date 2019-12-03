#include "boost/make_shared.hpp"
#include "HCNetSDK.h"
#include "error.h"
#include "Camera/Camera.h"
#include "Device/Hikvision/HikvisionIPC.h"

NS_BEGIN(device, 1)

void loginResultCallBack(
	LONG                        lUserID,
	DWORD                       dwResult,
	LPNET_DVR_DEVICEINFO_V30    lpDeviceInfo,
	void* pUser)
{
	return;
}

HikvisionIPC::HikvisionIPC(
	const std::string name, const std::string password,
	const std::string ipaddr, const unsigned short port /* = 0 */)
	: HikvisionDevice(name, password, ipaddr, port), streamID{ -1 }
{}

HikvisionIPC::~HikvisionIPC()
{}

int HikvisionIPC::openStream(void* hwnd /* = NULL */)
{
	int status{ ERR_OK };
	//Start live view and set callback data stream
	NET_DVR_PREVIEWINFO struPlayInfo{ 0 };
	struPlayInfo.hPlayWnd = reinterpret_cast<HWND>(hwnd);
	struPlayInfo.lChannel = 1;
	struPlayInfo.dwStreamType = 0;
	struPlayInfo.dwLinkMode = 0;
	struPlayInfo.bBlocked = 1;

	streamID = NET_DVR_RealPlay_V40(loginUserID, &struPlayInfo, &HikvisionIPC::realplayDataCallback, this);
	if (streamID < 0)
	{
		status = ERR_BAD_OPERATE;
	}

	return status;
}

int HikvisionIPC::closeStream(void)
{
	return 0;
}

int HikvisionIPC::loginDevice()
{
	int status{ ERR_OK };

	//Device information, output parameters
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40{};
	//Login parameters, including IP address, user name, and password, etc.
	NET_DVR_USER_LOGIN_INFO struLoginInfo{};
	struLoginInfo.bUseAsynLogin = 0; //synchronous login mode,0-NO,1-YES
	struLoginInfo.cbLoginResult = loginResultCallBack;
	struLoginInfo.pUser = this;
	strcpy_s(struLoginInfo.sDeviceAddress, loginDeviceIP.c_str());
	struLoginInfo.wPort = loginDevicePort;
	strcpy_s(struLoginInfo.sUserName, loginUserName.c_str());
	strcpy_s(struLoginInfo.sPassword, loginUserPassword.c_str());

	loginUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (-1 == loginUserID)
	{
		status = ERR_BAD_OPERATE;
	}

	return status;
}

void HikvisionIPC::realplayDataCallback(
	long streamID, unsigned long dataType, unsigned char* data, unsigned long dataBytes, void* pUser)
{
	HikvisionIPC* hikvisionIPC{ reinterpret_cast<HikvisionIPC*>(pUser) };

	if (hikvisionIPC && hikvisionIPC->postInputMediaDataCallback)
	{
		MediaDataPtr mediaDataPtr{ 
			boost::make_shared<NS(media, 1)::MediaData>(
				MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_HIKVISION) };
		if (mediaDataPtr)
		{
			mediaDataPtr->setData(data, dataBytes);
			hikvisionIPC->postInputMediaDataCallback(mediaDataPtr);
		}
	}
}

NS_END
