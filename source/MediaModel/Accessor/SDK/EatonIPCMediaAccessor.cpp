#include <WinSock2.h>
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "ipcdefine.h"
#include "ipcstruct.h"
#include "ipcsdk_c.h"
#include "error.h"
#include "MediaModel/Accessor/SDK/EatonIPCMediaAccessor.h"

NS_BEGIN(model, 1)

EatonIPCMediaAccessor::EatonIPCMediaAccessor() : MediaAccessor(), streamID{ 0 }
{}

EatonIPCMediaAccessor::~EatonIPCMediaAccessor(void)
{
	closeStream();
}

int EatonIPCMediaAccessor::openStream(
	const std::string name, const std::string password, 
	const std::string address, const int port /* = 8000 */, 
	const int channel /* = 0 */)
{
	int status{ MediaAccessor::openStream(name, password, address, port, channel) };

	if (ERR_OK == status)
	{
		const std::string executePath{
				boost::filesystem::initial_path<boost::filesystem::path>().string() };
		status = initializeEatonSDK(executePath);

		if (ERR_OK == status)
		{
			status = loginIPCDevice(name, password, address, port);
		}
	}

	return status;
}

int EatonIPCMediaAccessor::closeStream()
{
	return MediaAccessor::closeStream();
}

int EatonIPCMediaAccessor::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_SUPPORT;
}

int EatonIPCMediaAccessor::initializeEatonSDK(const std::string filePath)
{
	u32 errorCode{ RET_SUCCESS };
	bool bOpenTelnet{ false };

	BOOL32 bRet = IPC_InitDll(const_cast<char*>("ipcsdk.dll"), 3300, bOpenTelnet, &errorCode);
	if (bRet != 1)
	{
		return ERR_INITFAILD;
	}

	bRet = IPC_InitDll(const_cast<char*>("ipcmedia.dll"), 3000, bOpenTelnet, &errorCode);
	if (bRet != 1)
	{
		return ERR_INITFAILD;
	}

	return ERR_OK;
}

int EatonIPCMediaAccessor::loginIPCDevice(
	const std::string name, const std::string password, const std::string address, const int port)
{
	int status{ ERR_BAD_OPERATE };
	// create handle and get handle variable
	std::vector<std::string> ipaddrSegment;
	boost::split(ipaddrSegment, address, boost::is_any_of("."));
	unsigned long ipaddr{ inet_addr(((boost::format("%s.%s.%s.%s") % ipaddrSegment[3] % ipaddrSegment[2] % ipaddrSegment[1] % ipaddrSegment[0]).str()).c_str()) };
	streamID = IPC_CreateHandle(ipaddr, port, (char*)name.c_str(), (char*)password.c_str());

	if (0 <= streamID)
	{
		u32 errorCode{ RET_SUCCESS };
		BOOL32 bRet = IPC_Login(&streamID, (char*)name.c_str(), (char*)password.c_str(), &errorCode);
		if (bRet != 1)
		{
			IPC_DestroyHandle();
		}
		else
		{
			if (RET_SUCCESS == IPC_DelConnectDetect(&streamID, &errorCode) &&
				RET_SUCCESS == IPC_AddConnectDetect(&streamID, 3000, 3, DetectConnect, this, &errorCode))
			{
				status = ERR_OK;
			}
		}
	}

	return status;
}

int EatonIPCMediaAccessor::startIPCRealplay(void)
{
// 	TPLAYVIDEOPARAM tPlayVideoParam;
// 	memset(&tPlayVideoParam, 0, sizeof(TPLAYVIDEOPARAM));
// 	// 申请UDP码流
// 	tPlayVideoParam.byVideoSouce = 1;
// 	tPlayVideoParam.byStreamChn = 1;
// 	tPlayVideoParam.dwSrcIP = dwLocalIp;
// 	strcpy(tPlayVideoParam.szStreamType, "both");
// 	strcpy(tPlayVideoParam.szTransType, "rtp_udp");
// 	tPlayVideoParam.wRtpVideoPort = wVideoPort;
// 	tPlayVideoParam.wRtpAudioPort = wAudioPort;
// 	tPlayVideoParam.wRtpAudioPort2 = 0;
// 	tPlayVideoParam.wRtcpAudioPort2 = 0;
// 	tPlayVideoParam.wRtcpVideoPort = wVideoPort + 1;
// 	tPlayVideoParam.wRtcpAudioPort = wAudioPort + 1;
// 
// 	dwPlayId = IPC_StartRealPlay(&m_dwHandle, type_udp, &tPlayVideoParam, sizeof(TPLAYVIDEOPARAM), &tPlayVideoInfo, nLenInfo, &nError);
// 	if (nError != RET_SUCCESS)
// 	{
// 		CString str;
// 		str.Format(_T("Start Play Video Fail，Error Code：%d"), nError);
// 		MessageBox(str);
// 		return;
// 	}
// 	TSwitchParam tSwitchParam;
// 	memset(&tSwitchParam, 0, sizeof(TSwitchParam));
// 	tSwitchParam.tPlayPortInfo.tPlayVideoPort.wRtpPort = wVideoPort;
// 	tSwitchParam.tPlayPortInfo.tPlayVideoPort.wRtcpPort = wVideoPort + 1;
// 	tSwitchParam.tPlayPortInfo.tPlayVideoPort.wRtcpBackPort = tPlayVideoInfo.wRtcpVideoPort;
// 	tSwitchParam.tPlayPortInfo.tPlayAudioPort.wRtpPort = wAudioPort;
// 	tSwitchParam.tPlayPortInfo.tPlayAudioPort.wRtcpPort = wAudioPort + 1;
// 	tSwitchParam.tPlayPortInfo.tPlayAudioPort.wRtcpBackPort = tPlayVideoInfo.wRtcpAudioPort;
// 
// 	tSwitchParam.tRemotePortInfo.wRemoteVideoPort = 59000;
// 	tSwitchParam.tRemotePortInfo.wRemoteAudioPort = 59002;
// 
// 	tSwitchParam.tEncNameAndPayload.eEncName = E_ENCNAME_H264;
// 	tSwitchParam.tEncNameAndPayload.byPayload = 116;
// 
// 	s32 nMediaRet = MEDIA_SetSwitch(m_dwMediaID, g_dwIP, dwLocalIp, tSwitchParam);
// 	if (nMediaRet != MEDIARCV_SUCCESS)
// 	{
// 		CString str;
// 		str.Format(_T("MEDIA_SetSwitch，error code：%d"), nMediaRet);
// 		MessageBox(str);
// 	}
// }
// 
// // 申请码流的情况下才需要回调帧数据过来解码，如果只是rtsp链路接收告警，就不需要
// s32 nMediaRet = MEDIA_SetFrameCallBack(m_dwMediaID, frameCb, this);
// if (nMediaRet != MEDIARCV_SUCCESS)
// {
// 	CString str;
// 	str.Format(_T("MEDIA_SetFrameCallBack，error code：%d"), nMediaRet);
// 	MessageBox(str);
// }
	return 0;
}

void EatonIPCMediaAccessor::DetectConnect(
	unsigned int dwIP, unsigned short wPort, 
	unsigned int dwHandle, unsigned int dwCBconnectType, 
	unsigned int dwDataLen, unsigned int dwData, 
	void* pContext)
{
	if (dwCBconnectType == em_connect_no)
	{
	}
	else if (dwCBconnectType == em_connect_ok)
	{
	}
}

NS_END
