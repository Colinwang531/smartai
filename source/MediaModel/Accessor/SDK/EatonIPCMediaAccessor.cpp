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
//	std::string dllFilePath{ (boost::format("%s\\ipcsdk.dll") % filePath).str() };

	BOOL32 bRet = IPC_InitDll(const_cast<char*>("ipcsdk.dll"/*dllFilePath.c_str()*/), 3300, bOpenTelnet, &errorCode);
	if (bRet != 1)
	{
		return ERR_INITFAILD;
	}

//	dllFilePath = (boost::format("%s\\ipcmedia.dll") % filePath).str();
	bRet = IPC_InitDll(const_cast<char*>("ipcmedia.dll"/*dllFilePath.c_str()*/), 3000, bOpenTelnet, &errorCode);
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
				RET_SUCCESS == IPC_AddConnectDetect(&streamID, 3000, 2, DetectConnect, NULL, &errorCode))
			{
				status = ERR_OK;
			}
		}
	}

	return status;
}

void EatonIPCMediaAccessor::DetectConnect(
	unsigned long dwIP, unsigned short wPort, unsigned long dwHandle,
	unsigned long dwCBconnectType, unsigned long dwDataLen, unsigned long dwData, void* pContext)
{
	if (dwCBconnectType == em_connect_no)
	{
	}
	else if (dwCBconnectType == em_connect_ok)
	{
	}
}

NS_END
