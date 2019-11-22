// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of device.
//

#ifndef DEVICE_H
#define DEVICE_H

#include "boost/function.hpp"
#include "Mutex/RWLock.h"
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(device, 1)

typedef boost::function<int(MediaDataPtr)> PostInputMediaDataCallback;

class Device
{
public:
	Device(void);
	virtual ~Device(void);

public:
	virtual int createNewDevice(void);
	virtual int destoryDevice(void);
	inline unsigned long long getDeviceCount(void) const
	{
		ReadLock rl{ mtx };
		return deviceCount;
	}
	inline void setPostInputMediaDataCallback(PostInputMediaDataCallback callback = NULL)
	{
		postInputMediaDataCallback = callback;
	}

protected:
	static SharedMutex mtx;
	static unsigned long long deviceCount;
	PostInputMediaDataCallback postInputMediaDataCallback;
};//class Device

NS_END

#endif//DEVICE_H
