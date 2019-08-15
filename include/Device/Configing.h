//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2019-08-04
//		Description :				设备参数读写接口
//
//		History:						Author										Date													Description
//											王科威										2019-08-04									创建
//

#ifndef CONFIGING_H
#define CONFIGING_H

#include <string>
#include <vector>
#include "predef.h"

NS_BEGIN(device, 1)

typedef struct tagDigitCameraParameters_t
{
	std::string NVRIp;
	std::string cameraIp;
	int cameraIndex;
}DigitCameraParameters;

class Configing
{
protected:
	Configing(void) {}
	virtual ~Configing(void){}

protected:
	virtual int getDigitCameraParameters(
		const std::string NVRIp, std::vector<DigitCameraParameters>& parameters) = 0;
};//class Configing

NS_END

#endif//CONFIGING_H
