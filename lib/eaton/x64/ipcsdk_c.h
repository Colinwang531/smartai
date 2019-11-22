/*****************************************************************************
模块名  : ipcv7sdk功能接口
文件名  : ipcv7sdk.h
相关文件：ipcv7sdk.cpp
实现功能：
作者    ：刘登科
版本    ：IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人        走读人    修改内容
20141111      v1.0        刘登科                  创建
******************************************************************************/
#ifndef IPCSDK_H_
#define IPCSDK_H_


#include "ipcdefine.h"
#include "ipcstruct.h"


#ifdef __cplusplus
extern "C" {



#if defined(WIN32) || defined(_WIN64)
	#define IPC_API  extern "C" __declspec(dllexport)

#ifndef CDECL
	#define CDECL  __cdecl
#endif
#else
	#define IPC_API
	#define CDECL
#endif

#ifdef _LINUX_
	#ifndef NULL
	#ifdef __cplusplus
	#define NULL    0
	#else
	#define NULL    ((void *)0)
	#endif
	#endif
#endif

#endif // __cplusplus


/*=================================================================
函数名称: IPC_InitDll
功    能: 初始化SDK库资源
          全局调用一次即可，在应用程序开始处调用
		  (MFC工程在winapp中调用，动态库工程在库加载处调用)
参数说明:
			pzDLLName  初始化的动态库名称(如ipcsdk.dll,ipcmedia.dll)
			wTelnetPort      Telnet端口(默认3000)
			bOpenTelnet     是否开启Telnet(默认打开)
			pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_InitDll (char* pzDLLName = NULL/*"ipcsdk.dll"*/, short wTelnetPort=3300, bool bOpenTelnet= 0, u32* pErrorCode = NULL);

/*=================================================================
函数名称: IPC_ReleaseDll
功    能: 释放SDK的相关资源
          在应用程序退出时调用
参数说明:pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ReleaseDll(u32* pErrorCode);


/*=================================================================
函数名称: IPC_Login
功    能: 用户注册，注册信息与ip绑定，修改了ip后需要重新登录
          
参数说明:
		  pHandle   前端句柄
		  pName		用户名
		  pPassword 用户密码
		  pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32  CDECL IPC_Login(void *pHandle,char* pName, char* pPassword, u32* pErrorCode);

/*=================================================================
函数名称: IPC_Login_New(暂未启用)
功    能: 用户注册，注册信息与ip绑定，修改了ip后需要重新登录
          
参数说明:
		  pLoginInfo	登陆信息，结构体 TLOGININFO
		  IPC_CHECKLINK_CB 链路检测回调,设置为NULL及不开启链路检测，正常视频浏览建议不用开启链路检测，以减少性能消耗
		  pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32  CDECL IPC_Login_New(void* pLoginInfo,IPC_CHECKLINK_CB fCheckCB,u32* dwHandle, u32* pErrorCode);

/*=================================================================
函数名称: IPC_Logout
功    能: 用户注销注册
          
参数说明:
		  pHandle       前端句柄
		  pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Logout(void *pHandle, u32* pErrorCode);
/*=================================================================
函数名称: IPC_CheckConnectLost
功    能: 主动查询前端是否断链
参数说明:
		  dwIP		-- 设备IP 

返 回 值: 成功返回 RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_CheckConnectLost(u32 dwIp, unsigned short wPort);

/*=================================================================
函数名称: IPC_CreateHandle
功    能: 创建句柄
          
参数说明:
		 dwIp      注册IP
		 wPort     注册端口
		 pName	   用户名
		 pPassword 用户密码

返 回 值: 成功返回设备句柄, 失败返回错误码
=================================================================*/
IPC_API u32 CDECL IPC_CreateHandle(u32 dwIP, u16 wPort, char* pName, char* pPassword);
/*=================================================================
函数名称: IPC_GetInfoFromHandle
功    能: 根据句柄获取设备信息
          
参数说明:
		 dwHandle      设备句柄
		 pParam		   数据配置指针
		 nLen		   数据长度
		 pErrorCode    错误码指针

返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetInfoFromHandle(u32 dwHandle, void* pParam, int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_DestroyHandle()
功    能: 销毁句柄
          
参数说明:
		 

返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_DestroyHandle();
/*=================================================================
函数名称: IPC_GetSoftVersion
功    能: 获取软件版本号
参数说明:
          szVersion [out]    软件版本号

返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVersion(char *pchVer,int nMaxLen);

/*=================================================================
函数名称: IPC_GetCap
功    能: 获取能力集
参数说明: nCap -- 能力集个数
          apCapName -- 能力集的名称
		  anCapOut -- 能力集的返回值
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCap(void *pHandle, u16 nCap, char* apCapName[], u32 anCapOut[],u32* pErrorCode);



/*=================================================================
函数名称: IPC_GetRecSchedule
		  功    能: 获取布防时间参数
参数说明: pIndex, 布防类型是alarmin,warningline等,代表节点号
		  pParam, 布防类型detect, shieldalarm, alarmin, alarmout, exception, recchedule, warningline
	      pTimeArr, TEVENTTIME [],布防时间数组指针.
	      nlenTime, 布防时间数据个数.
	      nLenTimeStruct, 等于sizeof(TEVENTTIME)，用于后续兼容.
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<eventtype opt=” detect, shieldalarm, alarmin, alarmout, exception, recchedule, sightchange, warningline”/>
	      <recordtype opt=” alarm,manual,time,trigger”/>
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetEventTime (void *pHandle, char *pEventType, char *pIndex,void * pTimeArr, int nLenTime ,int nLenTimeStruct , char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetRecSchedule
		  功    能: 设置布防时间参数
参数说明: pIndex, 布防类型是alarmin,warningline等,代表节点号
		  pParam, 布防类型detect, shieldalarm, alarmin, alarmout, exception, recchedule, warningline
	      pTimeArr, TEVENTTIME [],布防时间数组指针.
	      nlenTime, 布防时间数据个数.
	      nLenTimeStruct, 等于sizeof(TEVENTTIME)，用于后续兼容.
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetEventTime (void *pHandle, char *pEventType,char *pIndex, void * pTimeArr, int nLenTime ,int nLenTimeStruct,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetTrancparent
		  功    能: 使用透明方式通信，传递和回传的参数为xml的结构数据
参数说明: pHandle		前端句柄
		  szEventType	事件类型
	      szParamIn		输入的参数
	      nLenIn		输入的参数长度
	      szParamOut	输出的参数
	      nLenOut		输出的参数长度
	      pErrorCode   	错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SendTransData(void *pHandle, char* szEventType, char* szParamIn, int nLenIn, char* szParamOut, int& nLenOut,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetDeviceInfo
功    能: 获取设备基本信息
参数说明: TDEVICEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDeviceInfo(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetDeviceInfo
功    能: 设置设备基本信息
参数说明: TDEVICEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDeviceInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetTestToolInfo
功    能: 设置测试工具基本信息
参数说明: TTOOLINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTestToolInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetDevSeqInfo
功    能: 设置测试工具基本信息
参数说明: TDEVSEQINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDevSeqInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetEepromInfo
功    能: 设置eeprom信息
参数说明: TEEPROMINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetEepromInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetSystemTime
功    能: 获取时间
参数说明: TSYSTIMEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSystemTime(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetSystemTime
功    能: 设置时间
参数说明: TSYSTIMEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSystemTime(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetSysTimeSynChronise
功    能: 设置时间同步
参数说明: TSYSTIMEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSysTimeSynChronise(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetTimeAuto
功    能: 设置自动校时
参数说明: TTIMEAUTOINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTimeAuto(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetTimeAuto
功    能: 获取自动校时
参数说明: TTIMEAUTOINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetTimeAuto(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetTimeAutoEx
功    能: 设置自动校时
参数说明: TTIMEAUTOINFOEX
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTimeAutoEx(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetTimeAutoEx
功    能: 获取自动校时
参数说明: TTIMEAUTOINFOEX
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetTimeAutoEx(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetDST
功    能: 设置夏令时
参数说明: TTIMEAUTOINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDST(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetDST
功    能: 获取夏令时
参数说明: TTIMEAUTOINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDST(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetRSInfo
功    能: 获取串口信息
参数说明: pParam, PTRSINFO
此处传递：<addressnum enable="false"><controlprotocol opt="pelcol"  enable="false">
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRSInfo(void *pHandle, void* ptParam, int nLen, char* szParamAssist, int nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetRSInfo
功能:设置串口停止
参数说明: pParam, PTRSINFO
此处传递：<addressnum enable="false"><controlprotocol opt="pelcol"  enable="false">
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRSInfo(void *pHandle, void* ptParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_LedFind
功能:打开关闭led灯
参数说明: pParam, PTLEDINFO;
szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
此处传递：<speed opt="5s,10s,30s,1m,30m,60m,24h"/>
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_LedFind(void *pHandle, void* ptParam, int nLen, char* szParamAssist, int nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SystemReboot
功能:系统重启
参数说明: 
		pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SystemReboot(void *pHandle,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_RecoverFactory
功能:设备恢复
参数说明: szMode, base(简单恢复，保留ip)/full(完全恢复)
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_RecoverFactory (void *pHandle, char* szMode,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_UserActive
功能:用户激活(完全恢复之后)
参数说明:
		dwIP         IP(恢复之后)
		wport        端口
		pParam       PTUSERACTIVEINFO
		pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_UserActive (u32 dwIP, u16 wPort, void*pParam,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ConfigUpload
功能:导入配置文件
参数说明: pParam, PTUPLOADINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ConfigUpload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ConfigDownload
功能:导出配置文件
参数说明: pParam, PTDOWNLOADINFO
		 pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ConfigDownload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SysUpload
功能:设备升级
参数说明: pParam, PTUPLOADINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SysUpload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
函数名称:IPC_ SysUploadState
功    能:升级状态查询
参数说明:pHandle  		前端句柄
		pParam,  		PTUPLOADSTATE;
		pErrorCode   	错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SysUploadState (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAdvanced 
功能:获取系统高级配置信息
参数说明: pParam, PTADVANCEDINFO
szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
此处传递：<osdpicfont opt=”song,black” /><mtcfmode opt=”hdr,fps60,stream4,4k,div4,normal,hdsdi enable=true”><aacmode opt=”autolow, normal, low enable=true”>
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAdvanced (void *pHandle, void *pParam, int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAdvanced 
功能:设置系统高级配置信息
参数说明: 	pParam, PTADVANCEDINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAdvanced (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetUserInfo
功    能: 获取用户信息
参数说明: pParam, 用户权限数组TUSERINFO[]
		  nLen,   数组长度
		  nLenStruct,数据长度,等于sizeof(TUSERINFO)
		  pErrorCode   错误码
		  pErrorCode   错误码
		  返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetUserInfo (void *pHandle, void *pParam, int& nLen, int& nLenStruct,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetUserInfo
功    能: 设置用户信息
参数说明: pParam, PTUSERINFO
		  nLenStruct,数据长度,等于sizeof(TUSERINFO)
		  pErrorCode   错误码
		  pErrorCode   错误码
		  返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetUserInfo (void *pHandle, void *pParam, int nLenStruct,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_AddUser
功    能: 添加用户
参数说明: pParam, PTUSERINFO
		  nLen,数据长度,等于sizeof(TUSERINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_AddUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_DelUser
功    能: 删除用户
参数说明: pParam, PTUSERDELINFO
		  nLen,数据长度,等于sizeof(TUSERDELINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_DelUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_UpdateUser
功    能: 修改用户
参数说明: pParam, PTUSERUPDATEINFO
		  nLen,数据长度,等于sizeof(TUSERUPDATEINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_UpdateUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetPowerInfo
功    能: 获取用户权限配置
参数说明: pParam, 用户权限数组TPOWERINFO[]
		  nLen, 数组长度
		  nLenStruct,数据长度,等于sizeof(TPOWERINFO).
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPowerInfo (void *pHandle, void *pParam, int nLen, int& nLenStruct,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPowerInfo
功    能: 设置用户权限配置
参数说明: pParam, PTPOWERINFO
		  nLen, 数组长度
		  nLenStruct,数据长度,等于sizeof(TPOWERINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPowerInfo (void *pHandle, void *pParam, int nLen, int nLenStruct, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetDdnsParam
功    能: 获取DDNS参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回 RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetDdns(void *pHandle,void *pParam, int nLen, char* const szParamAssist, int nLenAssist, u32 *pErrorCode);


/*=================================================================
函数名称: IPC_SetDdns
功    能: 设置DDNS参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetDdns(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetDdnsState
功    能: 获取DDNS状态参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetDdnsState(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetNetPort
功    能: 获取端口参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetNetPort(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetNetPort
功    能: 设置端口参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetNetPort(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetIpInfo
功    能: 获取IP参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetIpInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetIpInfo
功    能: 设置IP参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetIpInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetWlanInfo
功    能: 获取Wlan参数
参数说明: dwHandle      设备句柄
		  pParam [out] -- 配置数据指针TWLANINFO
		  nLen -- 数据长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWlanInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetWlanInfo
功    能: 设置Wlan参数
参数说明: dwHandle      设备句柄
		  pParam [out] -- 配置数据指针TWLANINFO
		  nLen -- 数据长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWlanInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetPPPoE
功    能: 获取PPPoE参数
参数说明: pParam [out] -- 配置数据指针TUPNPPORTMAP
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPPPoE(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetPPPoE
功    能: 设置PPPoE参数
参数说明: pParam [out] -- 配置数据指针TUPNPPORTMAP
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPPPoE(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetQos
功    能: 获取Qos参数
参数说明: pParam [out] -- 配置数据指针TQOSINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetQos(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetQos
功    能: 设置Qos参数
参数说明: pParam [out] -- 配置数据指针TQOSINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetQos(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetKSnmp
功    能: 获取KSnmp参数
参数说明: pParam [out] -- 配置数据指针TKSNMPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetKSnmp(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetKSnmp
功    能: 设置KSnmp参数
参数说明: pParam [out] -- 配置数据指针TKSNMPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetKSnmp(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_Get8021x
		  功    能: 获取8021x参数
参数说明: pParam, PT8021XINFO
	  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
	  此处传递：<protocoltype opt=“eap-tls,eap-ttls,eap-peap,eap-leap,eap-fast,eap-md5”>
				<eapolversion opt= “1, 2”>
	  返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
==================================================================*/
IPC_API int CDECL IPC_Get8021x(void *pHandle, void *pParam,  int nLen, char* const szParamAssist, int nLenAssist, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_Set8021x
功    能: 设置802.1x参数
参数说明: pParam [out] -- 配置数据指针T8021XINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_Set8021x(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetUPnP
功    能: 获取UPnP参数
参数说明: pParamInfo[out] -- TUPNPINFO
		  nlenInfo -- pParamInfo数据长度,等于sizeof(TUPNPINFO).
		  pParamMap[out] -- 端口映射表数据.
		  nlenMap -- 端口映射的数据个数TUPNPPORTMAP.
		  nLenMapStruct -- 等于sizeof(TUPNPPORTMAP)，用于后续兼容.
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetUPnP (void *pHandle, void * pInfo, int nlenInfo, void * pMap, int nLenMap, int &nLenMapStruct, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetUPnP
功    能: 设置UPnP参数
参数说明: pParamInfo[out] -- TUPNPINFO
		  nlenInfo -- pParamInfo数据长度,等于sizeof(TUPNPINFO).
		  pParamMap[out] -- 端口映射表数据.
		  nlenMap -- 端口映射的数据个数TUPNPPORTMAP.
		  nLenMapStruct -- 等于sizeof(TUPNPPORTMAP)，用于后续兼容.
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetUPnP (void *pHandle, void * pInfo, int nlenInfo, void * pMap, int nLenMap, int &nLenMapStruct, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetVsip
功    能: 获取Vsip参数
参数说明: pParam [out] -- 配置数据指针TKVISPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVsip(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetVsip
功    能: 设置Vsip参数
参数说明: pParam [out] -- 配置数据指针TKVISPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVsip(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetOnvif
功    能: 获取Onvif参数
参数说明: pParam [out] -- 配置数据指针TONVIFINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOnvif(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetOnvif
功    能: 设置Onvif参数
参数说明: pParam [out] -- 配置数据指针TONVIFINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOnvif(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetDpss
功    能: 获取Dpss参数
参数说明: dwHandle      设备句柄
		  pParam, TDPSSINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDpss(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetDpss
功    能: 设置Dpss参数
参数说明: dwHandle      设备句柄
		  pParam, TDPSSINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDpss(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetAuthMode
功    能: 获取AuthMode参数
参数说明: pParam [out] -- 配置数据指针TAUTHMODEINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAuthMode(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetAuthMode
功    能: 设置AuthMode参数
参数说明: pParam [out] -- 配置数据指针TAUTHMODEINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetAuthMode(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetGB28181
功    能: 获取GB28181参数
参数说明: pInfo, PTONVIFINFO
          byPlateid   注册平台号(1~16)
	      pEncChnArr TGBENCCHNINFO[] 编码通道数据数组指针
	      nlenEncChnInfo 编码通道数据数组个数
	      pAlarmChnArr TGBALARMCHNINFO[] 告警通道数据数组指针
	      nlenAlarmChnInfo告警通道数据数组个数
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：
		  <expand max="1024" opt="plat_domain,requriusedomain,fromtousedomain,
		  longitude,latitude,reg_retry_span,kplv_timeout_max_time,sdpfuseipc,
		  sdptimeisutc,rtcprtpdiff,alarmspan,catantfusecatarsp">
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetGB28181(void *pHandle, char*byPlateid,void *pInfo,  int nLenInfo, void *pEncChnArr, int& nLenEncChnInfo, void *pAlarmChnArr,  int& nLenAlarmChnInfo,char* const szParamAssist, int nLenAssist, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetGB28181
功    能: 设置GB28181参数
参数说明: pInfo, PTONVIFINFO
		  byPlateid   注册平台号(1~16)
	      pEncChnArr TGBENCCHNINFO[] 编码通道数据数组指针
	      nlenEncChnInfo 编码通道数据数组个数
	      pAlarmChnArr TGBALARMCHNINFO[] 告警通道数据数组指针
	      nlenAlarmChnInfo告警通道数据数组个数
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetGB28181(void *pHandle,char*byPlateid, void *pInfo,  int nLenInfo, void *pEncChnArr, int nLenEncChnInfo, void *pAlarmChnArr,  int nLenAlarmChnInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetWifiProbe
功    能: 获取wifi探针
参数说明: pParam, TWIFIPROBEINFO
		  nLen, 数据长度,等于sizeof(TWIFIPROBEINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWifiProbe (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetWifiProbe
功    能: 设置wifi探针
参数说明: pParam, TWIFIPROBEINFO
		  nLen, 数据长度,等于sizeof(TWIFIPROBEINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWifiProbe (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetWifiProbeList
功    能: 获取wifi探针设备列表
参数说明: pParam, TWIFIPROBELISTINFO
		  nLen, 数据长度,等于sizeof(TWIFIPROBELISTINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWifiProbeList (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetStorageState
		  功    能: 获取磁盘状态
参数说明: pParam, PTSTORAGESTATE
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：
		  <state opt="null,disker,diskrslowish,idel,noformat,formating,identifing">
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageState (void *pHandle,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetStorageNum
功    能: 获取磁盘个数
参数说明: 
	      nLenDisks    磁盘个数
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageNum (void *pHandle, int &nLenDisks, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetStorageManager
		  功    能: 获取存储管理参数
参数说明: pInfo, TSTORAGEMANAGER
	      nlenInfo, pInfo数据长度,等于sizeof(TSTORAGEMANAGER).
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<fullmode opt=”stop,over”/>   
		  <!-- req, xs:string --> <state opt="null,disker,diskrslowish,idel,noformat,formating,identifing">
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
 =================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageMgr  (void *pHandle, void * pInfo, int nLenInfo, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetStorageManager
		  功    能: 设置存储管理参数
参数说明: pInfo, TSTORAGEMANAGER
	      nlenInfo, pInfo数据长度,等于sizeof(TSTORAGEMANAGER).
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetStorageMgr  (void *pHandle, void * pInfo, int nLenInfo, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_DiskFomat 
功    能: 磁盘格式化
参数说明: pParam, PTSTORAGEFOMAT
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_DiskFomat  (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetFomatProgress
		  功    能: 磁盘格式化进度
参数说明: pParam, PTFOMATSTATE
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFomatProgress (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetRecSchedule
		  功    能: 获取录像计划参数
参数说明: pParam, PTRECSCHEDULE
	  nlenInfo, pParamInfo数据长度,等于sizeof(TRECSCHEDULE).
	  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<preparetime opt=”none,5,10,15,20,25,30,infinite”/>
	  <delaytime opt=”none,5,10,15,20,25,30,infinite”/>
	  <rectype opt=”main,assist,three”/><esps opt=”es, ps”/><time max= 8 index=1>
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRecSchedule (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetRecSchedule
		  功    能: 设置录像计划参数
参数说明: pParam, PTRECSCHEDULE
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRecSchedule (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetSnapPicConfig
功    能: 获取前端抓拍参数
参数说明: pParam, PTSNAPPICCONFIG
	      nlen,  pParam数据长度,等于sizeof(TSNAPPICCONFIG).
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<type opt=”jpeg, jpg,bmp”/>
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapPicCfg  (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetSnapPicConfig
功    能: 设置前端抓拍参数
参数说明: pParam, PTSNAPPICCONFIG
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapPicCfg (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetSnapTime
功    能: 获取定时抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPTIME
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapTime (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetSnapTime
功    能: 设置定时抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPTIME
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapTime (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetSnapTime
功    能: 获取事件抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPEVENT
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapEvent (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetSnapTime
功    能: 设置事件抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPEVENT
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapEvent (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetVideoShield
功    能: 获取移动侦测参数
参数说明: pParam, TDETECTINFO
	      nLen， TDETECTINFO长度， sizeof(TDETECTINFO);
	      pAreaArr, TDETECTPOINT视频遮蔽区域数组;
	      nLenArea，视频遮蔽区域数量;
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：
		  <allcolor opt="gray,red,yellow,blue,orange,green,transparent,half-transparent,mosaic">
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDetect(void *pHandle,  void *pParam,int nLen, void *pAreaArr,int nLenArea,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetVideoShield
功    能: 设置移动侦测参数
参数说明: pParam, TDETECTINFO
	      nLen， TDETECTINFO长度， sizeof(TDETECTINFO);
	      pAreaArr, TDETECTPOINT视频遮蔽区域数组;
	      nLenArea，视频遮蔽区域数量;
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDetect (void *pHandle, void *pParam,int nLen, void *pAreaArr,int nLenArea,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetShieldAlarm
功    能: 获取遮挡报警参数
参数说明: pParam,   TALARMINFO
	      nLen，    TALARMINFO长度， sizeof(TALARMINFO);
	      pAreaArr, TDETECTPOINT视频遮蔽区域数组;
	      nLenArea，视频遮蔽区域数量;
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项	
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetShieldAlarm (void *pHandle,void *pParam,int nLen, void *pAreaArr,int nLenArea,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetShieldAlarm
功    能: 设置遮挡报警参数
参数说明: pParam,   TALARMINFO
	      nLen，    TALARMINFO长度， sizeof(TALARMINFO);
	      pAreaArr, TDETECTPOINT视频遮蔽区域数组;
	      nLenArea，视频遮蔽区域数量;
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetShieldAlarm (void *pHandle,void *pParam,int nLen, void *pAreaArr,int nLenArea,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetWarningLine
功    能: 获取警戒线参数
参数说明: pParam,   TWARNINGLINEINFO
	      nLen，    TWARNINGLINEINFO长度， sizeof(TWARNINGLINEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWarningLine (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetWarningLine
功    能: 设置警戒线参数
参数说明: pParam,   TWARNINGLINEINFO
	      nLen，    TWARNINGLINEINFO长度， sizeof(TWARNINGLINEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWarningLine (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAreaDetectEnter
功    能: 获取区域入侵参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectEnter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAreaDetectEnter
功    能: 设置区域入侵参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectEnter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAreaDetectEntry
功    能: 获取区域进入参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectEntry (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAreaDetectEntry
功    能: 设置区域进入参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectEntry (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAreaDetectExit
功    能: 获取区域离开参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectExit (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAreaDetectExit
功    能: 设置区域离开参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectExit (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetObjectLeft
功    能: 获取物品遗留参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetObjectLeft (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetObjectLeft
功    能: 设置物品遗留参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetObjectLeft (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetObjectRemoval
功    能: 获取物品拿取参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetObjectRemoval (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetObjectRemoval
功    能: 设置物品拿取参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetObjectRemoval (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetPersonAggregate
功    能: 获取人员聚集参数
参数说明: pParam,      TPERSONDETECTINFO
	      nLen，       TPERSONDETECTINFO长度， sizeof(TPERSONDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPersonAggregate (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPersonAggregate
功    能: 设置人员聚集参数
参数说明: pParam,      TPERSONDETECTINFO
	      nLen，       TPERSONDETECTINFO长度， sizeof(TPERSONDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPersonAggregate (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetVirtualFocus
功    能: 获取虚焦检测参数
参数说明: pParam,      TVIRTUALFOCUSINFO
	      nLen，       TVIRTUALFOCUSINFO长度， sizeof(TVIRTUALFOCUSINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetVirtualFocus (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetVirtualFocus
功    能: 设置虚焦检测参数
参数说明: pParam,      TVIRTUALFOCUSINFO
	      nLen，       TVIRTUALFOCUSINFO长度， sizeof(TVIRTUALFOCUSINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetVirtualFocus (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetSightChange
功    能: 获取场景变更参数
参数说明: pParam,      TSIGHTCHANGEINFO
	      nLen，       TSIGHTCHANGEINFO长度， sizeof(TSIGHTCHANGEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSightChange (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetSightChange
功    能: 设置场景变更参数
参数说明: pParam,      TSIGHTCHANGEINFO
	      nLen，       TSIGHTCHANGEINFO长度， sizeof(TSIGHTCHANGEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSightChange (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAudioExcept
功    能: 获取声音异常参数
参数说明: pParam,      TAUDIOEXCEPTINFO
	      nLen，       TAUDIOEXCEPTINFO长度， sizeof(TAUDIOEXCEPTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAudioExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAudioExcept
功    能: 设置声音异常参数
参数说明: pParam,      TAUDIOEXCEPTINFO
	      nLen，       TAUDIOEXCEPTINFO长度， sizeof(TAUDIOEXCEPTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAudioExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetFaceDetect
功    能: 获取人脸检测参数
参数说明: pParam,      TFACEDETECTINFO
	      nLen，       TFACEDETECTINFO长度， sizeof(TFACEDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFaceDetect (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetFaceDetect
功    能: 设置人脸检测参数
参数说明: pParam,      TFACEDETECTINFO
	      nLen，       TFACEDETECTINFO长度， sizeof(TFACEDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFaceDetect (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAlarmIn
功    能: 获取报警输入参数
参数说明: pParam,PTALARMININFO
	      nLen， TALARMININFO长度， sizeof(TALARMININFO);
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
		  nLenAssist, 辅助信息长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAlarmIn(void *pHandle, char*pAlmNum, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAlarmIn
功    能: 设置报警输入参数
参数说明: pParam,PTALARMININFO
	      nLen， TALARMININFO长度， sizeof(TALARMININFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAlarmIn (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetAlarmOut
功    能: 获取报警输出参数
参数说明: pParam, PTALARMOUT
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAlarmOut (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetAlarmOut
功    能: 设置报警输出参数
参数说明: pParam, PTALARMOUT
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAlarmOut (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetExceptType
功    能: 获取异常类型参数
参数说明: pParam, PTEXCEPTTYPE
		  nLen， TEXCEPTTYPE长度， sizeof(TEXCEPTTYPE)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetExceptType (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetExcpet
		  功    能: 获取异常配置参数
参数说明: pExceptType  异常类型
          pParam, PTEXCEPTINFO
		  nLen， TEXCEPTINFO长度， sizeof(TEXCEPTINFO);
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
		  nLenAssist, 辅助信息长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetExcept (void *pHandle, char*pExceptType ,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetExcept
功    能: 设置异常配置参数
参数说明: pParam, PTEXCEPTINFO
		  nLen， TEXCEPTINFO长度， sizeof(TEXCEPTINFO);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_PtzMove
功    能: 设置PTZ移动(上,下,左,右,复位，停止)
参数说明: pParam,PTPTZMOVE
		  nLen， TPTZMOVE长度， sizeof(TPTZMOVE);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzMove (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_PtzScan
功    能: 设置PTZ水平巡航
参数说明: pParam,PTPTZSCAN
		  nLen， TPTZSCAN长度， sizeof(TPTZSCAN);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzScan (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_PtzZoneZoom
功    能: 设置PTZ区域缩放
参数说明: pParam,PTPTZZONEZOOM
		  nLen， TPTZZONEZOOM长度， sizeof(TPTZZONEZOOM);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzZoneZoom (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_PtzGotoPoint
功    能: 设置PTZ居中定位
参数说明: pParam,TPTZGOTOPOINT
		  nLen， TPTZGOTOPOINT长度， sizeof(TPTZGOTOPOINT);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzGotoPoint (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzWiper
功    能: 设置雨刷开启/关闭
参数说明: pParam,TPTZWIPER
		  nLen， TPTZWIPER长度， sizeof(TPTZWIPER);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzWiper (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetPtzBase
功    能: 获取云台基本参数
参数说明: 
	      pParam, PTPTZBASE
		  nLen， TPTZBASE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzBase (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ SetPtzBase
功    能: 设置云台基本参数
参数说明: pParam, PTPTZBASE
	      nLen，TPTZBASE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzBase (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ IPC_GetPtzMainTain
功    能: 获取云台断电维护参数
参数说明: 
	      pParam, PTPTZMAINTAIN
		  nLen， TPTZMAINTAIN的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzMainTain (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ IPC_SetPtzMainTain
功    能: 设置云台断电维护参数
参数说明: pParam, PTPTZMAINTAIN
	      nLen，TPTZMAINTAIN的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzMainTain (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ IPC_SetPtzRefactory
功    能: 设置云台恢复出厂设置
参数说明: pParam, PTPTZREFACTORY
	      nLen，TPTZREFACTORY的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzRefactory (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetPtzWatchOn
功    能: 获取云台守望任务参数
参数说明: 
		  pParam, PTPTZWATCHON
		  nLen， TPTZWATCHON的长度
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<mode opt=”hscan,vscan,preset,pathcruise,framescan,
		  randscan,fullviewscan,syncscan”>
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzWatchOn (void *pHandle, void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ SetPtzWatchOn
功    能: 设置云台守望任务参数
参数说明: pParam, PTPTZWATCHON
	      nLen，TPTZWATCHON的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzWatchOn (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetPtzPreset
功    能: 获取预置位参数
参数说明: 
	      bySn,需要获取的预置位的序号，1~255
		  pParam, PTPTZPRESET
		  nLen，bySn为0 返回TPTZPRESET数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPreset (void *pHandle, u8 bySn, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetPtzPresetAll
功    能: 获取全部预置位参数
参数说明:
		  pParam, PTPTZPRESETALL
		  nLen返回TPTZPRESET数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPresetAll (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetPtzPreset
功    能: 设置预置位参数
参数说明: pParam, PTPTZPRESET
	  返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPreset (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetPtzLimit
功    能: 获取云台限位任务参数
参数说明: 
		  pParam, PTPTZLIMIT
		  nLen， TPTZLIMIT的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzLimit (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ SetPtzLimit
功    能: 设置云台限位任务参数
参数说明: pParam, PTPTZLIMIT
	  nLen，TPTZLIMIT的长度
	  返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzLimit (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetPtzPathCruise
功    能: 获取巡航扫描参数
参数说明: 
	      bySn,需要获取的扫描路径的序号，1~8, 0代表获取所有信息
		  pParam, PTPTZPATHCRUISE
		  nLen，bySn为0 返回TPTZPATHCRUISE数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPathCruise (void *pHandle, u8 bySn, void *pParam,int& nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzPathCruise
功    能: 设置巡航扫描参数
参数说明: pParam, TPTZPATHCRUISE[]，扫描路径信息数组
	      nLen，TPTZPATHCRUISE数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPathCruise (void *pHandle, u8 bySn,void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzPathCruiseEvent
功    能: 设置巡航扫描事件(调用巡航,停止巡航)
参数说明: pParam, TPTZPATHCRUISEEVT，巡航扫描事件
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPathCruiseEvent (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetTimeTask
功    能: 获取定时任务参数
参数说明: 
	      pParam, PTTIMETASK
		  nLen，sizeof(TTIMETASK);
	      pTimeArr, PTTIMETASKLIST
		  nlenTime,定时任务时间数据个数.
		  nLenTimeStruct, 等于sizeof(TTIMETASKLIST)，用于后续兼容.
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<mode opt=”close,hscan,vscan,preset,pathcruise,framescan,randscan,
		  fullviewscan,syncscan”>
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetTimeTask (void *pHandle, void *pParam, int nLen, void * pTimeArr, int nLenTime ,int nLenTimeStruct , char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ SetTimeTask
功    能: 设置定时任务参数
参数说明: 
	      pParam, PTTIMETASK
		  nLen，sizeof(TTIMETASK);
	      pTimeArr, PTTIMETASKLIST
		  nlenTime,定时任务时间数据个数.
		  nLenTimeStruct, 等于sizeof(TTIMETASKLIST)，用于后续兼容.
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetTimeTask (void *pHandle, void *pParam, int nLen ,void * pTimeArr, int nLenTime ,int nLenTimeStruct,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_ GetPtzMainTain
功    能: 获取云台坐标参数
参数说明: 
	      pParam, PTPTZCOORDINATE
		  nLen， TPTZCOORDINATE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzCoorDinate (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzCoorDinateEvt
功    能: 设置云台坐标事件
参数说明: 
	      pParam, PTPTZCOORDINATE
		  nLen， TPTZCOORDINATE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzCoorDinateEvt(void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetPtzRatio
功    能: 获取云台倍率
参数说明: 
	      dwHandle      设备句柄
		  pParam, TPTZRATIO
		  nLen， TPTZRATIO的长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPtzRatio (void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzRatio
功    能: 设置云台倍率
参数说明: 
	      dwHandle      设备句柄
		  pParam, TPTZRATIO
		  nLen， TPTZRATIO的长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPtzRatio(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzCoorDinateEvt
功    能: 设置云台P,T,Z值
参数说明: 
	      dwHandle      设备句柄
		  pParam, PTPTZCOORDINATEEX
		  nLen， TPTZCOORDINATEEX的长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPtzCoorDinateEX(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_SetAquillaPTMove
功    能: 设置安奎拉延迟定位
参数说明: 
	      dwHandle      设备句柄
		  pParam, TPTZAQUILLAPTMOVE
		  nLen， TPTZAQUILLAPTMOVE的长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAquillaPTMove(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_SetAquillaPTZAngle
功    能: 设置安奎拉定位
参数说明: 
	      dwHandle      设备句柄
		  pParam, TPTZAQUILLAPTZANGLE
		  nLen， TPTZAQUILLAPTZANGLE的长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAquillaPTZAngle(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_PtzDeviationCheck
功    能: 云台误差校验
参数说明: 
	      dwHandle      设备句柄
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_PtzDeviationCheck(void* pHandle, u32* pErrorCode);
/*=================================================================
函数名称: IPC_GetPtzDevCheckRlt
功    能: 获取云台误差校验结果
参数说明: 
	      dwHandle      设备句柄
		  pParam,		TPTZDEVCHECKRLT
		  nLen，        TPTZDEVCHECKRLT的长度
		  pErrorCode    错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPtzDevCheckRlt(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_GetPtzPatternsInfo
功    能: 实现花样扫描信息获取
参数说明: pParam, PTPTZPATTERNSINFO
		  nLen,   花样扫描链表个数
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPatternsInfo (void *pHandle, void *pParam,  int &nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzPatternEvt
功    能: 设置花样扫描事件
参数说明: pParam, PTPTZPATTERNEVT
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPatternEvt (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_GetPtzHvangle
功    能: 获取视场角数据
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzHVangle (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetPtzWSInfo
功    能: 设置websocket信息给设备，用来上传ptz信息
参数说明: 
	      dwHandle      设备句柄
		  pParam,		TPTZWSINFO
		  nLen，        TPTZWSINFO长度
		  pErrorCode    错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPtzWSInfo(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_AddConnectDetect
功    能: 增加链路检测,一个ip只有一个检测
参数说明:
		  dwIP		-- 设备IP 
		  wPort		-- 设备http端口 
		  dwConnectTimeOut	-- 检测超时时间
		  dwReConnectTimes -- 断链重连次数
		  pcbfFun	-- 回调函数
          pContext	-- 上下文
返 回 值: 成功返回 RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_AddConnectDetect(void *pHandle, u32 dwConnectTimeOut, u32 dwReConnectTimes, cbfConnectDetect pcbfFun, void* pContext,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_DelConnectDetect
功    能: 删除链路检测,一个ip只有一个检测
参数说明:
		  dwIP		-- 设备IP 

返 回 值: 成功返回 RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_DelConnectDetect(void *pHandle,u32 *pErrorCode);

IPC_API BOOL32 CDECL IPC_CheckConnectLost(u32 dwIp, u16 wPort);

/*=================================================================
函数名称: IPC_IsConnect
功    能: 连接状态判断
          
参数说明:
		 dwHandle      设备句柄
		 bIsConnect	   连接状态
		 pErrorCode    错误码指针

返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_IsConnect(u32 dwHandle, bool& bIsConnect, u32* pErrorCode);
/*=================================================================
函数名称: IPC_GetVideoInfo
功    能: 获取rtp码流
参数说明: pParam [in]       输入结构体参数
		  nLen [in]		    输入结构体长度
		  pInfoOut [out]	输出结构体参数
		  nLenInfo [out]	输出结构体长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetVideoInfo(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode);

/*=================================================================
函数名称: IPC_GetRtspURL
功    能: 获取rtp码流,带告警元数据
参数说明: pParam [in]       输入结构体参数
		  nLen [in]		    输入结构体长度
		  pInfoOut [out]	输出结构体参数
		  nLenInfo [out]	输出结构体长度
		  bNoStream			FALSE申请rtsp码流码流,不申请rtsp码流，只申请rtsp的链路来传送告警数据
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRtspUrl(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode, int bNoStream = FALSE);


/*=================================================================
函数名称: IPC_GetRtspURLRec
功    能: 获取录像的url
参数说明: pParam [in]       输入结构体参数
		  nLen [in]		    输入结构体长度
		  pInfoOut [out]	输出结构体参数
		  nLenInfo [out]	输出结构体长度
		  bNoStream			TRUE不申请tcp码流，FALSE申请tcl码流
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRtspUrlRec(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode);


/*=================================================================
函数名称: IPC_StartVideo
功    能: 浏览请求参数(TCP,UDP)
参数说明: eType  码流传输类型 type_udp,type_tcp
		  pParam[in]--TVIDEOPARAM
		  nLen --输入结构体长度
		  pInfoOut[out]--TPLAYVIDEOINFO
		  nLenInfo[out] --输出结构体
		  pErrorCode   错误码
返 回 值: UDP:成功播放id,失败返回0，原因解析pErrorCode,TCP:成功播放返回id,失败依据pErrorCode是否为RET_FAILD
=================================================================*/
IPC_API u32 CDECL IPC_StartRealPlay(void *pHandle, emPlayVideoType eType,void* pParam, int nLen,void*pInfoOut,int& nLenInfo,u32* pErrorCode);

/*=================================================================
函数名称: IPC_SetRtspMetaPort
功    能: 设置Rtsp Meta端口
参数说明: eType  码流传输类型 type_udp,type_tcp
		  bVideo	是否存在视频
		  pParam[in]--TPLAYVIDEOPARAM
		  nLen --输入结构体长度	
		  pInfoOut[out]--TPLAYVIDEOINFO
		  nLenInfo[out] --输出结构体
		  pErrorCode   错误码
返 回 值: 成功播放id,失败返回0，原因解析pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_SetRtspMetaPort(void *pHandle, emPlayVideoType eType, bool bVideo, void* pParam, int nLen,void*pInfoOut,int& nLenInfo,u32* pErrorCode);


/*=================================================================
函数名称: IPC_RemoveRtspMetaPort
功    能: 去除Rtsp Meta端口
参数说明: 
		  dwPlayID      播放ID
		  pErrorCode    错误码
返 回 值: 成功播放id,失败返回0，原因解析pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_RemoveRtspMetaPort(void *pHandle, u32 dwPlayID, u32* pErrorCode);

/*=================================================================
函数名称: IPC_StopVideo
功    能: 停止浏览请求参数(TCP)
参数说明: eType         码流传输类型 type_udp,type_tcp
		  dwPlayID      播放ID
		  pErrorCode    错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopRealPlay(void *pHandle,emPlayVideoType eType,u32 dwPlayID, u32* pErrorCode);
/*=================================================================
函数名称: IPC_SendKeyFrame
功    能: 请求关键帧
参数说明: dwPlayID[in]--TRTCPVIDEOINFO中dwPlayID 播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SendKeyFrame(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
函数名称: IPC_GetVideoVolume_RTCP
功    能: 获取视频音量
参数说明: pInfoOut[out]--TRTCPVIDEOVOLUMEINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_GetVideoVolume_RTCP(void *pHandle, void *pInfoOut, int nLenInfo, u32* pErrorCode);
/*=================================================================
函数名称: IPC_SetVideoVolume_RTCP
功    能: 设置视频音量
参数说明: pParam[in]--TRTCPVIDEOVOLUMEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_SetVideoVolume_RTCP(void *pHandle, void* pParam, int nLen, u32* pErrorCode);
/*=================================================================
函数名称: IPC_VoiceCallState
功    能: 是否开启呼叫
参数说明: pInfoOut [out] TRTCPCALLSTATEINFO
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_VoiceCallState (void *pHandle, void* pInfoOut, int nOutInfo, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_StartVoiceCall
功    能: 开始呼叫
参数说明: pHandle		前端句柄
		  emTransType   编码传输格式
		  pInfoOut [out] TRTCPPREPARECALLINFO
		  pErrorCode   	错误码
返 回 值: 成功返回呼叫ID, 失败返回0，原因解析pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_StartVoiceCall(void *pHandle,emTransType eTranstype, void*pInfoOut,int& nLenInfo,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_StopVoiceCall
功    能: 停止呼叫
参数说明: dwPlayID[in]--TRTCPPREPARECALLINFO中dwID
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopVoiceCall(void *pHandle, u32 dwVoiceCallId, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_PuSnapShot
功    能: 前端手动抓拍
参数说明: 
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),四码流(4)
		  pchFileName     保存的图片文件全路径(文件路径),后缀为jpeg
		  pErrorCode      错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_PuSnapShot(void *pHandle, u8 byStreamId, const char *pchFileName, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetPuSnapShotUrl
功    能: 前端手动抓拍
参数说明: 
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),四码流(4)
		  pImageUrl     图片url
		  pErrorCode      错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPuSnapShotUrl(void *pHandle, u8 byStreamId, char *pImageUrl, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetMultiVideoMode
功    能: 获取多码流模式参数
参数说明: pInfoOut[in]--TMULTIVIDEOMODEINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetMultiVideoMode(void *pHandle,u8 byVideoSource, void *pInfoOut, int &nLenInfo, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetMultiVideoMode
功    能: 设置多码流模式参数
参数说明: pParam[in]--TMULTIVIDEOMODEPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetMultiVideoMode(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetVideoEnc
功    能: 获取视频编码参数
参数说明: pInfoOut[in]--TVIDEOENCINFO
		  byVideoSource[in]--视频源ID
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),死码流(4)
		  szAssistInfo 辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoEnc(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetVideoEnc
功    能: 设置视频编码参数
参数说明: pParam[in]--TVIDEOENCPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoEnc(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetAudioEnc
功    能: 获取音频编码参数
参数说明: pInfoOut[in]--TAUDIOENCINFO
		  byVideoSource[in]--视频源ID
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),死码流(4)
		  szAssistInfo 辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAudioEnc(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetAudioEnc
功    能: 设置音频编码参数
参数说明: pParam[in]--TAUDIOENCPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetAudioEnc(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

IPC_API int CDECL IPC_SetAudioEncExt(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetAudioDec
功    能: 获取音频解码参数
参数说明: pInfoOut[in]--TAUDIODECINFO
		  byVideoSource[in]--视频源ID
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),死码流(4)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAudioDec(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetAudioDec
功    能: 设置音频解码参数
参数说明: pParam[in]--TAUDIODECPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetAudioDec(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetVideoShield
功    能: 获取图像遮蔽
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TVIDEOSHIELDINFO
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoShield(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo,  char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetVideoShield
功    能: 设置图像遮蔽
参数说明: pParam[in] --TVIDEOSHIELDPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoShield(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetVideoRoi
功    能: 获取敏感区域编码
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TVIDEOROIINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoRoi(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetVideoRoi
功    能: 设置敏感区域编码
参数说明: pParam[in] --TVIDEOROIPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoRoi(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetVideoEncCut
功    能: 获取编码裁剪
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TVIDEOENCCUTINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoEncCut(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetCheckPointOsd
功    能: 获取 check point osd 信息
参数说明: 
		  dwHandle      设备句柄
		  byVideoSource[in] --视频源ID
		  pInfoOut[out] --TCHECKPOINTINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCheckPointOsd(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32* pErrorCode);

/*=================================================================
函数名称: IPC_SetCheckPointOsd
功    能: 设置check point osd
参数说明: pParam[in] --TCHECKPOINTPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetCheckPointOsd(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetUavTrack
功    能: 设置智能跟踪开关
参数说明: pParam[in] --enable
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetUavTrack(void *pHandle, bool pParam, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetUavTrack
功    能: 获取智能跟踪开关
参数说明: pParam[in] --enable
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetUavTrack(void *pHandle, bool& pParam, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetEventManualSnap
功    能: 获取 事件抓拍信息
参数说明: pParam[in] --TMANUALSNAPINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetEventManualSnap(void* pHandle,void* pInfoOut, int &nLenInfo, u32* pErrorCode);

/*=================================================================
函数名称: IPC_SetEventManualSnap
功    能: 设置事件抓拍信息
参数说明: pParam[in] --TMANUALSNAPINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetEventManualSnap(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetVideoEncCut
功    能: 设置编码裁剪
参数说明: pParam[in] --TVIDEOENCCUTPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoEncCut(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetOsdCap
功    能: 获取OSD参数能力支持
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TOSDCAPINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsdCap(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetOsd
功    能: 获取OSD全局属性参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TOSDINFO
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsd(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetOsd
功    能: 设置OSD全局属性参数
参数说明: pParam[in] --TOSDPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsd(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetOsdList
功    能: 设置单个OSD属性参数
参数说明: pParam TOSDLISTINFO[] OSD属性数组指针
	      nLen   OSD属性数组个数
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsdList(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetSmartOsdInfo
功    能: 设置烟火报警信息
参数说明:  pParam[in] --TOSDSMARTPARAM
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetSmartOsdInfo(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetOsdList
功    能: 获取单个OSD属性参数
参数说明: pInfoOut TOSDLISTINFO[] OSD属性数组指针
	      nLenInfo OSD属性数组个数
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsdList(void *pHandle, u8 byVideoSource,void *pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetSmartOsdInfo
功    能: 获取烟火报警信息
参数说明:byVideoSource[in] --视频源ID
		 pInfoOut[out] --TOSDSMARTINFO
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetSmartOsdInfo(void *pHandle, u8 byVideoSource,void *pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetOsdStateQuery
功    能: Osd图片上传状态查询
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TOSDSTATEINFO
		  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsdStateQuery(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetOsdPrepare
功    能: Osd图片准备上传
参数说明: pParam[in] --TOSDPREPAREPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsdPrepare(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetOsdUploadFile
功    能: Osd图片上传
参数说明: pParam[in] --TOSDUPLOADFILEPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsdUploadFile(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_RecoveryDef
功    能: 图像恢复默认
参数说明: pParam[in] --TIMGDEFPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_Img_Def(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetColor
功    能: 获取图像常规设置
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TCOLORINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetColor(void *pHandle, u8 byVideoSource, void* pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetColor
功    能: 设置图像常规设置
参数说明: pParam[in] --TCOLORPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetColor(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetFocus
功    能: 获取聚焦参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TFOCUSINFO
		  szParamAssist[out] --聚焦可选参数,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)	  
		  pErrorCode   错误码		  
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFocus(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetFocus
功    能: 设置聚焦参数
参数说明: pParam[in] --TFOCUSPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFocus (void *pHandle, void* pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetWhiteBlance
功    能: 获取白平衡参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TWHITEBLANCEINFO
		  szParamAssist[out] --白平衡可选参数,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)		  
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWhiteBlance (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetWhiteBlance
功    能: 设置白平衡参数
参数说明: pParam[in] --TWHITEBLANCEPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWhiteBlance (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetIrcutFilter
功    能: 获取日夜模式参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TRCUTFILTERINFO
		  szParamAssist[out] --日夜模式可选参数,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)	  
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetIrcutfilter (void *pHandle, u8 byVideoSource,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetIrcutFilter
功    能: 设置日夜模式参数
参数说明: pParam[in] --TTRCUTFILTERPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetIrcutfilter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetIris
功    能: 获取光圈参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TIRISINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetIris (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetIris
功    能: 设置光圈参数
参数说明: pParam[in] --TIRISPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetIris (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetShutter
功    能: 获取快门参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TSHUTTERINFO
		  szParamAssist[out] --快门可选参数,辅助信息以xml片段的方式传递enable选项和opt选项(建议长度512以上)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetShutter(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetSutter
功    能: 设置快门参数
参数说明: pParam[in] --TSHUTTERPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetShutter (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetGain
功    能: 获取增益参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TGAININFO
		  pParam   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGain(void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetGain
功    能: 设置增益参数
参数说明: pParam[in] --TGAINPARAM
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetGain (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetInfrared
功    能: 获取红外参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TINFRAREDINFO
		  szParamAssist[out] -- 红外光启用模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<contrlmode opt=”close,auto,farlamp,middlelamp,nearlamp,exnearlamp”/>
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetInfrared (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetInfrared
功    能: 设置红外参数
参数说明: pParam[in] --TINFRAREDPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetInfrared (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetLaser
功    能: 获取激光参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TLASERINFO
		  szParamAssist[out] --激光聚光模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<spotmode opt=”def,littlefacula,bigfaxula,custom1,custom2”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetLaser (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetLaser
功    能: 设置激光参数
参数说明: pParam[in] --TLASERPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetLaser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetCorrido
功    能: 获取走廊参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TCORRIDOINFO
		  szParamAssist[out] --走廊模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<spotmode opt=”close, left,right”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCorridorMode (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetCorrido
功    能: 设置走廊参数
参数说明: pParam[in] --TCORRIDOPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetCorridorMode (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetImageMode
功    能: 获取镜像模式参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TIMAGEMODEINFO
		  szAssistInfo[out] --镜像模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递:<spotmode opt=”close, horizont, vertical, central”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetImageMode (void *pHandle, u8 byVideoSource, void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
函数名称: IPC_SetImageMode
功    能: 设置镜像参数
参数说明: pParam[in] --TIMAGEMODEPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetImageMode (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetNoiseRedece2D
功    能: 获取2D降噪参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TNOISEREDUCE2DINFO
		  szAssistInfo[out] --2D模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<spotmode opt=”auto,open,close”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Get2DDenoise (void *pHandle, u8 byVideoSource, void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetNoiseRedece2D
功    能: 设置2D降噪参数
参数说明: pParam[in] --TNOISEREDUCE2DPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Set2DDenoise (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetNoiseRedece3D
功    能: 获取3D降噪参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TNOISEREDUCE3DINFO
		  szAssistInfo[out] --3D模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<spotmode opt=”auto,open,close”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Get3DDenoise(void *pHandle, u8 byVideoSource,void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);


/*=================================================================
函数名称: IPC_SetNoiseRedece3D
功    能: 设置3D降噪参数
参数说明: pParam[in] --TNOISEREDUCE3DPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Set3DDenoise (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetLocalEcho
功    能: 获取本地回显参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TLOCALECHOINFO
		  szAssistInfo[out] --模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<spotmode opt=”close, PAL,NTSC,LINE,INTERLEAVE”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetLocalEcho (void *pHandle, u8 byVideoSource, void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetLocalEcho
功    能: 设置本地回显参数
参数说明: pParam[in] --TLOCALECHOPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetLocalEcho (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetPowerLineFrequency
功    能: 获取防闪烁参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TPOWERLINEFREQUENCYINFO
		  szAssistInfo[out] --模式,辅助信息以xml片段的方式传递enable选项和opt选项
		  此处传递：<spotmode opt=”50hz,60hz,auto”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPowerLineFrequency (void *pHandle, u8 byVideoSource,void *pParam, int nLen,  char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetPowerLineFrequency
功    能: 设置防闪烁参数
参数说明: pParam[in] --TPOWERLINEFREQUENCYPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPowerLineFrequency (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetWDR
功    能: 获取宽动态参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWDR(void *pHandle, u8 byVideoSource, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetWDR
功    能: 设置宽动态参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为wdr 只需设置nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWDR (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetHLC
功    能: 获取强光抑制参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetHLC(void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetHLC
功    能: 设置强光抑制参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为hlc 只需设置nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetHLC (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetFogThrough
功    能: 获取透雾参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFogThrough (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetFogThrough
功    能: 设置透雾参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为fogthrough或者close 只需设置nLevel有效
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFogThrough (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetGamma
功    能: 获取Gamma参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGamma (void *pHandle, u8 byVideoSource, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetGamma
功    能: 设置Gamma参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为gamma或close 只需设置nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetGamma (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetSmartIR
功    能: 获取SmartIR参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSmartIR (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetSmartIR
功    能: 设置SmartIR参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为smartir或close 只需设置nLevel有效
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSmartIR (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetBLC
功    能: 获取背光补偿参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TDYNAMICMODEINFO 只有nLevel有效
		  szParamAssist 辅助信息以xml片段的方式传递enable选项和opt选项
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBLC(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetBLC
功    能: 设置背光补偿参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为autoblc,regionblc或close 只需设置nLevel有效
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetBLC (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetFillLight
功    能: 获取红外补光灯
参数说明: 
		  pParam[out] --TDFILLLIGHT
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFillLight (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetFillLight
功    能: 设置红外补光灯
参数说明: pParam[in] --TDFILLLIGHT
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFillLight (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetDynPluginState
功    能: 设置动态插件状态
参数说明: pParam[in] --TDDYNMAICPLUGIN
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDynPluginState (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetRecMonthInfo
功    能:	获取月视图
参数说明:	pHandle		前端句柄
		    pErrorCode   	错误码
			nYear       年
		    nMonth		月份 
		    pDataOut, 		Array of TRECMONTHINFO;
	        nArrayLenOut, 	数组长度;
返 回 值: 成功返回true, 失败返回false, 原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRecMonthInfo(void *pHandle, int nYear, int nMonth, void *pDataOut, int& nArrayLenOut ,u32* pErrorCode);
/*=================================================================
函数名称: IPC_GetRecordData
功    能:	获取录像数据(同步)
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			u32 dwRecId		录像id，id == 0 表示同步，id  != 0 需要在准备获取的接口里创建ID。
			pParam 		    PTRECPARAM;
			pDataOut		Array of PTRECDATA;
			nArrLenOut		录像数据数组长度,输入的时候，输入最大的长度
返 回 值: 成功返回获取的录像数据个数, 失败返回0, 原因解析pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_GetRecordData(void *pHandle, u32 dwRecId, void *pParam, void *pDataOut,int& nArrLenOut,u32* pErrorCode);
/*=================================================================
函数名称: IPC_GetRecordNext
功    能:	获取录像数据(异步)
参数说明:	pHandle		前端句柄
		    pErrorCode   	错误码
		    u32 dwTaskId		录像id，id  != 0 需要在准备获取的接口里创建ID。
		    dwFirstIndex	开始获取的起始索引位置
		    pDataOut		Array of PTRECDATA;
	        nArrLenOut		录像数据数组长度
返 回 值: 成功返回获取的录像数据个数, 失败返回0, 原因解析pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_GetRecordNext (void *pHandle, u32 dwTaskId, u32 dwFirstIndex,bool &bFinished,void *pDataOut,int nArrLenOut,u32* pErrorCode);
/*=================================================================
函数名称: IPC_PreLoadRecordList
功    能:	准备录像数据
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			pParam 		TRECPARAM
			nParamLen		结构体的长度
返 回 值: 成功返回查询ID, 失败返回0, 原因解析pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_PreLoadRecordList(void *pHandle, void *pParam, int nParamLen =0, u32* pErrorCode=0);
/*=================================================================
函数名称: 	IPC_StartRecordPlay
功    能:		开始放像
参数说明:		pHandle		前端句柄
				pErrorCode   	错误码
				pParam		PTRECSTARTPLAYPARAM
				nParamLen	放像参数的长度
				pDataOut		PTRECSTARTPLAYINFO
				nDataLen		放像回传数据长度
				bDec			是否解码
返 回 值: 	成功返回播放id, 失败返回0;
=================================================================*/
IPC_API u32 CDECL IPC_StartRecordPlay(void *pHandle, void *pParam, int nParamLen,void *pDataOut, int nDataLen,bool bDec, bool bNoStream, u32* pErrorCode);

/*=================================================================
函数名称: 	IPC_StopRecordPlay
功    能:	停止放像
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			dwPlayID		播放ID
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
函数名称: 	IPC_PauseRecordPlay
功    能:	暂停放像
参数说明:	pHandle		前端句柄
			pErrorCode  错误码
			dwPlayID	播放ID
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_PauseRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
函数名称: 	IPC_ResumeRecordPlay
功    能:	恢复放像
参数说明:	pHandle		前端句柄
			pErrorCode  错误码
			dwPlayID	播放ID
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_ResumeRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
函数名称: 	IPC_MoveRecordPlay
功    能:	播放位置移动
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			dwPlayID		播放ID
			pParam		PTTIMEEX
			nParamLen		参数长度
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_MoveRecordPlay(void *pHandle, u32 dwPlayID, void *pParam, int nParamLen,u32* pErrorCode);
/*=================================================================
函数名称: 	IPC_SetRecordPlayRate
功    能:	播放速度控制
参数说明:	pHandle			前端句柄
			pErrorCode   	错误码
			dwPlayID		播放ID
			dwPlayRate		emRecPlayRate
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRecordPlayRate(void *pHandle, u32 dwPlayID,u32 dwPlayRate, u32* pErrorCode);
/*=================================================================
函数名称: 	IPC_ Getrecplaystate
功    能:	获取放像进度
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			dwPlayID		播放ID
			pDataOut		PTRECPLAYSTATEINFO
			nDataOut          TRECPLAYSTATEINFO大小
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_Getrecplaystate (void *pHandle, u32 dwPlayID,void* pDataOut, int &nDataOut,u32* pErrorCode);


/*=================================================================
函数名称: 	IPC_Setrecdownloadspeed
功    能:	设置录像下载速度
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_Setrecdownloadspeed (void *pHandle,u32* pErrorCode);

/*=================================================================
函数名称: IPC_GetCompileTime
功    能: 获取ipcsdk库编译时间
          
参数说明:
		  szCompileTime sdk库编译时间
		  pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCompileTime(char* szCompileTime,u32*pErrorCode);

/*=================================================================
函数名称: IPC_AssTeleZoomInfo
功    能: 远端放大
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_AssTeleZoomInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetAssTeleZoomInfo
功    能: 远端放大
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAssTeleZoomInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
函数名称: IPC_AssMalfInfo
功    能: 畸形矫正
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_AssMalfInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetAssMalfInfo
功    能: 畸形矫正
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAssMalfInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetMTCF
功    能: 设置工作模式
参数说明: pParam, TMTCFPARAM
		  nLen, 数据长度,等于sizeof(TMTCFPARAM)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetMTCF (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetMTCF
功    能: 获取工作模式
参数说明: pParam, TMTCFPARAM
		  nLen, 数据长度,等于sizeof(TMTCFPARAM)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetMTCF (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetBatteryStatus
功    能: 获取电池电量
参数说明: pParam, TBATSTATUSINFO
		  nLen, 数据长度,等于sizeof(TBATSTATUSINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBatteryStatus (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetSSH
功    能: 设置SSH开启/关闭
参数说明: bOpen,       是否开启ssh
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSSH (void *pHandle, bool bOpen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetColorSize
功    能: 设置调色板
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetColorSize (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetColorSize
功    能: 获取调色板
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetColorSize (void *pHandle, void *pParam,  int nLen, char* szParamAssist, int& nLenAssist, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SethotPointState
功    能: 设置热点追踪状态
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SethotPointState (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GethotPointState
功    能: 获取热点追踪状态
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GethotPointState (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetGpsInfo
功    能: 获取GPS信息
参数说明: pParam, TPOSITIONINFO  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGpsInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetMobilenetwork
功    能: 获取4g参数
参数说明: dwHandle      设备句柄
		  pParam, TMOBILENETWORK
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetMobilenetwork(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetMobilenetwork
功    能: 设置4g参数
参数说明: dwHandle      设备句柄
		  pParam, TMOBILENETWORK
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetMobilenetwork(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetPubsec
功    能: 获取视图库参数
参数说明: dwHandle      设备句柄
		  pParam, TPUBSECPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPubsec(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetPubsec
功    能: 设置视图库参数
参数说明: dwHandle      设备句柄
		  pParam, TPUBSECPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPubsec(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetAzimuth
功    能: 设置方位角
参数说明: NA 
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAzimuth (void *pHandle, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_GetBasicPos
功    能: 获取基于虚拟零位的当前云台角度
参数说明: dwHandle      设备句柄
		  pParam, TBASICPOSINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBasicPos (void *pHandle,void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetDeployPicStart
功    能: 准备上传图片
参数说明: param totalnum 图片数目 最大200张
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDeployPicStart(void *pHandle, int nParam, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetDeployPicUpload
功    能: 上传图片
参数说明: pParam[in] --TDEPLOYUPLOADPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDeployPicUpload(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
函数名称: IPC_SetDeployPicStop
功    能: 上传图片
参数说明: pParam[in] --NA
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDeployPicStop(void *pHandle, u32 *pErrorCode);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif //
