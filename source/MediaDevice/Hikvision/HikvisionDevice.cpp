#include "HCNetSDK.h"
#include "boost/make_shared.hpp"
#include "error.h"
#include "URL/Url.h"
using URL = framework::wrapper::URL;
#include "MediaData/MediaData.h"
#include "MediaDevice/Hikvision/HikvisionDevice.h"

namespace framework
{
	namespace multimedia
	{
		HikvisionDevice::HikvisionDevice() : MediaDevice(), loginID{ -1 }
		{}

		HikvisionDevice::~HikvisionDevice()
		{}

		int HikvisionDevice::openStream(const std::string& streamUrl)
		{
			BOOL ret{ FALSE };
			int status{ streamUrl.empty() ? ERR_INVALID_PARAM : ERR_OK };
			//Initialize SDK when the first device was created.
			if (1 == MediaDevice::openStream(streamUrl))
			{
				ret = NET_DVR_Init();
			}

			if (ERR_OK == status && TRUE == ret)
			{
				URL url;
				url.setUrl(streamUrl);
				std::string name, password, address;
				unsigned short port;
				url.getAuthentication(name, password);
				url.getAddressPort(address, port);
				const std::string channel{ url.getParameter("channel") };

				if (!name.empty() && !password.empty() && !address.empty() && 0 < port)
				{
					if (0 > loginID)
					{
						status = loginDevice(name, password, address, port);
					}

					if (ERR_OK == status)
					{
						status = openStream(atoi(channel.c_str()));
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
			NET_DVR_DEVICEINFO_V30 deviceInfoV30{ 0 };
			loginID = NET_DVR_Login_V30(
				const_cast<char*>(ipaddr.c_str()), port, const_cast<char*>(username.c_str()), const_cast<char*>(password.c_str()), &deviceInfoV30);

			if (-1 < loginID)
			{
				if (90 == deviceInfoV30.byDVRType)
				{
					getDVRConfig();
				}

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

		int HikvisionDevice::openStream(const int channel /* = 0 */)
		{
			int status{ -1 < channel ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				//Start live view and set callback data stream
				NET_DVR_PREVIEWINFO struPlayInfo{ 0 };
				struPlayInfo.hPlayWnd = NULL;
				struPlayInfo.lChannel = channel;
				struPlayInfo.dwStreamType = 0;
				struPlayInfo.dwLinkMode = 0;
				struPlayInfo.bBlocked = 1;

				LONG streamID{ 
					NET_DVR_RealPlay_V40(loginID, &struPlayInfo, &HikvisionDevice::realplayMediaDataCallback, this) };
				if (streamID < 0)
				{
					status = ERR_BAD_OPERATE;
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
			HikvisionDevice* hikvisionDevice{ reinterpret_cast<HikvisionDevice*>(pUser) };

			if (hikvisionDevice)
			{
				MediaDataPtr mediaDataPtr{
					boost::make_shared<MediaData>(
						MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_HIKVISION) };
				if (mediaDataPtr && hikvisionDevice->mediaDataCaptureCallback)
				{
					mediaDataPtr->setData(data, dataBytes);
					hikvisionDevice->mediaDataCaptureCallback(mediaDataPtr);
				}
			}
		}
	}//namespace multimedia
}//namespace framework
