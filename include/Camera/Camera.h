//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-01
//		Description :				摄像机类
//
//		History:						Author										Date													Description
//											王科威										2019-10-01									创建
//

#ifndef CAMERA_H
#define CAMERA_H

#include "predef.h"

NS_BEGIN(device, 1)

class Camera
{
public:
	Camera(
		const std::string parent, const std::string address, const unsigned long long idx = 0);
	virtual ~Camera(void);

public:
	inline const std::string getParentIP(void) const
	{
		return parentIP;
	}

	inline const std::string getCameraIP(void) const
	{
		return cameraIP;
	}
	
	inline const unsigned long long getCameraIndex(void) const
	{
		return cameraIndex;
	}

protected:
	const std::string parentIP;
	const std::string cameraIP;
	const unsigned long long cameraIndex;
};//class Camera

NS_END

#endif//CAMERA_H
