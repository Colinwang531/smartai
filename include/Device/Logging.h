//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2017-05-31
//		Description :				设备登录/注销接口
//
//		History:						Author										Date													Description
//											王科威										2017-05-31									创建
//

#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include "predef.h"

NS_BEGIN(device, 1)

class Logging
{
protected:
	Logging(void) {}
	virtual ~Logging(void){}

protected:
	//The asynchronous login status, UserID, and device information are returned by the call-back function(fLoginResultCallBack) 
	//which is configured in the structure of NET_DVR_USER_LOGIN_INFO. For asynchronous login, the returned value of -1 
	//indicates login failed, and other returned value indicates the user ID. The userID is unique, and is indispensable for operating 
	//the device.
	virtual int loginDevice(
		const std::string name, const std::string password, 
		const std::string ip, const unsigned short port = 0, 
		const bool sync = true) = 0;
	virtual int logoutDevice(void) = 0;

// 	virtual void asyncUserLoginResultNotifier(
// 		const int userID = -1, const int result = 0, void* userData = NULL) = 0;
};//class Logging

NS_END

#endif//LOGGING_H
