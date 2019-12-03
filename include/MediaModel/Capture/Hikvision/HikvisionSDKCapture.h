// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// AV live stream accessor using HIKVISION SDK.
//

#ifndef HIKVISION_IPC_MEDIA_ACCESSOR_H
#define HIKVISION_IPC_MEDIA_ACCESSOR_H

#include "boost/shared_ptr.hpp"
#include "Device/Device.h"
using DevicePtr = boost::shared_ptr<NS(device, 1)::Device>;
#include "MediaModel/Accessor/MediaAccessor.h"

NS_BEGIN(model, 1)

class HikvisionIPCMediaAccessor : public MediaAccessor
{
public:
	HikvisionIPCMediaAccessor(void);
	virtual ~HikvisionIPCMediaAccessor(void);

public:
	int openStream(
		const std::string name, const std::string password,
		const std::string address, const int port = 8000,
		const int channel = 0, void* hwnd = NULL) override;
	int closeStream(void) override;

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	DevicePtr devicePtr;
	unsigned long streamID;
};//class HikvisionIPCMediaAccessor

NS_END

#endif//HIKVISION_IPC_MEDIA_ACCESSOR_H
