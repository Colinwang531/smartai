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

#include <boost/unordered_map.hpp>
#include "predef.h"

NS_BEGIN(device, 1)

class Configing
{
protected:
	Configing(void) {}
	virtual ~Configing(void){}

protected:
	virtual int getDigitCameras(
		const int userLoginID,
		boost::unordered_map<const std::string, const int>& cameras) = 0;
};//class Configing

NS_END

#endif//CONFIGING_H
