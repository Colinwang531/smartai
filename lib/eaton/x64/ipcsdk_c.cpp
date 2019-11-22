/*****************************************************************************
模块名  : ipcsdk开发包接口模块
文件名  : ipcsdk.cpp
相关文件: 
实现功能: 动态加载dll接口
作者    : 刘登科
版本    : KDM-V5.0-R1.0 Copyright (C) 2008 - All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期            版本      修改人        走读人      修改内容
20141117          v1.0      刘登科                    创建
******************************************************************************/
#if defined(WIN32) || defined(_WIN64)
#include <Windows.h>
#else
	#include <dlfcn.h>
	#include <stdio.h>
	#include <stdlib.h>
#endif
#include "ipcsdk_c.h"

#include <map>
#include <string.h>
#include <iterator>

// 库句柄
#if defined(WIN32) || defined(_WIN64)
HINSTANCE m_hSDKDll = NULL;
#define  PROCADDR GetProcAddress
#else
void* m_hSDKDll = NULL;
#define PROCADDR dlsym
#endif


// 载入dll函数接口的宏，无参
#define LOADDLLFUN(btrue, fun, funname);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun();\
	else\
	pFun();\
}\
}

// 载入dll函数接口的宏，参1
#define LOADDLLFUN1(btrue, fun, funname, p1);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1);\
	else\
	pFun(p1);\
}\
}

// 载入dll函数接口的宏，参2
#define LOADDLLFUN2(btrue, fun, funname, p1, p2);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2);\
	else\
	pFun(p1, p2);\
}\
}


// 载入dll函数接口的宏，参3
#define LOADDLLFUN3(btrue, fun, funname, p1, p2, p3);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3);\
	else\
	pFun(p1, p2, p3);\
}\
}

// 载入dll函数接口的宏，参4
#define LOADDLLFUN4(btrue, fun, funname, p1, p2, p3,p4);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4);\
	else\
	pFun(p1, p2, p3,p4);\
}\
}

// 载入dll函数接口的宏，参5
#define LOADDLLFUN5(btrue, fun, funname, p1, p2, p3,p4,p5);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5);\
	else\
	pFun(p1, p2, p3,p4,p5);\
}\
}

// 载入dll函数接口的宏，参6
#define LOADDLLFUN6(btrue, fun, funname, p1, p2, p3, p4, p5, p6);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5, p6);\
	else\
	pFun(p1, p2, p3,p4,p5, p6);\
}\
}

// 载入dll函数接口的宏，参7
#define LOADDLLFUN7(btrue, fun, funname, p1, p2, p3, p4, p5, p6, p7);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5, p6, p7);\
	else\
	pFun(p1, p2, p3,p4,p5, p6, p7);\
}\
}

// 载入dll函数接口的宏，参8
#define LOADDLLFUN8(btrue, fun, funname, p1, p2, p3, p4, p5, p6, p7,p8);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5, p6, p7,p8);\
	else\
	pFun(p1, p2, p3,p4,p5, p6, p7,p8);\
}\
}

// 载入dll函数接口的宏，参9
#define LOADDLLFUN9(btrue, fun, funname, p1, p2, p3, p4, p5, p6, p7, p8, p9);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5, p6, p7,p8, p9);\
	else\
	pFun(p1, p2, p3,p4,p5, p6, p7,p8, p9);\
}\
}

// 载入dll函数接口的宏，参11
#define LOADDLLFUN11(btrue, fun, funname, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5, p6, p7,p8, p9,p10, p11);\
	else\
	pFun(p1, p2, p3,p4,p5, p6, p7,p8, p9,p10, p11);\
}\
}

// 载入dll函数接口的宏，参12
#define LOADDLLFUN12(btrue, fun, funname, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);\
{\
	fun pFun = NULL;\
	if (m_hSDKDll != NULL ) \
{\
	pFun = (fun)PROCADDR(m_hSDKDll, funname);\
}\
	if (pFun == NULL)\
{\
	if(btrue)\
	return ERR_NOFUN;\
}\
	if (pFun != NULL)\
{\
	if(btrue)\
	return pFun(p1, p2, p3,p4,p5, p6, p7,p8, p9,p10, p11, p12);\
	else\
	pFun(p1, p2, p3,p4,p5, p6, p7,p8, p9,p10, p11, p12);\
}\
}

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
IPC_API BOOL32 CDECL IPC_InitDll (char* pzDLLName, short wTelnetPort, bool bOpenTelnet,u32* pErrorCode)
{
#if defined(WIN32) || defined(_WIN64)
	char szFilePath[MAX_PATH];
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	char* lpszEnd = strrchr(szFilePath, '\\');
	*(lpszEnd+1) = 0;
	strcat_s(szFilePath, pzDLLName);

	if (strcmp(pzDLLName,"ipcsdk.dll") == 0)
	{		
		m_hSDKDll = ::LoadLibraryA(szFilePath);
		if (NULL == m_hSDKDll)
		{
			*pErrorCode = ERR_NODLL;
			return false;
		}
	}

#else	
	if(m_hSDKDll == NULL)
	{
		printf("linux start dlopen \n");
		m_hSDKDll = dlopen(pzDLLName, RTLD_LAZY|RTLD_GLOBAL);
		printf("linux start dlopen m_hSDKDll = %d\n", m_hSDKDll);
		if(m_hSDKDll == NULL)
		{
			char* error;
			error = dlerror();
			printf("dlopen is %s\n" , error);
			return ERR_NODLL;		
		}
	}
#endif
	
	typedef  bool  (* _IPC_InitDll)(short, int, u32*);
	LOADDLLFUN3(false, _IPC_InitDll, "IPC_InitDll",wTelnetPort,bOpenTelnet, pErrorCode);
	return true;
}

/*=================================================================
函数名称: IPC_ReleaseDll
功    能: 释放SDK的相关资源
          在应用程序退出时调用
参数说明:pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ReleaseDll(u32* pErrorCode)
{
	if (m_hSDKDll != NULL)
	{
		typedef  bool  (CDECL* _IPC_ReleaseDll)(u32*);
		LOADDLLFUN1(false, _IPC_ReleaseDll, "IPC_ReleaseDll",pErrorCode);
#if defined(WIN32) || defined(_WIN64)		
		if (m_hSDKDll != NULL)
		{
			if(::FreeLibrary(m_hSDKDll) != 0)
			{	
				m_hSDKDll = NULL;
			}
		}
#else
		if (m_hSDKDll != NULL)
		{
			if(dlclose(m_hSDKDll) == 0)
			{	
				m_hSDKDll = NULL;
			}
		}	
#endif		
	}

	return true;
}

/*=================================================================
函数名称: IPC_Logout
功    能: 用户注销注册
          
参数说明:
		  pHandle       前端句柄
		  pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Logout(void *pHandle, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Logout)(void *, u32*);
	LOADDLLFUN2(TRUE, _IPC_Logout, "IPC_Logout", pHandle,pErrorCode);
	*pErrorCode = ERR_NOFUN;
	return false;
}

IPC_API BOOL32 CDECL IPC_Login(void *pHandle, char* pName, char* pPassword, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Login)(void *, char*, char*, u32* );
	LOADDLLFUN4(TRUE, _IPC_Login, "IPC_Login", pHandle, pName, pPassword,pErrorCode);
	*pErrorCode = ERR_NOFUN;
	return false;
}

IPC_API BOOL32  CDECL IPC_Login_New(void* pLoginInfo, IPC_CHECKLINK_CB fCheckCB,u32* dwHandle, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Login_New)(void*, IPC_CHECKLINK_CB,u32*,u32* );
	LOADDLLFUN4(TRUE, _IPC_Login_New, "IPC_Login_New", pLoginInfo,fCheckCB,dwHandle,pErrorCode);
	*pErrorCode = ERR_NOFUN;
	return false;
}

IPC_API BOOL32 CDECL IPC_CheckConnectLost(u32 dwIp, unsigned short wPort)
{
	typedef  bool  (* _IPC_CheckConnectLost)(u32, unsigned short);
	LOADDLLFUN2(TRUE, _IPC_CheckConnectLost, "IPC_CheckConnectLost", dwIp, wPort);

	return ERR_NOFUN;
}

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
IPC_API u32 CDECL IPC_CreateHandle(u32 dwIP, u16 wPort, char* pName, char* pPassword)
{
	typedef  bool  (CDECL* _IPC_CreateHandle)(u32,  u16, char* , char*);
	LOADDLLFUN4(TRUE, _IPC_CreateHandle, "IPC_CreateHandle", dwIP, wPort, pName, pPassword);
	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_GetInfoFromHandle(u32 dwHandle, void* pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetInfoFromHandle)(void *, void*, int,u32* );
	LOADDLLFUN4(TRUE, _IPC_GetInfoFromHandle, "IPC_GetInfoFromHandle", &dwHandle, pParam, nLen,pErrorCode);
	*pErrorCode = ERR_NOFUN;
	return false;
}

/*=================================================================
函数名称: IPC_DestroyHandle()
功    能: 销毁句柄
          
参数说明:
		 

返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_DestroyHandle()
{
	typedef  bool  (CDECL* _IPC_DestroyHandle)();
	LOADDLLFUN(TRUE, _IPC_DestroyHandle, "IPC_DestroyHandle");
	return ERR_NOFUN;
}

IPC_API int CDECL IPC_GetVersion(char *pchVer,int nMaxLen)
{
	typedef  bool  (CDECL* _IPC_GetVersion)(char *, int);
	LOADDLLFUN2(TRUE, _IPC_GetVersion, "IPC_GetVersion", pchVer, nMaxLen);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetRecSchedule
		  功    能: 获取布防时间参数
参数说明: pParam, 布防类型detect, shieldalarm, alarmin, alarmout, exception, recchedule, warningline
	      pTimeArr, TEVENTTIME [],布防时间数组指针.
	      nlenTime, 布防时间数据个数.
	      nLenTimeStruct, 等于sizeof(TEVENTTIME)，用于后续兼容.
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<eventtype opt=” detect, shieldalarm, alarmin, alarmout, exception, recchedule, sightchange, warningline”/>
	      <recordtype opt=” alarm,manual,time,trigger”/>
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetEventTime (void *pHandle, char *pEventType, char *pIndex,void * pTimeArr, int nLenTime ,int nLenTimeStruct , char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetEventTime)(void *, char*, char*,void *, int, int, char*, int&,u32*);
	LOADDLLFUN9(TRUE, _IPC_GetEventTime, "IPC_GetEventTime", pHandle, pEventType,pIndex, pTimeArr, nLenTime, nLenTimeStruct, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetRecSchedule
		  功    能: 获取布防时间参数
参数说明: pParam, 布防类型detect, shieldalarm, alarmin, alarmout, exception, recchedule, warningline
	      pTimeArr, TEVENTTIME [],布防时间数组指针.
	      nlenTime, 布防时间数据个数.
	      nLenTimeStruct, 等于sizeof(TEVENTTIME)，用于后续兼容.
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetEventTime (void *pHandle, char *pEventType,  char *pIndex,void * pTimeArr, int nLenTime ,int nLenTimeStruct,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetEventTime)(void *, char*,char*, void *, int, int,u32 *);
	LOADDLLFUN7(TRUE, _IPC_SetEventTime, "IPC_SetEventTime", pHandle, pEventType, pIndex, pTimeArr, nLenTime, nLenTimeStruct, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_SendTransData(void *pHandle, char* szEventType, char* szParamIn, int nLenIn, char* szParamOut, int& nLenOut,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SendTransData)(void *, char* , char* , int , char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_SendTransData, "IPC_SendTransData", pHandle, szEventType,szParamIn, nLenIn, szParamOut, nLenOut,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetVideoInfo
功    能: 获取rtp码流
参数说明: pParam [in]       输入结构体参数
		  nLen [in]		    输入结构体长度
		  pInfoOut [out]	输出结构体参数
		  nLenInfo [out]	输出结构体长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetVideoInfo(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVideoInfo)(void *, emPlayVideoType,void*, int, void*, int&,u32*);
	LOADDLLFUN7(TRUE, _IPC_GetVideoInfo, "IPC_GetVideoInfo", pHandle,eType,pParam, nParamLen, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}


/*=================================================================
函数名称: IPC_GetRtspURL
功    能: 获取rtp码流，带告警元数据
参数说明: pParam [in]       输入结构体参数
		  nLen [in]		    输入结构体长度
		  pInfoOut [out]	输出结构体参数
		  nLenInfo [out]	输出结构体长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRtspUrl(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode, int bNoStream)
{
	typedef  bool  (CDECL* _IPC_GetRtspUrl)(void *, emPlayVideoType,void*, int, void*, int&,u32*,int);
	LOADDLLFUN8(TRUE, _IPC_GetRtspUrl, "IPC_GetRtspUrl", pHandle,eType,pParam, nParamLen, pInfoOut, nLenInfo, pErrorCode,bNoStream);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetRtspURLRec
功    能: 获取rtp码流，带告警元数据
参数说明: pParam [in]       输入结构体参数
		  nLen [in]		    输入结构体长度
		  pInfoOut [out]	输出结构体参数
		  nLenInfo [out]	输出结构体长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRtspUrlRec(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetRtspUrlRec)(void *, emPlayVideoType,void*, int, void*, int&,u32*);
	LOADDLLFUN7(TRUE, _IPC_GetRtspUrlRec, "IPC_GetRtspUrlRec", pHandle,eType,pParam, nParamLen, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}




/*=================================================================
函数名称: IPC_SendKeyFrame
功    能: 请求关键帧
参数说明: dwPlayID[in]--TRTCPVIDEOINFO中dwPlayID 播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SendKeyFrame(void *pHandle, u32 dwPlayID, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SendKeyFrame)(void *, u32, u32*);
	LOADDLLFUN3(TRUE, _IPC_SendKeyFrame, "IPC_SendKeyFrame", pHandle,dwPlayID,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetVideoVolume_RTCP
功    能: 获取视频音频
参数说明: pInfoOut[out]--TRTCPVIDEOVOLUMEINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoVolume_RTCP(void *pHandle, void *pInfoOut, int nLenInfo, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVideoVolume_RTCP)(void *, void* , int , u32* );
	LOADDLLFUN4(TRUE, _IPC_GetVideoVolume_RTCP, "IPC_GetVideoVolume_RTCP", pHandle, pInfoOut, nLenInfo,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetVideoVolume_RTCP
功    能: 设置视频音频
参数说明: pInfoOut[out]--TRTCPVIDEOVOLUMEINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoVolume_RTCP(void *pHandle, void* pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVideoVolume_RTCP)(void *, void* , int , u32* );
	LOADDLLFUN4(TRUE, _IPC_SetVideoVolume_RTCP, "IPC_SetVideoVolume_RTCP", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_VoiceCallState
功    能: 是否开启呼叫
参数说明: pInfoOut [out] TRTCPCALLSTATEINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_VoiceCallState (void *pHandle, void* pInfoOut, int nOutInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_CallState_RTCP)(void *,void*, int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_CallState_RTCP, "IPC_CallState_RTCP",pHandle,pInfoOut, nOutInfo,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_StartVoiceCall
功    能: 开始呼叫
参数说明: pHandle		前端句柄
		  emTransType   编码传输格式
		  pInfoOut[out] TRTCPPREPARECALLINFO
		  pErrorCode   	错误码
返 回 值: 成功返回呼叫ID, 失败返回0，原因解析pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_StartVoiceCall(void *pHandle,emTransType eTranstype, void*pInfoOut,int& nLenInfo,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_StartVoiceCall)(void *, emTransType, void *,int&,u32*);
	LOADDLLFUN5(TRUE, _IPC_StartVoiceCall, "IPC_StartVoiceCall", pHandle, eTranstype,pInfoOut,nLenInfo,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_StopVoiceCall
功    能: 停止呼叫
参数说明: dwID[in]--TRTCPPREPARECALLINFO中dwID
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopVoiceCall(void *pHandle, u32 dwVoiceCallId, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_StopVoiceCall)(void *, u32, u32*);
	LOADDLLFUN3(TRUE, _IPC_StopVoiceCall, "IPC_StopVoiceCall", pHandle, dwVoiceCallId,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_PuSnapShot
功    能: 前端手动抓拍
参数说明: 
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),四码流(4)
		  pchFileName     保存的图片文件全路径(文件路径),后缀为jpeg
		  pErrorCode      错误码
返 回 值: 返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_PuSnapShot(void *pHandle, u8 byStreamId, const char *pchFileName, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PuSnapShot)(void *, u8,const char * ,u32*);
	LOADDLLFUN4(TRUE, _IPC_PuSnapShot, "IPC_PuSnapShot", pHandle, byStreamId,pchFileName,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetPuSnapShotUrl
功    能: 前端手动抓拍
参数说明: 
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),四码流(4)
		  pImageUrl     图片url 大小为260字符
		  pErrorCode      错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPuSnapShotUrl(void *pHandle, u8 byStreamId, char *pImageUrl, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPuSnapShotUrl)(void *, u8,char * ,u32*);
	LOADDLLFUN4(TRUE, _IPC_GetPuSnapShotUrl, "IPC_GetPuSnapShotUrl", pHandle, byStreamId,pImageUrl,pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_GetRecMonthInfo(void *pHandle, int nYear,int nMonth, void *pDataOut, int& nArrayLenOut ,u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetRecMonthInfo)(void *, int, int, void*, int&, u32*);
	LOADDLLFUN6(TRUE, _IPC_GetRecMonthInfo, "IPC_GetRecMonthInfo", pHandle, nYear,nMonth,pDataOut,nArrayLenOut,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API u32 CDECL IPC_GetRecordData(void *pHandle, u32 dwRecId, void *pParam, void *pDataOut,int& nArrLenOut,u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetRecordData)(void *, u32, void*, void*, int&,u32*);
	LOADDLLFUN6(TRUE, _IPC_GetRecordData, "IPC_GetRecordData", pHandle, dwRecId,pParam,pDataOut,nArrLenOut,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API u32 CDECL IPC_GetRecordNext (void *pHandle, u32 dwTaskId, u32 dwFirstIndex,bool &bFinished,void *pDataOut,int nArrLenOut,u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetRecordNext)(void *, u32, u32,bool&,void*,int,u32*);
	LOADDLLFUN7(TRUE, _IPC_GetRecordNext, "IPC_GetRecordNext", pHandle, dwTaskId,dwFirstIndex,bFinished,pDataOut,nArrLenOut,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_PreLoadRecordList
功    能:	准备录像数据
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			pParam 		TRECPARAM
			nParamLen		结构体的长度
返 回 值: 成功返回查询ID, 失败返回0, 原因解析pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_PreLoadRecordList(void *pHandle, void *pParam, int nParamLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PreLoadRecordList)(void *,void*, int,u32*);
	LOADDLLFUN4(TRUE, _IPC_PreLoadRecordList, "IPC_PreLoadRecordList", pHandle, pParam,nParamLen,pErrorCode);

	return ERR_NOFUN;
}
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
				bNoStream		是否不申请码流，默认申请码流
返 回 值: 	成功返回播放id, 失败返回0;
=================================================================*/
IPC_API u32 CDECL IPC_StartRecordPlay(void *pHandle, void *pParam, int nParamLen,void *pDataOut, int nDataLen,bool bDec,bool bNoStream, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_StartRecordPlay)(void *,void*, int,void*,int,bool,bool,u32*);
	LOADDLLFUN8(TRUE, _IPC_StartRecordPlay, "IPC_StartRecordPlay", pHandle, pParam,nParamLen,pDataOut,nDataLen,bDec,bNoStream,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: 	IPC_StopRecordPlay
功    能:	停止放像
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
			dwPlayID		播放ID
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_StopRecordPlay)(void *,u32,u32*);
	LOADDLLFUN3(TRUE, _IPC_StopRecordPlay, "IPC_StopRecordPlay", pHandle, dwPlayID,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: 	IPC_PauseRecordPlay
功    能:	暂停放像
参数说明:	pHandle		前端句柄
			pErrorCode  错误码
			dwPlayID	播放ID
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_PauseRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PauseRecordPlay)(void *,u32,u32*);
	LOADDLLFUN3(TRUE, _IPC_PauseRecordPlay, "IPC_PauseRecordPlay", pHandle, dwPlayID,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: 	IPC_ResumeRecordPlay
功    能:	恢复放像
参数说明:	pHandle		前端句柄
			pErrorCode  错误码
			dwPlayID	播放ID
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_ResumeRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_ResumeRecordPlay)(void *,u32,u32*);
	LOADDLLFUN3(TRUE, _IPC_ResumeRecordPlay, "IPC_ResumeRecordPlay", pHandle,dwPlayID,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_MoveRecordPlay(void *pHandle, u32 dwPlayID, void *pParam, int nParamLen,u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_MoveRecordPlay)(void *,u32, void*,int,u32*);
	LOADDLLFUN5(TRUE, _IPC_MoveRecordPlay, "IPC_MoveRecordPlay", pHandle, dwPlayID,pParam,nParamLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: 	IPC_SetRecordPlayRate
功    能:	播放速度控制
参数说明:	pHandle			前端句柄
			pErrorCode   	错误码
			dwPlayID		播放ID
			dwPlayRate		emRecPlayRate
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRecordPlayRate(void *pHandle, u32 dwPlayID,u32 dwPlayRate, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetRecordPlayRate)(void *,u32, u32,u32*);
	LOADDLLFUN4(TRUE, _IPC_SetRecordPlayRate, "IPC_SetRecordPlayRate", pHandle, dwPlayID,dwPlayRate,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_Getrecplaystate (void *pHandle, u32 dwPlayID,void* pDataOut, int &nDataOut,u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Getrecplaystate)(void *,u32, void*,int&,u32*);
	LOADDLLFUN5(TRUE, _IPC_Getrecplaystate, "IPC_Getrecplaystate", pHandle, dwPlayID,pDataOut,nDataOut,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: 	IPC_Setrecdownloadspeed
功    能:	设置录像下载速度
参数说明:	pHandle		前端句柄
			pErrorCode   	错误码
返 回 值: 	成功返回true, 失败返回false,错误码pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_Setrecdownloadspeed (void *pHandle,u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Setrecdownloadspeed)(void *,u32*);
	LOADDLLFUN2(TRUE, _IPC_Setrecdownloadspeed, "IPC_Setrecdownloadspeed", pHandle, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetMultiVideoMode
功    能: 获取多码流模式参数
参数说明: pInfoOut[in]--TMULTIVIDEOMODEINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetMultiVideoMode(void *pHandle, u8 byVideoSource, void *pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetMultiVideoMode)(void *, u8, void*, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetMultiVideoMode, "IPC_GetMultiVideoMode", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetMultiVideoMode
功    能: 设置多码流模式参数
参数说明: pParam[in]--TMULTIVIDEOMODEPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetMultiVideoMode(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetMultiVideoMode)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetMultiVideoMode, "IPC_SetMultiVideoMode", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetVideoEnc
功    能: 获取视频编码参数
参数说明: pInfoOut[in]--TVIDEOENCINFO
		  byVideoSource[in]--视频源ID
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),死码流(4)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoEnc(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVideoEnc)(void *,u8, u8, void*, int&, char*, int&, u32 *);
	LOADDLLFUN8(TRUE, _IPC_GetVideoEnc, "IPC_GetVideoEnc", pHandle,byVideoSource, byStreamId, pInfoOut, nLenInfo, szAssistInfo, nAssistLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetVideoEnc
功    能: 设置视频编码参数
参数说明: pParam[in]--TVIDEOENCPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoEnc(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVideoEnc)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetVideoEnc, "IPC_SetVideoEnc", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}


/*=================================================================
函数名称: IPC_GetAudioEnc
功    能: 获取音频编码参数
参数说明: pInfoOut[in]--TAUDIOENCINFO
		  byVideoSource[in]--视频源ID
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),死码流(4)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAudioEnc(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAudioEnc)(void *,u8, u8, void*, int&, char*, int&, u32 *);
	LOADDLLFUN8(TRUE, _IPC_GetAudioEnc, "IPC_GetAudioEnc", pHandle,byVideoSource, byStreamId, pInfoOut, nLenInfo, szAssistInfo, nAssistLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetAudioEnc
功    能: 设置音频编码参数
参数说明: pParam[in]--TAUDIOENCPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetAudioEnc(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAudioEnc)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAudioEnc, "IPC_SetAudioEnc", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API int CDECL IPC_SetAudioEncExt(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAudioEncExt)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAudioEncExt, "IPC_SetAudioEncExt", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetAudioDec
功    能: 获取音频解码参数
参数说明: pInfoOut[in]--TAUDIODECINFO
		  byVideoSource[in]--视频源ID
		  byStreamId[in]--主码流(1),辅码流(2),三码流(3),四码流(4)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAudioDec(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAudioDec)(void *,u8, u8, void*, int&, u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetAudioDec, "IPC_GetAudioDec", pHandle,byVideoSource, byStreamId, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetAudioDec
功    能: 设置音频解码参数
参数说明: pParam[in]--TAUDIODECPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetAudioDec(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAudioDec)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAudioDec, "IPC_SetAudioDec", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetVideoShield
功    能: 获取图像遮蔽
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TVIDEOSHIELDINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoShield(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo,  char* szAssistInfo, int &nAssistLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVideoShield)(void *, u8, void *, int&, char*, int&, u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetVideoShield, "IPC_GetVideoShield", pHandle,byVideoSource, pInfoOut, nLenInfo, szAssistInfo, nAssistLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetVideoShield
功    能: 设置图像遮蔽
参数说明: pParam[in] --TVIDEOSHIELDPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoShield(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVideoShield)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetVideoShield, "IPC_SetVideoShield", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetVideoRoi
功    能: 获取敏感区域编码
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TVIDEOROIINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoRoi(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVideoRoi)(void *, u8, void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetVideoRoi, "IPC_GetVideoRoi", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetVideoRoi
功    能: 设置敏感区域编码
参数说明: pParam[in] --TVIDEOROIPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoRoi(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVideoRoi)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetVideoRoi, "IPC_SetVideoRoi", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetVideoEncCut
功    能: 获取编码裁剪
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TVIDEOENCCUTINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVideoEncCut(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVideoEncCut)(void *, u8, void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetVideoEncCut, "IPC_GetVideoEncCut", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetCheckPointOsd
功    能: 获取osd信息
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TCHECKPOINTINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetCheckPointOsd(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetCheckPointOsd)(void *, u8, void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetCheckPointOsd, "IPC_GetCheckPointOsd", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetCheckPointOsd
功    能: 获取osd信息
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TCHECKPOINTPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetCheckPointOsd(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetCheckPointOsd)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetCheckPointOsd, "IPC_SetCheckPointOsd", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetUavTrack
功    能: 设置智能跟踪开关
参数说明: pParam[in] --enable
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetUavTrack(void *pHandle, bool pParam,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetUavTrack)(void *, bool, u32 *);
	LOADDLLFUN3(TRUE, _IPC_SetUavTrack, "IPC_SetUavTrack", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetUavTrack
功    能: 获取智能跟踪开关
参数说明: pParam[in] --enable
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetUavTrack(void *pHandle, bool& pParam, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetUavTrack)(void *, bool&, u32 *);
	LOADDLLFUN3(TRUE, _IPC_GetUavTrack, "IPC_GetUavTrack", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetEventManualSnap
功    能: 获取事件抓拍信息
参数说明:
		  pInfoOut[out] --TMANUALSNAPINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetEventManualSnap(void *pHandle,void* pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetEventManualSnap)(void *, void *, int &, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetEventManualSnap, "IPC_GetEventManualSnap", pHandle, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetEventManualSnap
功    能: 设置事件抓拍信息
参数说明: pParam[in] --TMANUALSNAPINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetEventManualSnap(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetEventManualSnap)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetEventManualSnap, "IPC_SetEventManualSnap", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetVideoEncCut
功    能: 设置编码裁剪
参数说明: pParam[in] --TVIDEOENCCUTPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVideoEncCut(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVideoEncCut)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetVideoEncCut, "IPC_SetVideoEncCut", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetOsdCap
功    能: 获取OSD参数能力支持
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TOSDCAPINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsdCap(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetOsdCap)(void *, u8, void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetOsdCap, "IPC_GetOsdCap", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetOsd
功    能: 获取OSD参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TOSDINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsd(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetOsd)(void *, u8, void *, int&, char*, int&, u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetOsd, "IPC_GetOsd", pHandle,byVideoSource, pInfoOut, nLenInfo, szAssistInfo, nAssistLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetOsd
功    能: 设置OSD参数
参数说明: pParam[in] --TOSDPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsd(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetOsd)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetOsd, "IPC_SetOsd", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetOsdList
功    能: 设置单个OSD属性参数
参数说明:  pParam[in] --TOSDLISTPARAM
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsdList(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetOsdList)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetOsdList, "IPC_SetOsdList", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetSmartOsdInfo
功    能: 设置烟火报警信息
参数说明:  pParam[in] --TOSDSMARTPARAM
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetSmartOsdInfo(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSmartOsdInfo)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSmartOsdInfo, "IPC_SetSmartOsdInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetOsdList
功    能: 获取单个OSD属性参数
参数说明:byVideoSource[in] --视频源ID
		 pInfoOut[out] --TOSDLISTINFO
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsdList(void *pHandle, u8 byVideoSource,void *pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetOsdList)(void *, u8, void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetOsdList, "IPC_GetOsdList", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetSmartOsdInfo
功    能: 获取烟火报警信息
参数说明:byVideoSource[in] --视频源ID
		 pInfoOut[out] --TOSDSMARTINFO
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetSmartOsdInfo(void *pHandle, u8 byVideoSource,void *pInfoOut, int &nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSmartOsdInfo)(void *, u8, void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetSmartOsdInfo, "IPC_GetSmartOsdInfo", pHandle,byVideoSource, pInfoOut, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetOsdStateQuery
功    能: Osd图片上传状态查询
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TOSDSTATEINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOsdStateQuery(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetOsdStateQuery)(void *, u8, void *, int&, char*, int&, u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetOsdStateQuery, "IPC_GetOsdStateQuery", pHandle,byVideoSource, pInfoOut, nLenInfo, szAssistInfo, nAssistLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetOsdPrepare
功    能: Osd图片准备上传
参数说明: pParam[in] --TOSDPREPAREPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsdPrepare(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetOsdPrepare)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetOsdPrepare, "IPC_SetOsdPrepare", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetOsdUploadFile
功    能: Osd图片上传
参数说明: pParam[in] --TOSDUPLOADFILEPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOsdUploadFile(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetOsdUploadFile)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetOsdUploadFile, "IPC_SetOsdUploadFile", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_IMG_Def
功    能: 图像恢复默认
参数说明: pParam[in] --TIMGDEFPARAM
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_Img_Def(void *pHandle,void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Img_Def)(void *,void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_Img_Def, "IPC_Img_Def",pHandle,pParam,nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetColor
功    能: 获取图像常规设置
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TCOLORINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetColor(void *pHandle, u8 byVideoSource, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetColor)(void *, u8, void *, int,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetColor, "IPC_GetColor", pHandle,byVideoSource, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetColor
功    能: 设置图像常规设置
参数说明: pParam[in] --TCOLORPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetColor(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetColor)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetColor, "IPC_SetColor",pHandle,  pParam,  nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetFocus
功    能: 获取聚焦参数
参数说明: byVideoSource[in] --视频源ID
		  pParam[out] --TFOCUSINFO
		  szAssistInfo[out] --聚焦可选参数
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFocus(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetFocus)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetFocus, "IPC_GetFocus", pHandle,byVideoSource,pParam,nLen, szParamAssist,  nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetFocus
功    能: 设置聚焦参数
参数说明: pParam[in] --TFOCUSPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFocus (void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetFocus)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetFocus, "IPC_SetFocus", pHandle,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetWhiteBlance
功    能: 获取白平衡参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TWHITEBLANCEINFO
		  szAssistInfo[out] --白平衡可选参数
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWhiteBlance (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetWhiteBlance)(void *, u8 ,void *,int ,char* , int ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetWhiteBlance, "IPC_GetWhiteBlance", pHandle,byVideoSource,pParam, nLen, szParamAssist,  nLenAssist,pErrorCode);
	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetWhiteBlance
功    能: 设置白平衡参数
参数说明: pParam[in] --TWHITEBLANCEPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWhiteBlance (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetWhiteBlance)(void *, void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetWhiteBlance, "IPC_SetWhiteBlance", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetIrcutFilter
功    能: 获取日夜模式参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TRCUTFILTERINFO
		  szAssistInfo[out] --日夜模式可选参数
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetIrcutfilter (void *pHandle, u8 byVideoSource,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetIrcutFilter)(void *, u8 ,void *,int , char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetIrcutFilter, "IPC_GetIrcutFilter",pHandle, byVideoSource,pParam,nLen, szParamAssist,  nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetIrcutFilter
功    能: 设置日夜模式参数
参数说明: pParam[in] --TTRCUTFILTERPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetIrcutfilter (void *pHandle,void *pParam,int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetIrcutFilter)(void *,void *,int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetIrcutFilter, "IPC_SetIrcutFilter", pHandle,pParam,nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetIris
功    能: 获取光圈参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TIRISINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetIris (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetIris)(void *, u8 ,void *,int ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetIris, "IPC_GetIris", pHandle, byVideoSource,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetIris
功    能: 设置光圈参数
参数说明: pParam[in] --TIRISPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetIris (void *pHandle,void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetIris)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetIris, "IPC_SetIris", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetShutter
功    能: 获取快门参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TSHUTTERINFO
		  szAssistInfo[out] --快门可选参数
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetShutter(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetShutter)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetShutter, "IPC_GetShutter",pHandle,byVideoSource,pParam,nLen,szParamAssist,nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetSutter
功    能: 设置快门参数
参数说明: pParam[in] --TSHUTTERPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetShutter (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetShutter)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetShutter, "IPC_SetShutter", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetGain
功    能: 获取增益参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TGAININFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGain(void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetGain)(void *, u8 ,void *,int ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetGain, "IPC_GetGain", pHandle,byVideoSource,pParam,nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetGain
功    能: 设置增益参数
参数说明: pParam[in] --TGAINPARAM
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetGain (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetGain)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetGain, "IPC_SetGain", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetInfrared
功    能: 获取红外参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TINFRAREDINFO
		  szAssistInfo[out] -- 红外光启用模式 <contrlmode opt=”close,auto,farlamp,middlelamp,nearlamp,exnearlamp”/>
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_GetInfrared(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetInfrared)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetInfrared, "IPC_GetInfrared", pHandle,byVideoSource,pParam,nLen,szParamAssist,nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetInfrared
功    能: 设置红外参数
参数说明: pParam[in] --TINFRAREDPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_SetInfrared(void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetInfrared)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetInfrared, "IPC_SetInfrared", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetLaser
功    能: 获取激光参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TLASERINFO
		  szAssistInfo[out] --激光聚光模式<spotmode opt=”def,littlefacula,bigfaxula,custom1,custom2”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_GetLaser(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetLaser)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetLaser, "IPC_GetLaser",pHandle,byVideoSource,pParam,nLen,szParamAssist,nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetLaser
功    能: 设置激光参数
参数说明: pParam[in] --TLASERPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetLaser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetLaser)(void *, void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetLaser, "IPC_SetLaser",  pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetCorrido
功    能: 获取走廊参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TCORRIDOINFO
		  szAssistInfo[out] --走廊模式<spotmode opt=”close, left,right”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCorridorMode (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetCorridorMode)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetCorridorMode, "IPC_GetCorridorMode", pHandle,byVideoSource,pParam,nLen,szParamAssist,nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetCorrido
功    能: 设置走廊参数
参数说明: pParam[in] --TCORRIDOPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetCorridorMode (void *pHandle,void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetCorridorMode)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetCorridorMode, "IPC_SetCorridorMode", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetImageMode
功    能: 获取镜像模式参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TIMAGEMODEINFO
		  szAssistInfo[out] --镜像模式<spotmode opt=”close, horizont, vertical, central”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetImageMode (void *pHandle, u8 byVideoSource, void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetImageMode)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetImageMode, "IPC_GetImageMode",  pHandle,byVideoSource,pParam,nLen,szParamAssist,nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetImageMode
功    能: 设置镜像参数
参数说明: pParam[in] --TIMAGEMODEPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetImageMode (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetImageMode)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetImageMode, "IPC_SetImageMode", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetNoiseRedece2D
功    能: 获取2D降噪参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TNOISEREDUCE2DINFO
		  szAssistInfo[out] --2D模式<spotmode opt=”auto,open,close”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Get2DDenoise (void *pHandle, u8 byVideoSource, void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Get2DDenoise)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_Get2DDenoise, "IPC_Get2DDenoise", pHandle,byVideoSource,pParam,nLen,szAssistInfo,nAssistLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetNoiseRedece2D
功    能: 设置2D降噪参数
参数说明: pParam[in] --TNOISEREDUCE2DPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Set2DDenoise (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Set2DDenoise)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_Set2DDenoise, "IPC_Set2DDenoise",pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetNoiseRedece3D
功    能: 获取3D降噪参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TNOISEREDUCE3DINFO
		  szAssistInfo[out] --3D模式<spotmode opt=”auto,open,close”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Get3DDenoise(void *pHandle, u8 byVideoSource,void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Get3DDenoise)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_Get3DDenoise, "IPC_Get3DDenoise",  pHandle,byVideoSource,pParam,nLen,szAssistInfo,nAssistLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetNoiseRedece3D
功    能: 设置3D降噪参数
参数说明: pParam[in] --TNOISEREDUCE3DPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Set3DDenoise (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Set3DDenoise)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_Set3DDenoise, "IPC_Set3DDenoise", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetLocalEcho
功    能: 获取本地回显参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TLOCALECHOINFO
		  szAssistInfo[out] --模式<spotmode opt=”close, PAL,NTSC,LINE,INTERLEAVE”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetLocalEcho (void *pHandle, u8 byVideoSource, void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetLocalEcho)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetLocalEcho, "IPC_GetLocalEcho",  pHandle,byVideoSource,pParam,nLen,szAssistInfo,nAssistLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetLocalEcho
功    能: 设置本地回显参数
参数说明: pParam[in] --TLOCALECHOPARAM
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetLocalEcho (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetLocalEcho)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetLocalEcho, "IPC_SetLocalEcho",  pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetPowerLineFrequency
功    能: 获取防闪烁参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TPOWERLINEFREQUENCYINFO
		  szAssistInfo[out] --模式<spotmode opt=”50hz,60hz,auto”/> 
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPowerLineFrequency (void *pHandle, u8 byVideoSource,void *pParam, int nLen,  char* szAssistInfo, int &nAssistLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPowerLineFrequency)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetPowerLineFrequency, "IPC_GetPowerLineFrequency",  pHandle,byVideoSource,pParam,nLen,szAssistInfo,nAssistLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetPowerLineFrequency
功    能: 设置防闪烁参数
参数说明: pParam[in] --TPOWERLINEFREQUENCYPARAM
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPowerLineFrequency (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPowerLineFrequency)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPowerLineFrequency, "IPC_SetPowerLineFrequency",  pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}


/*=================================================================
函数名称: IPC_GetWdr
功    能: 获取宽动态参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWDR(void *pHandle, u8 byVideoSource, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetWDR)(void *, u8, void *, int,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetWDR, "IPC_GetWDR",pHandle, byVideoSource, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetWDR
功    能: 设置宽动态参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为wdr 只需设置nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWDR (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetWDR)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetWDR, "IPC_SetWDR",pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetHLC
功    能: 获取强光抑制参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetHLC(void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetHLC)(void *, u8, void *, int,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetHLC, "IPC_GetHLC", pHandle, byVideoSource, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetHLC
功    能: 设置强光抑制参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为hlc 只需设置nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetHLC (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetHLC)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetHLC, "IPC_SetHLC", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetFogThrough
功    能: 获取透雾参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFogThrough (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetFogThrough)(void *, u8 ,void *,int ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetFogThrough, "IPC_GetFogThrough", pHandle,  byVideoSource,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetFogThrough
功    能: 设置透雾参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为fogthrough或者close 只需设置nLevel有效
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFogThrough (void *pHandle,void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetFogThrough)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetFogThrough, "IPC_SetFogThrough", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetGamma
功    能: 获取Gamma参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGamma (void *pHandle, u8 byVideoSource, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetGamma)(void *, u8 ,void *,int ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetGamma, "IPC_GetGamma", pHandle,  byVideoSource,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetGamma
功    能: 设置Gamma参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为gamma或close 只需设置nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetGamma (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetGamma)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetGamma, "IPC_SetGamma", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetSmartIR
功    能: 获取SmartIR参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSmartIR (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSmartIR)(void *, u8 ,void *,int ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetSmartIR, "IPC_GetSmartIR",  pHandle,  byVideoSource,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetSmartIR
功    能: 设置SmartIR参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为smartir或close 只需设置nLevel有效
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSmartIR (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSmartIR)(void *,void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSmartIR, "IPC_SetSmartIR",  pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetBLC
功    能: 获取背光补偿参数
参数说明: byVideoSource[in] --视频源ID
		  pInfoOut[out] --TDYNAMICMODEINFO 只有nLevel有效
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBLC(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetBlc)(void *, u8 ,void *,int ,char* , int& ,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetBlc, "IPC_GetBlc", pHandle,  byVideoSource,pParam, nLen, szParamAssist,  nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetBLC
功    能: 设置背光补偿参数
参数说明: pParam[in] --TDYNAMICMODEPARAM  szMode[17]默认为autoblc,regionblc或close 只需设置nLevel有效
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetBLC (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetBLC)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetBLC, "IPC_SetBLC", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetFillLight
功    能: 获取红外补光灯
参数说明: 
		  pParam[out] --TDFILLLIGHT
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFillLight (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetFillLight)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetFillLight, "IPC_GetFillLight", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetFillLight
功    能: 设置红外补光灯
参数说明: pParam[in] --TDFILLLIGHT
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFillLight (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetFillLight)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetFillLight, "IPC_SetFillLight", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetDynPluginState
功    能: 设置动态插件状态
参数说明: pParam[in] --TDDYNMAICPLUGIN
          pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDynPluginState (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDynPluginState)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetDynPluginState, "IPC_SetDynPluginState", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetDeviceInfo
功    能: 获取设备信息
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDeviceInfo(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetDeviceInfo)(void *, void *,int,u32*);
	LOADDLLFUN4(TRUE, _IPC_GetDeviceInfo, "IPC_GetDeviceInfo", pHandle,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetDeviceInfo
功    能: 设置设备信息
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDeviceInfo(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDeviceInfo)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetDeviceInfo, "IPC_SetDeviceInfo", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetTestToolInfo
功    能: 设置测试工具基本信息
参数说明: TTOOLINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTestToolInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetTestToolInfo)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetTestToolInfo, "IPC_SetTestToolInfo", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetDevSeqInfo
功    能: 设置devseq信息
参数说明: TDEVSEQINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDevSeqInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDevSeqInfo)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetDevSeqInfo, "IPC_SetDevSeqInfo", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetEepromInfo
功    能: 设置eeprom信息
参数说明: TEEPROMINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetEepromInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetEepromInfo)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetEepromInfo, "IPC_SetEepromInfo", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetSystemTime
功    能: 获取系统时间
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSystemTime(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSystemTime)(void *, void *,int&,u32*);
	LOADDLLFUN4(TRUE, _IPC_GetSystemTime, "IPC_GetSystemTime", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetSystemTime
功    能: 设置系统时间
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSystemTime(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSystemTime)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSystemTime, "IPC_SetSystemTime", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetSysTimeSynChronise
功    能: 设置时间同步
参数说明: TSYSTIMEINFO
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSysTimeSynChronise(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSysTimeSynChronise)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSysTimeSynChronise, "IPC_SetSysTimeSynChronise", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetTimeAutoEx
功    能: 设置自动校时
参数说明: TTIMEAUTOINFOEX
pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTimeAutoEx(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetTimeAutoEx)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetTimeAutoEx, "IPC_SetTimeAutoEx", pHandle, pParam,nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetTimeAutoEx
功    能: 获取自动校时
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetTimeAutoEx(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetTimeAutoEx)(void *, void* , int& ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetTimeAutoEx, "IPC_GetTimeAutoEx", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetTimeAuto
功    能: 设置自动校时
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTimeAuto(void *pHandle, void* pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetTimeAuto)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetTimeAuto, "IPC_SetTimeAuto", pHandle, pParam,nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetTimeAuto
功    能: 获取自动校时
参数说明: pParam [out] -- 配置数据指针
nLen -- 数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetTimeAuto(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetTimeAuto)(void *, void* , int& ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetTimeAuto, "IPC_GetTimeAuto", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetDST
功    能: 设置夏令时
参数说明: TTIMEAUTOINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDST(void *pHandle, void* pParam, int nLen,u32 *pErrorCode )
{
	typedef  bool  (CDECL* _IPC_SetDST)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetDST, "IPC_SetDST", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetDST
功    能: 获取夏令时
参数说明: TTIMEAUTOINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDST(void *pHandle, void* pParam, int nLen,u32 *pErrorCode  )
{
	typedef  bool  (CDECL* _IPC_GetDST)(void *, void* , int ,u32 * );
	LOADDLLFUN4(TRUE, _IPC_GetDST, "IPC_GetDST", pHandle,pParam,nLen,pErrorCode );

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetRSInfo
功    能: 获取串口信息
参数说明: pParam, PTRSINFO
此处传递：<addressnum enable="false"><controlprotocol opt="pelcol"  enable="false">
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRSInfo(void *pHandle, void* ptParam, int nLen, char* szParamAssist, int nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetRSInfo)(void *, void* , int , char* , int ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetRSInfo, "IPC_GetRSInfo", pHandle, ptParam, nLen, szParamAssist,nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetRSInfo
功能:设置串口停止
参数说明: pParam, PTRSINFO
此处传递：<addressnum enable="false"><controlprotocol opt="pelcol"  enable="false">
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRSInfo(void *pHandle, void* ptParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetRSInfo)(void *, void* , int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetRSInfo, "IPC_SetRSInfo", pHandle,ptParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_LedFind
功能:打开关闭led灯
参数说明: pParam, PTLEDINFO;
szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
此处传递：<speed opt="5s,10s,30s,1m,30m,60m,24h"/>
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_LedFind(void *pHandle, void* ptParam, int nLen, char* szParamAssist, int nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_LedFind)(void *, void* , int , char* , int ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_LedFind, "IPC_LedFind",pHandle, ptParam, nLen, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SystemReboot
功能:系统重启
参数说明: szMode, base(简单恢复，保留ip)/full(完全恢复)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SystemReboot(void *pHandle,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SystemReboot)(void *,u32 *);
	LOADDLLFUN2(TRUE, _IPC_SystemReboot, "IPC_SystemReboot",pHandle,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_RecoverFactory
功能:设备恢复
参数说明: szMode, base(简单恢复，保留ip)/full(完全恢复)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_RecoverFactory (void *pHandle, char* szMode,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_RecoverFactory)(void *, char* ,u32 *);
	LOADDLLFUN3(TRUE, _IPC_RecoverFactory, "IPC_RecoverFactory", pHandle, szMode,pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_UserActive (u32 dwIP, u16 wPort, void*pParam,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_UserActive)(u32, u16, void* ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_UserActive, "IPC_UserActive",dwIP, wPort, pParam,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_ConfigUpload
功能:导入配置文件
参数说明: pParam, PTUPLOADINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_ConfigUpload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_ConfigUpload)(void *, void *, int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_ConfigUpload, "IPC_ConfigUpload", pHandle, pParam,  nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_ConfigDownload
功能:pParam, PTDOWNLOADINFO
参数说明: pParam, PTDOWNLOADINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_ConfigDownload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_ConfigDownload)(void *, void *, int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_ConfigDownload, "IPC_ConfigDownload", pHandle, pParam,  nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SysUpload
功能:系统升级
参数说明: pParam, PTUPLOADINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SysUpload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SysUpload)(void *, void *, int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_SysUpload, "IPC_SysUpload", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称:IPC_ SysUploadState
功    能:升级状态查询
参数说明:pHandle  		前端句柄
		pParam,  		PTUPLOADSTATE;
		pErrorCode   	错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SysUploadState (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SysUploadState)(void *, void *, int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_SysUploadState, "IPC_SysUploadState", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetAdvanced 
功能:获取系统高级配置信息
参数说明: pParam, PTADVANCEDINFO
szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
此处传递：<osdpicfont opt=”song,black” /><mtcfmode opt=”hdr,fps60,stream4,4k,div4,normal,hdsdi enable=true”><aacmode opt=”autolow, normal, low enable=true”>
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAdvanced(void *pHandle, void *pParam, int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAdvanced)(void *, void *, int , char* , int& ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetAdvanced, "IPC_GetAdvanced", pHandle, pParam, nLen, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetAdvanced 
功能:设置系统高级配置信息
参数说明: 	pParam, PTADVANCEDINFO
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAdvanced (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAdvanced)(void *, void *, int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAdvanced, "IPC_SetAdvanced", pHandle,pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetUserInfo
功    能: 获取用户权限配置
参数说明: pArrUserInfo, 用户权限数组TUSERINFO[]
		  nArrLen, 数组长度
		  nLenStruct,数据长度,等于sizeof(TUSERINFO).
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetUserInfo (void *pHandle, void *pParam, int &nLen, int& nLenStruct,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetUserInfo)(void *, void *, int&, int&,u32*);
	LOADDLLFUN5(TRUE, _IPC_GetUserInfo, "IPC_GetUserInfo", pHandle, pParam, nLen, nLenStruct,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetUserInfo
功    能: 设置用户权限配置
参数说明: pParam, PTUSERINFO
		  nLenStruct,数据长度,等于sizeof(TUSERINFO)
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetUserInfo (void *pHandle, void *pParam, int nLenStruct,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetUserInfo)(void *, void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetUserInfo, "IPC_SetUserInfo", pHandle, pParam,nLenStruct,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_AddUser
功    能: 设置用户权限配置
参数说明: pParam, PTUSERINFO
		  nLen,数据长度,等于sizeof(TUSERINFO)
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_AddUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_AddUser)(void *, void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_AddUser, "IPC_AddUser", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_DelUser
功    能: 设置用户权限配置
参数说明: pParam, PTUSERDELINFO
		  nLen,数据长度,等于sizeof(TUSERDELINFO)
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_DelUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_DelUser)(void *, void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_DelUser, "IPC_DelUser", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_UpdateUser
功    能: 修改用户
参数说明: pParam, PTUSERUPDATEINFO
		  nLen,数据长度,等于sizeof(TUSERUPDATEINFO)
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_UpdateUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_UpdateUser)(void *, void *, int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_UpdateUser, "IPC_UpdateUser", pHandle,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetPowerInfo
功    能: 获取用户权限配置
参数说明: pArrPowerInfo, 用户权限数组TPOWERINFO[]
		  nArrLen, 数组长度
		  nLenStruct,数据长度,等于sizeof(TPOWERINFO).
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPowerInfo (void *pHandle, void *pParam, int nLen, int& nLenStruct,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPowerInfo)(void *, void *, int , int& ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetPowerInfo, "IPC_GetPowerInfo", pHandle, pParam, nLen, nLenStruct,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPowerInfo
功    能: 设置用户权限配置
参数说明: pParam, PTPOWERINFO
		  pArrPowerInfo, 用户权限数组TPOWERINFO[]
		  nArrLen, 数组长度
		  nLenStruct,数据长度,等于sizeof(TPOWERINFO)
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPowerInfo (void *pHandle, void *pParam, int nLen, int nLenStruct,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPowerInfo)(void *, void *, int , int ,u32 *);
	LOADDLLFUN5(TRUE, _IPC_SetPowerInfo, "IPC_SetPowerInfo", pHandle, pParam,  nLen,  nLenStruct,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetDdns
功    能: 获取DDNS参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetDdns(void *pHandle,void *pParam/* = NULL*/, int nLen, char* const szParamAssist, int nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetDdns)(void *, void *,int, char* const, u32*);
	LOADDLLFUN5(TRUE, _IPC_GetDdns, "IPC_GetDdns", pHandle, pParam,nLen,szParamAssist, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetDdns
功    能: 设置DDNS参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetDdns(void *pHandle,void *pParam/* = NULL*/, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDdns)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetDdns, "IPC_SetDdns", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetDdnsState
功    能: 获取DDNS状态参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetDdnsState(void *pHandle,void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetDdnsState)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetDdnsState, "IPC_GetDdnsState", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetNetPort
功    能: 获取端口参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetNetPort(void *pHandle,void *pParam/* = NULL*/, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetNetPort)(void *, void *, int , u32*);
	LOADDLLFUN4(TRUE, _IPC_GetNetPort, "IPC_GetNetPort", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetNetPort
功    能: 设置端口参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetNetPort(void *pHandle,void *pParam/* = NULL*/, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetNetPort)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetNetPort, "IPC_SetNetPort", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetIpInfo
功    能: 获取IP参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetIpInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetIpInfo)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetIpInfo, "IPC_GetIpInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetIpInfo
功    能: 设置IP参数
参数说明: pParam [out] -- 配置数据指针
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetIpInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetIpInfo)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetIpInfo, "IPC_SetIpInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetWlanInfo
功    能: 获取Wlan参数
参数说明: dwHandle      设备句柄
		  pParam [out] -- 配置数据指针TWLANINFO
		  nLen -- 数据长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWlanInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetWlanInfo)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetWlanInfo, "IPC_GetWlanInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetWlanInfo
功    能: 设置Wlan参数
参数说明: dwHandle      设备句柄
		  pParam [out] -- 配置数据指针TWLANINFO
		  nLen -- 数据长度
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWlanInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetWlanInfo)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetWlanInfo, "IPC_SetWlanInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetPPPoE
功    能: 获取PPPoE参数
参数说明: pParam [out] -- 配置数据指针TUPNPPORTMAP
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPPPoE(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPPPoE)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetPPPoE, "IPC_GetPPPoE", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetPPPoE
功    能: 设置PPPoE参数
参数说明: pParam [out] -- 配置数据指针TUPNPPORTMAP
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPPPoE(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPPPoE)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetPPPoE, "IPC_SetPPPoE", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetQos
功    能: 获取Qos参数
参数说明: pParam [out] -- 配置数据指针TQOSINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetQos(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetQos)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetQos, "IPC_GetQos", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetQos
功    能: 设置Qos参数
参数说明: pParam [out] -- 配置数据指针TQOSINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetQos(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetQos)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetQos, "IPC_SetQos", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}
/*=================================================================
函数名称: IPC_GetKSnmp
功    能: 获取KSnmp参数
参数说明: pParam [out] -- 配置数据指针TKSNMPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetKSnmp(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetKSnmp)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetKSnmp, "IPC_GetKSnmp", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}
/*=================================================================
函数名称: IPC_SetKSnmp
功    能: 设置KSnmp参数
参数说明: pParam [out] -- 配置数据指针TKSNMPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetKSnmp(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetKSnmp)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetKSnmp, "IPC_SetKSnmp", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;	
}
/*=================================================================
函数名称: IPC_Get8021x
		  功    能: 获取8021x参数
参数说明: pParam, PT8021XINFO
	  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项
	  此处传递：<protocoltype opt=“eap-tls,eap-ttls,eap-peap,eap-leap,eap-fast,eap-md5”>
				<eapolversion opt= “1, 2”>
	  返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
==================================================================*/
IPC_API int CDECL IPC_Get8021x(void *pHandle, void *pParam,  int nLen, char* const szParamAssist, int nLenAssist, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Get8021x)(void *, void *,int, char*const, u32*);
	LOADDLLFUN5(TRUE, _IPC_Get8021x, "IPC_Get8021x", pHandle, pParam,nLen,szParamAssist, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_Set8021x
功    能: 设置802.1x参数
参数说明: pParam [out] -- 配置数据指针T8021XINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_Set8021x(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_Set8021x)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_Set8021x, "IPC_Set8021x", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API int CDECL IPC_GetUPnP (void *pHandle, void * pInfo, int nlenInfo, void * pMap, int nLenMap, int &nLenMapStruct, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetUPnP)(void *, void *,int,void*,int,int&, u32*);
	LOADDLLFUN7(TRUE, _IPC_GetUPnP, "IPC_GetUPnP", pHandle, pInfo,nlenInfo ,pMap, nLenMap,nLenMapStruct, pErrorCode);

	return ERR_NOFUN;	
}

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
IPC_API int CDECL IPC_SetUPnP (void *pHandle, void * pInfo, int nlenInfo, void * pMap, int nLenMap, int &nLenMapStruct, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetUPnP)(void *, void *,int,void*,int,int&, u32*);
	LOADDLLFUN7(TRUE, _IPC_SetUPnP, "IPC_SetUPnP", pHandle, pInfo,nlenInfo ,pMap, nLenMap,nLenMapStruct, pErrorCode);

	return ERR_NOFUN;	
}
/*=================================================================
函数名称: IPC_GetVsip
功    能: 获取Vsip参数
参数说明: pParam [out] -- 配置数据指针TKVISPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetVsip(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVsip)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_GetVsip, "IPC_GetVsip", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;	
}
/*=================================================================
函数名称: IPC_SetVsip
功    能: 设置Vsip参数
参数说明: pParam [out] -- 配置数据指针TKVISPINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetVsip(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVsip)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_SetVsip, "IPC_SetVsip", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetOnvif
功    能: 获取Onvif参数
参数说明: pParam [out] -- 配置数据指针TONVIFINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetOnvif(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetOnvif)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_GetOnvif, "IPC_GetOnvif", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetOnvif
功    能: 设置Onvif参数
参数说明: pParam [out] -- 配置数据指针TONVIFINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetOnvif(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetOnvif)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_SetOnvif, "IPC_SetOnvif", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_GetDpss
功    能: 获取Dpss参数
参数说明: dwHandle      设备句柄
		  pParam, TDPSSINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDpss(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetDpss)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetDpss, "IPC_GetDpss", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetDpss
功    能: 设置Dpss参数
参数说明: dwHandle      设备句柄
		  pParam, TDPSSINFO
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDpss(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDpss)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetDpss, "IPC_SetDpss", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetAuthMode
功    能: 获取AuthMode参数
参数说明: pParam [out] -- 配置数据指针TAUTHMODEINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAuthMode(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAuthMode)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_GetAuthMode, "IPC_GetAuthMode", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;	
}

/*=================================================================
函数名称: IPC_SetAuthMode
功    能: 设置AuthMode参数
参数说明: pParam [out] -- 配置数据指针TAUTHMODEINFO
		  nLen -- 数据长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetAuthMode(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAuthMode)(void *, void *, u32*);
	LOADDLLFUN3(TRUE, _IPC_SetAuthMode, "IPC_SetAuthMode", pHandle, pParam, pErrorCode);

	return ERR_NOFUN;	
}

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
IPC_API int CDECL IPC_GetGB28181(void *pHandle, char*byPlateid,void *pInfo,  int nLenInfo, void *pEncChnArr, int& nLenEncChnInfo, void *pAlarmChnArr,  int& nLenAlarmChnInfo,char* const szParamAssist, int nLenAssist, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetGB28181)(void*, char*, void *, int, void*, int&, void*, int&, char*const, int, u32*);
	LOADDLLFUN11(TRUE, _IPC_GetGB28181, "IPC_GetGB28181", pHandle, byPlateid, pInfo, nLenInfo, pEncChnArr, nLenEncChnInfo, pAlarmChnArr, nLenAlarmChnInfo, szParamAssist, nLenAssist, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API int CDECL IPC_SetGB28181(void *pHandle,char*byPlateid, void *pInfo,  int nLenInfo, void *pEncChnArr, int nLenEncChnInfo, void *pAlarmChnArr,  int nLenAlarmChnInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetGB28181)(void *, char*, void *, int, void*, int, void*, int, u32*);
	LOADDLLFUN9(TRUE, _IPC_SetGB28181, "IPC_SetGB28181", pHandle, byPlateid, pInfo, nLenInfo, pEncChnArr, nLenEncChnInfo, pAlarmChnArr, nLenAlarmChnInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetWifiProbe
功    能: 获取wifi探针
参数说明: pParam, TWIFIPROBEINFO
		  nLen, 数据长度,等于sizeof(TWIFIPROBEINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWifiProbe (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetWifiProbe)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetWifiProbe, "IPC_GetWifiProbe", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetWifiProbe
功    能: 设置wifi探针
参数说明: pParam, TWIFIPROBEINFO
		  nLen, 数据长度,等于sizeof(TWIFIPROBEINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWifiProbe (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetWifiProbe)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetWifiProbe, "IPC_SetWifiProbe", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetWifiProbeList
功    能: 获取wifi探针设备列表
参数说明: pParam, TWIFIPROBELISTINFO
		  nLen, 数据长度,等于sizeof(TWIFIPROBELISTINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWifiProbeList (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetWifiProbeList)(void *, void *, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetWifiProbeList, "IPC_GetWifiProbeList", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetStorageState
		  功    能: 获取磁盘状态
参数说明: pParam, PTSTORAGESTATE
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：
		  <state opt="null,disker,diskrslowish,idel,noformat,formating,identifing">
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetStorageState(void *pHandle,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetStorageState)(void *,void *,int , char* , int& ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetStorageState, "IPC_GetStorageState", pHandle,pParam,nLen, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetStorageNum
功    能: 获取磁盘个数
参数说明: 
	      nLenDisks    磁盘个数
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageNum (void *pHandle, int &nLenDisks, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetStorageNum)(void *, int& ,u32 *);
	LOADDLLFUN3(TRUE, _IPC_GetStorageNum, "IPC_GetStorageNum", pHandle, nLenDisks,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetStorageMgr
		  功    能: 获取存储管理参数
参数说明: pInfo, TSTORAGEMANAGER
	      nlenInfo, pInfo数据长度,等于sizeof(TSTORAGEMANAGER).
	      pDisks, TDISKINFO [],磁盘信息数据数组指针.
	      nLenDisks, 磁盘信息的数据个数.
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<fullmode opt=”stop,over”/>   
		  <!-- req, xs:string --> <state opt="null,disker,diskrslowish,idel,noformat,formating,identifing">
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
 =================================================================*/
IPC_API int CDECL IPC_GetStorageMgr(void *pHandle, void * pInfo, int nLenInfo, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetStorageMgr)(void *, void * , int , char* , int& ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetStorageMgr, "IPC_GetStorageMgr", pHandle,  pInfo,  nLenInfo, szParamAssist,  nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetStorageMgr
		  功    能: 设置存储管理参数
参数说明: pInfo, TSTORAGEMANAGER
	      nlenInfo, pInfo数据长度,等于sizeof(TSTORAGEMANAGER).
	      pDisks, TDISKINFO [],磁盘信息数据数组指针.
	      nLenDisks, 磁盘信息的数据个数.
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetStorageMgr(void *pHandle, void * pInfo, int nLenInfo, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetStorageMgr)(void *, void * , int , u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetStorageMgr, "IPC_SetStorageMgr", pHandle, pInfo, nLenInfo, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_DiskFomat
		  功    能: 磁盘格式化
参数说明: pParam, PTSTORAGEFOMAT
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_DiskFomat (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_DiskFomat)(void *, void *,int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_DiskFomat, "IPC_DiskFomat", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetFomatProgress
		  功    能: 磁盘格式化进度
参数说明: pParam, PTFOMATSTATE
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetFomatProgress (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetFomatProgress)(void *, void *,int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetFomatProgress, "IPC_GetFomatProgress", pHandle,pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetRecSchedule
		  功    能: 获取录像计划参数
参数说明: pParam, PTRECSCHEDULE
	  nlenInfo, pParamInfo数据长度,等于sizeof(TRECSCHEDULE).
	  szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<preparetime opt=”none,5,10,15,20,25,30,infinite”/>
	  <delaytime opt=”none,5,10,15,20,25,30,infinite”/>
	  <rectype opt=”main,assist,three”/><esps opt=”es, ps”/><time max= 8 index=1>
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetRecSchedule (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetRecSchedule)(void *, void *,int , char* , int& ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetRecSchedule, "IPC_GetRecSchedule", pHandle, pParam, nLen,  szParamAssist,  nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetRecSchedule
		  功    能: 设置录像计划参数
参数说明: pParam, PTRECSCHEDULE
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetRecSchedule (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetRecSchedule)(void *, void *,int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetRecSchedule, "IPC_SetRecSchedule", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetSnapPicConfig
		  功    能: 获取前端抓拍参数
参数说明: pParam, PTSNAPPICCONFIG
	      nlen,  pParam数据长度,等于sizeof(TSNAPPICCONFIG).
	      szParamAssist,辅助信息以xml片段的方式传递enable选项和opt选项此处传递：<type opt=”jpeg, jpg,bmp”/>
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetSnapPicCfg (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSnapPicCfg)(void *, void *,int , char* , int& ,u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetSnapPicCfg, "IPC_GetSnapPicCfg",pHandle, pParam, nLen, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetSnapPicConfig
		  功    能: 设置前端抓拍参数
参数说明: pParam, PTSNAPPICCONFIG
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetSnapPicCfg (void *pHandle,  void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSnapPicCfg)(void *, void *,int ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSnapPicCfg, "IPC_SetSnapPicCfg", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetSnapTime
功    能: 获取定时抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPTIME
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapTime (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSnapTime)(void *, void *,int& ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetSnapTime, "IPC_GetSnapTime", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetSnapTime
功    能: 设置定时抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPTIME
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapTime (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSnapTime)(void *, void *,int& ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSnapTime, "IPC_SetSnapTime", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetSnapTime
功    能: 获取事件抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPEVENT
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapEvent (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSnapEvent)(void *, void *,int& ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetSnapEvent, "IPC_GetSnapEvent", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetSnapTime
功    能: 设置事件抓拍的配置
参数说明: pHandle  前端句柄
	      pParam, PTSNAPEVENT
	      pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapEvent (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSnapEvent)(void *, void *,int& ,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSnapEvent, "IPC_SetSnapEvent", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_GetDetect(void *pHandle,  void *pParam,int nLen, void *pAreaArr,int nLenArea,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetDetect)(void *, void *, int, void*, int, char*, int&,u32*);
	LOADDLLFUN8(TRUE, _IPC_GetDetect, "IPC_GetDetect", pHandle, pParam, nLen, pAreaArr, nLenArea, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_SetDetect (void *pHandle, void *pParam,int nLen, void *pAreaArr,int nLenArea,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDetect)(void *, void *, int, void*, int,u32 *);
	LOADDLLFUN6(TRUE, _IPC_SetDetect, "IPC_SetDetect", pHandle, pParam, nLen, pAreaArr, nLenArea,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_GetShieldAlarm (void *pHandle,void *pParam,int nLen, void *pAreaArr,int nLenArea,char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetShieldAlarm)(void *, void *, int, void*, int, char*, int&,u32 *);
	LOADDLLFUN8(TRUE, _IPC_GetShieldAlarm, "IPC_GetShieldAlarm",pHandle, pParam, nLen, pAreaArr, nLenArea, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_SetShieldAlarm (void *pHandle,void *pParam,int nLen, void *pAreaArr,int nLenArea,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetShieldAlarm)(void*, void *, int, void*, int, u32*);
	LOADDLLFUN6(TRUE, _IPC_SetShieldAlarm, "IPC_SetShieldAlarm", pHandle, pParam, nLen, pAreaArr, nLenArea,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetWarningLine
功    能: 获取警戒线参数
参数说明: pParam,   TWARNINGLINEINFO
	      nLen，    TWARNINGLINEINFO长度， sizeof(TWARNINGLINEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWarningLine (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetWarningLine)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetWarningLine, "IPC_GetWarningLine", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetWarningLine
功    能: 设置警戒线参数
参数说明: pParam,   TWARNINGLINEINFO
	      nLen，    TWARNINGLINEINFO长度， sizeof(TWARNINGLINEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWarningLine (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetWarningLine)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetWarningLine, "IPC_SetWarningLine", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetAreaDetect
功    能: 获取区域入侵参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectEnter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAreaDetectEnter)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetAreaDetectEnter, "IPC_GetAreaDetectEnter", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetAreaDetect
功    能: 设置区域入侵参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectEnter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAreaDetectEnter)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAreaDetectEnter, "IPC_SetAreaDetectEnter", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetAreaDetectEntry
功    能: 获取区域进入参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectEntry (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAreaDetectEntry)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetAreaDetectEntry, "IPC_GetAreaDetectEntry", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetAreaDetectEntry
功    能: 设置区域进入参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectEntry (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAreaDetectEntry)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAreaDetectEntry, "IPC_SetAreaDetectEntry", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetAreaDetectExit
功    能: 获取区域离开参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectExit (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAreaDetectExit)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetAreaDetectExit, "IPC_GetAreaDetectExit", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetAreaDetectExit
功    能: 设置区域离开参数
参数说明: pParam,      TAREADETECTINFO
	      nLen，       TAREADETECTINFO长度， sizeof(TAREADETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectExit (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAreaDetectExit)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAreaDetectExit, "IPC_SetAreaDetectExit", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetObjectLeft
功    能: 获取物品遗留参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetObjectLeft (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetObjectLeft)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetObjectLeft, "IPC_GetObjectLeft", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetObjectLeft
功    能: 设置物品遗留参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetObjectLeft (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetObjectLeft)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetObjectLeft, "IPC_SetObjectLeft", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetObjectRemoval
功    能: 获取物品拿取参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetObjectRemoval (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetObjectRemoval)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetObjectRemoval, "IPC_GetObjectRemoval", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetObjectRemoval
功    能: 设置物品拿取参数
参数说明: pParam,      TOBJECTDETECTINFO
	      nLen，       TOBJECTDETECTINFO长度， sizeof(TOBJECTDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetObjectRemoval (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetObjectRemoval)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetObjectRemoval, "IPC_SetObjectRemoval", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetPersonAggregate
功    能: 获取人员聚集参数
参数说明: pParam,      TPERSONDETECTINFO
	      nLen，       TPERSONDETECTINFO长度， sizeof(TPERSONDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPersonAggregate (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPersonAggregate)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPersonAggregate, "IPC_GetPersonAggregate", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPersonAggregate
功    能: 设置人员聚集参数
参数说明: pParam,      TPERSONDETECTINFO
	      nLen，       TPERSONDETECTINFO长度， sizeof(TPERSONDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPersonAggregate (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPersonAggregate)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPersonAggregate, "IPC_SetPersonAggregate", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetVirtualFocus
功    能: 获取虚焦检测参数
参数说明: pParam,      TVIRTUALFOCUSINFO
	      nLen，       TVIRTUALFOCUSINFO长度， sizeof(TVIRTUALFOCUSINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetVirtualFocus (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetVirtualFocus)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetVirtualFocus, "IPC_GetVirtualFocus", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetVirtualFocus
功    能: 设置虚焦检测参数
参数说明: pParam,      TVIRTUALFOCUSINFO
	      nLen，       TVIRTUALFOCUSINFO长度， sizeof(TVIRTUALFOCUSINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetVirtualFocus (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetVirtualFocus)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetVirtualFocus, "IPC_SetVirtualFocus", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetSightChange
功    能: 获取场景变更参数
参数说明: pParam,      TSIGHTCHANGEINFO
	      nLen，       TSIGHTCHANGEINFO长度， sizeof(TSIGHTCHANGEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSightChange (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetSightChange)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetSightChange, "IPC_GetSightChange", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetSightChange
功    能: 设置场景变更参数
参数说明: pParam,      TSIGHTCHANGEINFO
	      nLen，       TSIGHTCHANGEINFO长度， sizeof(TSIGHTCHANGEINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSightChange (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSightChange)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetSightChange, "IPC_SetSightChange", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetAudioExcept
功    能: 获取声音异常参数
参数说明: pParam,      TAUDIOEXCEPTINFO
	      nLen，       TAUDIOEXCEPTINFO长度， sizeof(TAUDIOEXCEPTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAudioExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAudioExcept)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetAudioExcept, "IPC_GetAudioExcept", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetAudioExcept
功    能: 设置声音异常参数
参数说明: pParam,      TAUDIOEXCEPTINFO
	      nLen，       TAUDIOEXCEPTINFO长度， sizeof(TAUDIOEXCEPTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAudioExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAudioExcept)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAudioExcept, "IPC_SetAudioExcept", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetFaceDetect
功    能: 获取人脸检测参数
参数说明: pParam,      TFACEDETECTINFO
	      nLen，       TFACEDETECTINFO长度， sizeof(TFACEDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFaceDetect (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetFaceDetect)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetFaceDetect, "IPC_GetFaceDetect", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetFaceDetect
功    能: 设置人脸检测参数
参数说明: pParam,      TFACEDETECTINFO
	      nLen，       TFACEDETECTINFO长度， sizeof(TFACEDETECTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFaceDetect (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetFaceDetect)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetFaceDetect, "IPC_SetFaceDetect", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_GetAlarmIn(void *pHandle, char*pAlmNum, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAlarmIn)(void *,char* ,void *, int, char*, int&,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetAlarmIn, "IPC_GetAlarmIn", pHandle, pAlmNum,pParam, nLen, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetAlarmIn
功    能: 设置报警输入参数
参数说明: pParam,PTALARMININFO
	      nLen， TALARMININFO长度， sizeof(TALARMININFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAlarmIn (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAlarmIn)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAlarmIn, "IPC_SetAlarmIn", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetAlarmOut
功    能: 获取报警输出参数
参数说明: pParam, PTALARMOUT
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAlarmOut (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAlarmOut)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetAlarmOut, "IPC_GetAlarmOut", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetAlarmOut
功    能: 设置报警输出参数
参数说明: pParam, PTALARMOUT
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAlarmOut (void *pHandle,void *pParam, int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAlarmOut)(void *, void *, int,u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAlarmOut, "IPC_SetAlarmOut", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetExceptType
功    能: 获取异常类型参数
参数说明: pParam, PTEXCEPTTYPE
		  nLen， TEXCEPTTYPE长度， sizeof(TEXCEPTTYPE)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API int CDECL IPC_GetExceptType (void *pHandle, void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetExceptType)(void*, void *, int,u32*);
	LOADDLLFUN4(TRUE, _IPC_GetExceptType, "IPC_GetExceptType", pHandle, pParam, nLen,pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_GetExcept (void *pHandle, char*pExceptType ,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetExcept)(void *, char*, void *, int, char*, int&,u32 *);
	LOADDLLFUN7(TRUE, _IPC_GetExcept, "IPC_GetExcept", pHandle, pExceptType, pParam, nLen, szParamAssist, nLenAssist,pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetExcpet
功    能: 获取异常配置参数
参数说明: pParam, PTEXCEPTINFO
		  nLen， TEXCEPTINFO长度， sizeof(TEXCEPTINFO);
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetExcept)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetExcept, "IPC_SetExcept", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPtzMove
		  功    能: 设置PTZ移动(上,下,左,右,复位，停止)
参数说明: pParam,PTPTZMOVE
		  nLen， TPTZMOVE长度， sizeof(TPTZMOVE);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzMove (void *pHandle,void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PtzMove)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_PtzMove, "IPC_PtzMove", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetPtzScan
		  功    能: 设置PTZ巡航
参数说明: pParam,PTPTZSCAN
		  nLen， TPTZSCAN长度， sizeof(TPTZSCAN);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzScan (void *pHandle,void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PtzScan)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_PtzScan, "IPC_PtzScan", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_PtzZoneZoom
功    能: 设置PTZ区域缩放
参数说明: pParam,PTPTZZONEZOOM
		  nLen， TPTZZONEZOOM长度， sizeof(TPTZZONEZOOM);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzZoneZoom (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PtzZoneZoom)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_PtzZoneZoom, "IPC_PtzZoneZoom", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_PtzGotoPoint
功    能: 设置PTZ居中定位
参数说明: pParam,TPTZGOTOPOINT
		  nLen， TPTZGOTOPOINT长度， sizeof(TPTZGOTOPOINT);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_PtzGotoPoint (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PtzGotoPoint)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_PtzGotoPoint, "IPC_PtzGotoPoint", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPtzWiper
功    能: 设置雨刷开启/关闭
参数说明: pParam,TPTZWIPER
		  nLen， TPTZWIPER长度， sizeof(TPTZWIPER);
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzWiper (void *pHandle,void *pParam,int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzWiper)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzWiper, "IPC_SetPtzWiper", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ GetPtzBase
功    能: 获取云台基本参数
参数说明: 
	      pParam, PTPTZBASE
		  nLen， TPTZBASE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzBase (void *pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzBase)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzBase, "IPC_GetPtzBase", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ SetPtzBase
		  功    能: 设置云台基本参数
参数说明: pParam, PTPTZBASE
	      nLen，TPTZBASE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzBase (void *pHandle, void *pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzBase)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzBase, "IPC_SetPtzBase", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ IPC_GetPtzMainTain
功    能: 获取云台断电维护参数
参数说明: 
	      pParam, PTPTZMAINTAIN
		  nLen， TPTZMAINTAIN的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzMainTain (void *pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzMainTain)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzMainTain, "IPC_GetPtzMainTain", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ IPC_SetPtzMainTain
		  功    能: 设置云台断电维护参数
参数说明: pParam, PTPTZMAINTAIN
	      nLen，TPTZMAINTAIN的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzMainTain (void *pHandle, void *pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzMainTain)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzMainTain, "IPC_SetPtzMainTain", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ IPC_SetPtzRefactory
功    能: 设置云台恢复出厂设置
参数说明: pParam, PTPTZREFACTORY
	      nLen，TPTZREFACTORY的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzRefactory (void *pHandle, void *pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzRefactory)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzRefactory, "IPC_SetPtzRefactory", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API int CDECL IPC_GetPtzWatchOn (void *pHandle, void *pParam,int nLen,char* szParamAssist, int& nLenAssist, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzWatchOn)(void *, void *, int, char*, int&, u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetPtzWatchOn, "IPC_GetPtzWatchOn", pHandle, pParam, nLen, szParamAssist, nLenAssist, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ SetPtzWatchOn
		  功    能: 设置云台守望任务参数
参数说明: pParam, PTPTZWATCHON
	      nLen，TPTZWATCHON的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzWatchOn (void *pHandle, void *pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzWatchOn)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzWatchOn, "IPC_SetPtzWatchOn", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ GetPtzPreset
功    能: 获取预置位参数
参数说明: 
	      bySn,需要获取的预置位的序号，1~255
		  pParam, PTPTZPRESET
		  nLen，bySn为0 返回TPTZPRESET数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPreset (void *pHandle, u8 bySn, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzPreset)(void *, u8,void *, int, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetPtzPreset, "IPC_GetPtzPreset", pHandle,bySn, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_ GetPtzPresetAll
功    能: 获取全部预置位参数
参数说明: 
		  pParam, PTPTZPRESETALL
		  nLen，返回TPTZPRESETAll数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPresetAll (void *pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzPresetAll)(void *,void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzPresetAll, "IPC_GetPtzPresetAll", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetPtzPreset
		  功    能: 设置预置位参数
参数说明: pParam, PTPTZPRESET
	  返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPreset (void *pHandle, void *pParam,  int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzPreset)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzPreset, "IPC_SetPtzPreset", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ GetPtzLimit
		  功    能: 获取云台限位任务参数
参数说明: 
		  pParam, PTPTZLIMIT
		  nLen， TPTZLIMIT的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzLimit (void *pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzLimit)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzLimit, "IPC_GetPtzLimit", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ SetPtzLimit
		  功    能: 设置云台限位任务参数
参数说明: pParam, PTPTZLIMIT
	  nLen，TPTZLIMIT的长度
	  返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzLimit (void *pHandle, void *pParam, int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzLimit)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzLimit, "IPC_SetPtzLimit", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ GetPtzPathCruise
功    能: 获取巡航扫描参数
参数说明: 
	      bySn,需要获取的扫描路径的序号，1~8, 0代表获取所有信息
		  pParam, PTPTZPATHCRUISE
		  nLen，bySn为0 返回TPTZPATHCRUISE数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPathCruise (void *pHandle, u8 bySn, void *pParam,int& nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzPathCruise)(void *, u8,void *, int&, u32 *);
	LOADDLLFUN5(TRUE, _IPC_GetPtzPathCruise, "IPC_GetPtzPathCruise", pHandle,bySn, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPtzPathCruise
功    能: 设置巡航扫描参数
参数说明: pParam, TPTZPATHCRUISE[]，扫描路径信息数组
	      nLen，TPTZPATHCRUISE数组的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPathCruise (void *pHandle, u8 bySn,void *pParam,  int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzPathCruise)(void *,u8, void *, int, u32 *);
	LOADDLLFUN5(TRUE, _IPC_SetPtzPathCruise, "IPC_SetPtzPathCruise", pHandle,bySn,pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPtzPathCruiseEvent
功    能: 设置巡航扫描事件(调用巡航,停止巡航)
参数说明: pParam, TPTZPATHCRUISEEVT，巡航扫描事件
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPathCruiseEvent (void *pHandle, void *pParam,  int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzPathCruiseEvent)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzPathCruiseEvent, "IPC_SetPtzPathCruiseEvent", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API int CDECL IPC_GetTimeTask (void *pHandle, void *pParam, int nLen, void * pTimeArr, int nLenTime ,int nLenTimeStruct , char* szParamAssist, int& nLenAssist, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetTimeTask)(void *, void *, int,void*,int,int,char*,int&, u32 *);
	LOADDLLFUN9(TRUE, _IPC_GetTimeTask, "IPC_GetTimeTask", pHandle, pParam, nLen,pTimeArr,nLenTime,nLenTimeStruct, szParamAssist,nLenAssist, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API int CDECL IPC_SetTimeTask (void *pHandle, void *pParam, int nLen ,void * pTimeArr, int nLenTime ,int nLenTimeStruct, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetTimeTask)(void *, void *, int,void*,int,int, u32 *);
	LOADDLLFUN7(TRUE, _IPC_SetTimeTask, "IPC_SetTimeTask", pHandle, pParam, nLen,pTimeArr,nLenTime,nLenTimeStruct, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_ GetPtzMainTain
功    能: 获取云台坐标参数
参数说明: 
	      pParam, PTPTZCOORDINATE
		  nLen， TPTZCOORDINATE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzCoorDinate (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzCoorDinate)(void *, void *, int,char*,int&, u32 *);
	LOADDLLFUN6(TRUE, _IPC_GetPtzCoorDinate, "IPC_GetPtzCoorDinate", pHandle, pParam, nLen, szParamAssist, nLenAssist, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPtzCoorDinateEvt
功    能: 设置云台坐标事件
参数说明: 
	      pParam, PTPTZCOORDINATE
		  nLen， TPTZCOORDINATE的长度
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzCoorDinateEvt(void *pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzCoorDinateEvt)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzCoorDinateEvt, "IPC_SetPtzCoorDinateEvt", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_GetPtzRatio (void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzRatio)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzRatio, "IPC_GetPtzRatio", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_SetPtzRatio(void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzRatio)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzRatio, "IPC_SetPtzRatio", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_SetPtzCoorDinateEX(void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzCoorDinateEX)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzCoorDinateEX, "IPC_SetPtzCoorDinateEX", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
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
IPC_API BOOL32 CDECL IPC_SetAquillaPTMove(void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAquillaPTMove)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAquillaPTMove, "IPC_SetAquillaPTMove", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_SetAquillaPTZAngle(void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAquillaPTZAngle)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetAquillaPTZAngle, "IPC_SetAquillaPTZAngle", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_PtzDeviationCheck
功    能: 云台误差校验
参数说明: 
	      dwHandle      设备句柄
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_PtzDeviationCheck(void* pHandle, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_PtzDeviationCheck)(void *, u32 *);
	LOADDLLFUN2(TRUE, _IPC_PtzDeviationCheck, "IPC_PtzDeviationCheck", pHandle, pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_GetPtzDevCheckRlt(void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzDevCheckRlt)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzDevCheckRlt, "IPC_GetPtzDevCheckRlt", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetPtzPatternsInfo
		  功    能: 实现花样扫描信息获取
参数说明: pParam, PTPTZPATTERNSINFO
		  nLen,   花样扫描链表个数
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzPatternsInfo (void *pHandle, void *pParam,  int &nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzPatternsInfo)(void *, void *, int&, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzPatternsInfo, "IPC_GetPtzPatternsInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_SetPtzPatternEvt
功    能: 设置花样扫描事件
参数说明: pParam, PTPTZPATTERNEVT
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SetPtzPatternEvt (void *pHandle, void *pParam,  int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzPatternEvt)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzPatternEvt, "IPC_SetPtzPatternEvt", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetPtzHvangle
功    能: 获取视场角数据
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetPtzHVangle (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPtzHVangle)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_GetPtzHVangle, "IPC_GetPtzHVangle", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API BOOL32 CDECL IPC_SetPtzWSInfo(void* pHandle, void *pParam,int nLen, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPtzWSInfo)(void *, void *, int, u32 *);
	LOADDLLFUN4(TRUE, _IPC_SetPtzWSInfo, "IPC_SetPtzWSInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetCap
功    能: 获取能力集
参数说明: nCap -- 能力集个数
          apCapName -- 能力集的名称
		  anCapOut -- 能力集的返回值
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCap(void *pHandle, u16 nCap, char* apCapName[], u32 anCapOut[], u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetCap)(void *, u16,char *[], u32[],u32* );
	LOADDLLFUN5(TRUE, _IPC_GetCap, "IPC_GetCap", pHandle, nCap, apCapName, anCapOut,pErrorCode);

	*pErrorCode = ERR_NOFUN;
	return false;
}

IPC_API int CDECL IPC_AddConnectDetect(void *pHandle, u32 dwConnectTimeOut, u32 dwReConnectTimes, cbfConnectDetect pcbfFun, void* pContext,u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_AddConnectDetect)(void*, u32, u32, cbfConnectDetect, void*, u32*);
	LOADDLLFUN6(TRUE, _IPC_AddConnectDetect, "IPC_AddConnectDetect", pHandle, dwConnectTimeOut, dwReConnectTimes, pcbfFun, pContext, pErrorCode);

	return ERR_NOFUN;
}


IPC_API int CDECL IPC_DelConnectDetect(void *pHandle, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_DelConnectDetect)(void*, u32*);
	LOADDLLFUN2(TRUE, _IPC_DelConnectDetect, "IPC_DelConnectDetect", pHandle, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_IsConnect
功    能: 连接状态判断
          
参数说明:
		 dwHandle      设备句柄
		 bIsConnect	   连接状态
		 pErrorCode    错误码指针

返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_IsConnect(u32 dwHandle, bool& bIsConnect, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_IsConnect)(u32,bool&,u32*);
	LOADDLLFUN3(TRUE, _IPC_IsConnect, "IPC_IsConnect", dwHandle,bIsConnect,pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_StartRealPlay
功    能: 浏览请求参数(TCP)
参数说明: pParam[in]--TRTSPVIDEOPARAM
		  nLen --输入结构体长度
		  pInfoOut[out]--TPLAYVIDEOINFO
		  nLenInfo[out] --输出结构体
		  pErrorCode   错误码
返 回 值: UDP:成功播放id,失败返回0，原因解析pErrorCode,TCP:成功播放返回id,失败依据pErrorCode是否为RET_FAILD
=================================================================*/
IPC_API u32 CDECL IPC_StartRealPlay(void *pHandle, emPlayVideoType eType,void* pParam, int nLen,void*pInfoOut,int& nLenInfo,u32* pErrorCode)
{
	typedef  u32  (CDECL* _IPC_StartRealPlay)(void *, emPlayVideoType ,void* , int ,void*,int& ,u32* );
	LOADDLLFUN7(TRUE, _IPC_StartRealPlay, "IPC_StartRealPlay", pHandle,  eType, pParam,  nLen,pInfoOut,nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

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
IPC_API u32 CDECL IPC_SetRtspMetaPort(void *pHandle, emPlayVideoType eType, bool bVideo, void* pParam, int nLen,void*pInfoOut,int& nLenInfo,u32* pErrorCode)
{
	typedef  u32  (CDECL* _IPC_SetRtspMetaPort)(void *, emPlayVideoType ,bool, void* , int ,void*,int& ,u32* );
	LOADDLLFUN8(TRUE, _IPC_SetRtspMetaPort, "IPC_SetRtspMetaPort", pHandle,  eType,bVideo, pParam,  nLen,pInfoOut,nLenInfo, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_RemoveRtspMetaPort
功    能: 去除Rtsp Meta端口
参数说明: 
		  dwPlayID      播放ID
		  pErrorCode    错误码
返 回 值: 成功播放id,失败返回0，原因解析pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_RemoveRtspMetaPort(void *pHandle, u32 dwPlayID, u32* pErrorCode)
{
	typedef  u32  (CDECL* _IPC_RemoveRtspMetaPort)(void *, u32,u32* );
	LOADDLLFUN3(TRUE, _IPC_RemoveRtspMetaPort, "IPC_RemoveRtspMetaPort", pHandle, dwPlayID, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_StopVideo_RTSP
功    能: 停止浏览请求参数(TCP)
参数说明: pParam[in]--TRTSPPLAYVIDEOINFO
		  nLen --输入结构体长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopRealPlay(void *pHandle,emPlayVideoType eType,u32 dwPlayID, u32* pErrorCode)
{
	typedef  bool  (CDECL* _IPC_StopRealPlay)(void *,emPlayVideoType ,u32 , u32* );
	LOADDLLFUN4(TRUE, _IPC_StopRealPlay, "IPC_StopRealPlay", pHandle, eType,dwPlayID, pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_GetCompileTime
功    能: 获取ipcsdk库编译时间
          
参数说明:
		  szCompileTime sdk库编译时间
		  pErrorCode   	错误码
返回值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCompileTime(char* szCompileTime,u32*pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetCompileTime)( char*, u32* );
	LOADDLLFUN2(TRUE, _IPC_GetCompileTime, "IPC_GetCompileTime",szCompileTime , pErrorCode);

	return ERR_NOFUN;
}
/*=================================================================
函数名称: IPC_AssTeleZoomInfo
功    能: 远端放大
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_AssTeleZoomInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_AssTeleZoomInfo)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_AssTeleZoomInfo, "IPC_AssTeleZoomInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetAssTeleZoomInfo
功    能: 远端放大
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAssTeleZoomInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAssTeleZoomInfo)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetAssTeleZoomInfo, "IPC_GetAssTeleZoomInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_AssMalfInfo
功    能: 畸形矫正
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_AssMalfInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_AssMalfInfo)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_AssMalfInfo, "IPC_AssMalfInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetAssMalfInfo
功    能: 畸形矫正
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)    
返 回 值: 成功返回ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GetAssMalfInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetAssMalfInfo)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetAssMalfInfo, "IPC_GetAssMalfInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SetMTCF
功    能: 设置工作模式
参数说明: pParam, TMTCFPARAM
		  nLen, 数据长度,等于sizeof(TMTCFPARAM)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetMTCF (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetMTCF)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetMTCF, "IPC_SetMTCF", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetMTCF
功    能: 获取工作模式
参数说明: pParam, TMTCFPARAM
		  nLen, 数据长度,等于sizeof(TMTCFPARAM)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetMTCF (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetMTCF)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetMTCF, "IPC_GetMTCF", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GetBatteryStatus
功    能: 获取电池电量
参数说明: pParam, TBATSTATUSINFO
		  nLen, 数据长度,等于sizeof(TBATSTATUSINFO)
		  pErrorCode   错误码
返 回 值: 成功返回true, 失败返回false，原因解析pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBatteryStatus (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetBatteryStatus)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetBatteryStatus, "IPC_GetBatteryStatus", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetSSH (void *pHandle, bool bOpen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetSSH)(void*, bool, u32*);
	LOADDLLFUN3(TRUE, _IPC_SetSSH, "IPC_SetSSH", pHandle, bOpen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API int CDECL IPC_SetColorSize (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetColorSize)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetColorSize, "IPC_SetColorSize", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API int CDECL IPC_GetColorSize (void *pHandle, void *pParam,  int nLen, char* szParamAssist, int& nLenAssist, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetColorSize)(void*, void*, int, char*, int&, u32*);
	LOADDLLFUN6(TRUE, _IPC_GetColorSize, "IPC_GetColorSize", pHandle, pParam, nLen, szParamAssist, nLenAssist, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_GetGpsInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetGpsInfo)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetGpsInfo, "IPC_GetGpsInfo", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_SethotPointState
功    能: 设置热点追踪状态
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_SethotPointState (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SethotPointState)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SethotPointState, "IPC_SethotPointState", pHandle, pParam,nLen, pErrorCode);

	return ERR_NOFUN;
}

/*=================================================================
函数名称: IPC_GethotPointState
功    能: 获取热点追踪状态
参数说明: pParam, PTPTZHVANGLE  
返 回 值: 成功返回RET_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int CDECL IPC_GethotPointState (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GethotPointState)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GethotPointState, "IPC_GethotPointState", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}


IPC_API BOOL32 CDECL IPC_GetMobilenetwork(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetMobilenetwork)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetMobilenetwork, "IPC_GetMobilenetwork", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetMobilenetwork(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetMobilenetwork)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetMobilenetwork, "IPC_SetMobilenetwork", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_GetPubsec(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetPubsec)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetPubsec, "IPC_GetPubsec", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetPubsec(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetPubsec)(void*, void*, int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetPubsec, "IPC_SetPubsec", pHandle, pParam, nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetAzimuth (void *pHandle, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetAzimuth)(void*, u32*);
	LOADDLLFUN2(TRUE, _IPC_SetAzimuth, "IPC_SetAzimuth", pHandle, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_GetBasicPos (void *pHandle,void *pParam,  int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_GetBasicPos)(void*,void*,int, u32*);
	LOADDLLFUN4(TRUE, _IPC_GetBasicPos, "IPC_GetBasicPos", pHandle,pParam,nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetDeployPicStart(void *pHandle, int nParam, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDeployPicStart)(void*,int, u32*);
	LOADDLLFUN3(TRUE, _IPC_SetDeployPicStart, "IPC_SetDeployPicStart", pHandle,nParam, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetDeployPicUpload(void *pHandle, void* pParam, int nLen, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDeployPicUpload)(void*,void*,int, u32*);
	LOADDLLFUN4(TRUE, _IPC_SetDeployPicUpload, "IPC_SetDeployPicUpload", pHandle,pParam,nLen, pErrorCode);

	return ERR_NOFUN;
}

IPC_API BOOL32 CDECL IPC_SetDeployPicStop(void *pHandle, u32 *pErrorCode)
{
	typedef  bool  (CDECL* _IPC_SetDeployPicStop)(void*,u32*);
	LOADDLLFUN2(TRUE, _IPC_SetDeployPicStop, "IPC_SetDeployPicStop", pHandle, pErrorCode);

	return ERR_NOFUN;
}


