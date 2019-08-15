//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@hotmail.com
//		Date :							2019-08-04
//		Description :				�豸������д�ӿ�
//
//		History:						Author										Date													Description
//											������										2019-08-04									����
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
