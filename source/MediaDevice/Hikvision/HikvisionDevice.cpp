#include "HCNetSDK.h"
#include "boost/make_shared.hpp"
#include "error.h"
#include "URL/Url.h"
using URL = framework::wrapper::URL;
#include "MediaData/MediaData.h"
#include "MediaDevice/Hikvision/HikvisionDevice.h"

void __stdcall fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{

}

namespace framework
{
	namespace multimedia
	{
		HikvisionDevice::HikvisionDevice() : MediaDevice(), loginID{ -1 }
		{}

		HikvisionDevice::~HikvisionDevice()
		{}

		int HikvisionDevice::openStream(const std::string url, void* hwnd /* = NULL */)
		{
			BOOL ret{ FALSE };
			int status{ url.empty() ? ERR_INVALID_PARAM : ERR_OK };
			//Initialize SDK when the first device was created.
			if (1 == MediaDevice::openStream(url))
			{
				ret = NET_DVR_Init();
			}

			if (ERR_OK == status && TRUE == ret)
			{
				URL urlParser;
				urlParser.setUrl(url);
				std::string name, password, address;
				unsigned short port;
				urlParser.getAuthentication(name, password);
				urlParser.getAddressPort(address, port);
				const std::string channel{ urlParser.getParameter("channel") };

				if (!name.empty() && !password.empty() && !address.empty() && 0 < port)
				{
					if (0 > loginID)
					{
						status = loginDevice(name, password, address, port);
					}

					if (ERR_OK == status)
					{
						status = openStream(atoi(channel.c_str()), hwnd);
					}
				}
			}

			return status;
		}

		int HikvisionDevice::closeStream()
		{
			int status{ ERR_OK };

			//Release SDK when the last device was destroyed.
			if (0 == MediaDevice::closeStream())
			{
				NET_DVR_Cleanup();
			}

			return status;
		}

		int HikvisionDevice::loginDevice(
			const std::string& username, const std::string password, const std::string ipaddr, const unsigned short port /* = 8000 */)
		{
			int status{ ERR_BAD_OPERATE };
			NET_DVR_DEVICEINFO_V40 deviceInfoV30{ 0 };
			NET_DVR_USER_LOGIN_INFO userLoginInfo{ 0 };
			strcpy_s(userLoginInfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, ipaddr.c_str());
			userLoginInfo.wPort = port;
			strcpy_s(userLoginInfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN, username.c_str());
			strcpy_s(userLoginInfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN, password.c_str());
// 			loginID = NET_DVR_Login_V30(
// 				const_cast<char*>(ipaddr.c_str()), port, const_cast<char*>(username.c_str()), const_cast<char*>(password.c_str()), &deviceInfoV30);
			loginID = NET_DVR_Login_V40(&userLoginInfo, &deviceInfoV30);

			if (-1 < loginID)
			{
// 				if (90 == deviceInfoV30.byDVRType)
// 				{
// 					getDVRConfig();
// 				}

				status = ERR_OK;
			}

			return status;
		}

		int HikvisionDevice::logoutDevice()
		{
			int status{ ERR_INVALID_PARAM };

			if (-1 < loginID)
			{
				status = NET_DVR_Logout(loginID) ? ERR_OK : ERR_BAD_OPERATE;
			}

			return status;
		}

		int HikvisionDevice::openStream(const int channel /* = 0 */, void* hwnd /* = NULL */)
		{
			int status{ -1 < channel ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				//Start live view and set callback data stream
				NET_DVR_PREVIEWINFO struPlayInfo{ 0 };
				struPlayInfo.hPlayWnd = (HWND)hwnd;
				struPlayInfo.lChannel = channel;
				struPlayInfo.dwStreamType = 0;
				struPlayInfo.dwLinkMode = 0;
 				struPlayInfo.bBlocked = 1;

				LONG streamID{ 
					NET_DVR_RealPlay_V40(loginID, &struPlayInfo, NULL/*&HikvisionDevice::realplayMediaDataCallback*/, NULL/*this*/) };
				if (streamID < 0)
				{
					DWORD dwErr = NET_DVR_GetLastError();
					status = ERR_BAD_OPERATE;
				}
				else
				{
					BOOL ret = NET_DVR_SetRealDataCallBackEx(streamID, fRealDataCallBack/*&HikvisionDevice::realplayMediaDataCallback*/, NULL/*this*/);
					if (ret)
					{
						status = ERR_OK;
					}
				}
			}
			
			return status;
		}

		int HikvisionDevice::getDVRConfig()
		{
			int status{ ERR_BAD_OPERATE };
			LONG groupNo = 0;
			DWORD dwReturned = 0;
			NET_DVR_IPPARACFG_V40 IPAccessCfgV40{ 0 };

			if (NET_DVR_GetDVRConfig(loginID, NET_DVR_GET_IPPARACFG_V40, groupNo, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
			{
				for (DWORD i = 0; i < IPAccessCfgV40.dwDChanNum; i++)
				{
					switch (IPAccessCfgV40.struStreamMode[i].byGetStreamType)
					{
						case 0: //Get stream from device
						{
							if (IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byEnable)
							{
// 								BYTE byIPID{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPID };
// 								BYTE byIPIDHigh{ IPAccessCfgV40.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh };
// 								int iDevInfoIndex{ byIPIDHigh * 256 + byIPID - 1 - groupNo * 64 };
// 
// 								Camera camera(loginDeviceIP, IPAccessCfgV40.struIPDevInfo[iDevInfoIndex].struIP.sIpV4, IPAccessCfgV40.dwStartDChan + i);
// 								digitCameras.push_back(camera);
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

		void HikvisionDevice::realplayMediaDataCallback(
			long streamID, unsigned long dataType, unsigned char* data, unsigned long dataBytes, void* pUser)
		{
// 			HikvisionDevice* hikvisionDevice{ reinterpret_cast<HikvisionDevice*>(pUser) };
// 
// 			if (hikvisionDevice)
// 			{
// 				MediaDataPtr mediaDataPtr{
// 					boost::make_shared<MediaData>(
// 						MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_HIKVISION) };
// 				if (mediaDataPtr && hikvisionDevice->mediaDataCaptureCallback)
// 				{
// 					mediaDataPtr->setData(data, dataBytes);
// 					hikvisionDevice->mediaDataCaptureCallback(mediaDataPtr);
// 				}
// 			}
		}
	}//namespace multimedia
}//namespace framework
