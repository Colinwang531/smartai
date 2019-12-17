// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of HIKVISION device.
// Support DVR / NVR / IPC / DECODER etc.
//

#ifndef HIKVISION_DEVICE_H
#define HIKVISION_DEVICE_H

#include "predef.h"
#include "MediaDevice/MediaDevice.h"
#include "MediaDevice/EnableDeviceLoginAndLogout.h"

namespace framework
{
	namespace multimedia
	{
		class HikvisionDevice
			: public MediaDevice, protected EnableDeviceLoginAndLogout
		{
		public:
			HikvisionDevice(void);
			virtual ~HikvisionDevice(void);

		protected:
			//Return : Error code.
			int openStream(const std::string url, void* hwnd = NULL) override;
			int closeStream(void) override;
			int loginDevice(
				const std::string& username, const std::string password, const std::string ipaddr, const unsigned short port = 8000) override;
			int logoutDevice(void) override;

		private:
			int openStream(const int channel = 0, void* hwnd = NULL);
			int getDVRConfig(void);
			static void CALLBACK realplayMediaDataCallback(
				long streamID, unsigned long dataType, unsigned char* data, unsigned long dataBytes, void* pUser);

		protected:
			int loginID;
		};//class HikvisionDevice
	}//namespace multimedia
}//namespace framework

#endif//HIKVISION_DEVICE_H
