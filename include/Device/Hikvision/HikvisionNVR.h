//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				����NVRϵ��ʵ����
//
//		History:						Author										Date													Description
//											������										2019-07-19									����
//

#ifndef HIKVISION_NVR_H
#define HIKVISION_NVR_H

#include "boost/unordered_map.hpp"
using DigitCameraParameters = boost::unordered_map<const std::string, const int>;
#include "Device/HikvisionDevice.h"

NS_BEGIN(device, 1)

class HikvisionNVR : public HikvisionDevice
{
public:
	HikvisionNVR(void);
	virtual ~HikvisionNVR(void);

protected:
	int login(
		const char* name = NULL, const char* password = NULL,
		const char* ip = NULL, const unsigned short port = 0,
		const bool sync = true) override;
	int logout(void) override;
	virtual int gotDigitCameraParameters(DigitCameraParameters& parameters);
};//class HikvisionNVR

NS_END

#endif//HIKVISION_NVR_H
