//////////////////////////////////////////////////////////////////////////
// ipcdefine.h 
#ifndef IPCDEFINE_H_
#define IPCDEFINE_H_

#pragma pack(1)

/***************************************************************************************
					类型定义
****************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	  /* Type definition */
/*-----------------------------------------------------------------------------
系统公用文件，开发人员严禁修改
------------------------------------------------------------------------------*/

typedef int		s32,BOOL32;
typedef unsigned long   u32;
typedef unsigned char	u8;
typedef unsigned short  u16;
typedef short           s16;
typedef char            s8;

#ifdef _MSC_VER
typedef __int64			s64;
#else 
typedef long long		s64;
#endif 

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else 
typedef unsigned long long u64;
#endif

#ifndef _MSC_VER
#ifndef LPSTR
#define LPSTR   char *
#endif
#ifndef LPCSTR
#define LPCSTR  const char *
#endif
#endif



#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#ifndef CDECL
#ifdef WIN32
#define CDECL  __cdecl
#else
#define CDECL
#endif
#endif

/********************************************************************************/

/**********************************MAXMIN VALUE************************************/
#define MAX_USER_NAME                64   // 最大用户名长度(64位)
#define MAX_USER_PASSWORD            64   // 最大用户密码长度(64位)
#define MAX_DEV_NAME				 64   // 最大设备名称长度(64位)
#define MAX_PROTOCOL_NAME			 32   // 最大协议名称长度(32位)


#ifndef MAX_PATH
#define MAX_PATH 260
#endif

/************************************errcode************************************/					
#define RET_FAILD                       (0)    // 失败
#define RET_SUCCESS                     (1)    // 成功返回
#define ERR_NODLL						(2)    // 没有找到SDKDLL
#define ERR_NOFUN						(3)    // 没有找到SDKDLL对应接口
#define ERR_INITFAILD                   (4)    // 初始化失败
#define ERR_RELEASE                  	(5)    // 释放失败
#define ERR_AUCFAILD					(6)    // 获取鉴权信息失败
#define ERR_MEMERYFAILD					(7)    // 获取内存失败
#define ERR_PARAM						(8)    // 参数错误
#define ERR_HTTPSEND					(9)	   // http发送失败
#define ERR_ANALVZE						(10)   // 解析返回数据失败
#define ERR_UPLOADING					(11)   // 设备正在升级
#define ERR_NOLOGIN					    (12)   // 没有注册
#define ERR_NODEFINE                    (13)   // 功能模块没有定义

#define ERR_UNKNOW						(201)  //CGI App未知错误
#define ERR_USERNAME					(202)  //用户名错误(核心服务层用户不存在)
#define ERR_NOT_AUTH					(203)  //用户信息未验证
#define ERR_PASS						(204)  //用户密码错误
#define ERR_AUTHID						(205)  //鉴权ID错误
#define ERR_NO_POWER					(206)  //没有权限执行操作
#define ERR_IP_DENY					    (207)  //IP被拒绝
#define ERR_OLDPASS				    	(208)  //旧密码错误
#define ERR_USER_EXIST					(209)  //用户已存在
#define ERR_URL_NO_SUPPORT				(210)  //此CGI请求不支持

#define ERR_NEED_BODY                   (211)  //请求没有携带消息体
#define ERR_XML                     	(212)  //CGI请求携带的XML格式错误
#define ERR_XML_ROOT                	(213)  //CGI请求携带XML错误（请求的URL与XML内的根节点不匹配）
#define ERR_PARAM_LOST                  (214)  //CGI请求携带的XML中缺少必要的参数（XML元素）
#define ERR_PARMM_TOOLONG               (215)  //XML中携带的参数过长
#define ERR_CHECK                   	(216)  //升级信息检查错误
#define ERR_FILE                    	(217)  //升级文件检查错误, 重发当前的包
#define ERR_UPGRADE_FAILURE             (218)  //设备升级失败
#define ERR_XML_PARAM                   (219)  //XML参数错误
#define ERR_IP                      	(220)  //IP地址格式错误
#define ERR_MASK                    	(221)  //IP掩码错误
#define ERR_GATEWAY                 	(222)  //IP网关错误
#define ERR_DNS                     	(223)  //DNS地址错误
#define ERR_MULTICAST               	(224)  //组播地址错误
#define ERR_MTU                     	(225)  //MTU参数错误
#define ERR_SEQ                     	(226)  //操作时序错误
#define ERR_URL                     	(227)  //请求的URL格式错误
#define ERR_INVALID_ARG                 (228)  //无效的信令
#define ERR_DEVICE_BUSY                 (229)  //设备正忙
#define ERR_RECOVER                     (230)  //设备恢复
#define ERR_USER_BANNED                 (231)  //用户被禁止登录
#define ERR_USER_ACTIVE                 (232)  //用户需要激活
#define ERR_NEED_REQUERY                (233)  //需要重新查询

#define ERR_NOFORMAT                    (2509) // 前端正在录像不能格式化

/********************************************************************************/

/************************************event************************************/	

#define EVT_PTZ_BEGIN                  	(0)    			// PTZ事件定义起始位置				
#define EVT_PTZ_FOUCUS                  (EVT_PTZ_BEGIN + 1)    // PTZ焦点事件


/********************************************************************************/

/************************************cap************************************/	

#define CAP_ANONYM                  	"anonym"    	// 红外灯能力集
#define CAP_HELP                  		"help"    		// 帮助
#define CAP_VIDEOSOURCENUM              "videosourcenum"// 视频源数量
#define CAP_STREAMNUM                  	"streamnum"    	// 码流路数
#define CAP_AUDIOENC                  	"audioenc"    	// 音频编码能力
#define CAP_AUDIODEC                  	"audiodec"    	// 音频解码能力
#define CAP_REC                  		"rec"    		// 支持录像功能
#define CAP_IMAGE                  		"image"    		// 支持图像调节功能
#define CAP_BRIGHTNESS                  "brightness"    // 亮度
#define CAP_CONTRAST                  	"contrast"    	// 对比度
#define CAP_SATURATION                 	"saturation"    // 饱和度
#define CAP_HUE                 		"hue"    		// 锐度
#define CAP_FOCUS                 		"focus"    		// 支持聚焦
#define CAP_WHITEBALANCE                "whitebalance"  // 白平衡 
#define CAP_DYNAMICMODE                 "dynamicmode"   // 支持动态调节
#define CAP_IMAGEENHANCE                "imageenhance"  // 图像增强
#define CAP_WDR                 		"wdr"    		// 宽动态
#define CAP_BLC                 		"blc"    		// 强光抑制
#define CAP_HLC                 		"hlc"    		// 背光补偿
#define CAP_FOGTHROUGH                 	"fogthrough"    // 透雾
#define CAP_GAMMA                 		"gamma"    		// Gamma
#define CAP_SMARTIR                 	"smartir"    	// SmartIR
#define CAP_IRIS                 		"iris"    		// 光圈
#define CAP_IRCUTFILTER                 "ircutfilter"   // 日夜切换
#define CAP_SHUTTER                 	"shutter"    	// 支持快门
#define CAP_GAIN                 		"gain"    		// 增益
#define CAP_CORRIDORMODE                "corridormode"  // 支持走廊模式
#define CAP_IMAGEMODE                 	"imagemode"    	// 镜像模式
#define CAP_NOISEREDUCE2D               "noisereduce2d" // 2D降噪 
#define CAP_NOISEREDUCE3D               "noisereduce3d" // 3D降噪
#define CAP_LOCALECHO                 	"localecho"    	// 支持本地回显
#define CAP_HDSDI                 		"hdsdi"    		// HDSDI
#define CAP_POWERLINEFREQUENCY          "powerlinefrequency"// 防闪烁
#define CAP_IMAGETRUN                 	"imagetrun"    	// 图像翻转
#define CAP_ENCCLIP                 	"encclip"    	// 编码裁剪
#define CAP_ROI                 		"roi"    		// 敏感区域
#define CAP_VIDEOSHILD                 	"videoshild"    // 视频遮蔽
#define CAP_QOS                 		"qos"    		// Qos
#define CAP_AUDIODEC                 	"audiodec"    	// 音频解码
#define CAP_AUDIOENC                 	"audioenc"    	// 音频编码
#define CAP_OSD                 		"osd"    		// 字幕
#define CAP_DYNAMICOSD                 	"dynamicosd"    // 动态OSD
#define CAP_MTCF                 		"mtcf"    		// mtcf 工作模式
#define CAP_PTZ                  		"ptz"    		// 云台
#define CAP_PRESET                 		"preset"    	// 预置位
#define CAP_WATCHON                 	"watchon"    	// 云台守望
#define CAP_TIMETASK                 	"timetask"    	// 定时任务
#define CAP_PATHCRUISE                 	"pathcruise"    // 巡航扫描
#define CAP_PATTERNS                 	"patterns"    	// 花样扫描
#define CAP_LIMIT                 		"limit"    		// 限位
#define CAP_LASER                 		"laser"    		// 激光
#define CAP_WIPER                 		"wiper"    		// 雨刷, 
#define CAP_SHAKE                 		"shake"    		// 防抖, 
#define CAP_ESHAKE                 		"eshake"    	// 电子防抖
#define CAP_DEFRONST                 	"defronst"    	// 除霜,
#define CAP_ASSFOCUS                 	"assfocus"    	// 辅助调焦
#define CAP_INFRARED                 	"infrared"    	// 红外 
#define CAP_ANALYZE                 	"analyze"    	// 视频分析
#define CAP_DETECT                 		"detect"    	// 移动侦测区域数量，0不支持
#define CAP_SHIELDALARM                 "shieldalarm"   // 遮蔽报警区域数量，0不支持
#define CAP_ALARMIN                 	"alarmin"    	// 报警输入数量，0不支持
#define CAP_ALARMOUT                 	"alarmout"    	// 报警输出数量，0不支持
#define CAP_EXCEPTION                 	"exception"    	// 异常联动  
#define CAP_MULCARD                 	"mulcard"    	// 网卡数量
#define CAP_WIRELESS                 	"wireless"    	// 否支持无线
#define CAP_PPPOE                 		"pppoe"    		// PPPoE支持
#define CAP_DDNS                 		"ddns"    		// DDNS支持
#define CAP_8021X                 		"8021x"    		// 8021X支持
#define CAP_VISP                 		"visp"    		// Kedavsip
#define CAP_ONVIF                 		"onvif"    		// Onvif
#define CAP_GB28181                 	"gb28181"    	// Gb28181
#define CAP_KSNMP                 		"ksnmp"    		// Ksnmp
#define CAP_IPADDR                 		"ipaddr"    	// ip
#define CAP_WIFI                 		"wifi"    		// wifi
#define CAP_BLUETOOTH                 	"bluetooth"    	// 蓝牙
#define CAP_AUTOMAINTAIN                "automaintain"  // 自动维护
#define CAP_FIND                 		"find"    		// 发现
#define CAP_RS232                 		"rs232"    		// rs232
#define CAP_RS485                 		"rs485"    		// rs485
#define CAP_STORAGE                 	"storage"    	// 存储能力集
#define CAP_SNAPPIC                 	"snappic"    	// 抓图
#define CAP_LOG                			"log"    		// 日志
#define CAP_SECURITY                 	"security"    	// 安全



/********************************************************************************/

/***********************************enum************************************/

// 码流分辨率
enum ERESOLUTION 
{
	ERESOLUTION_1024_768,		// 1024× 768
	ERESOLUTION_1920_1280,		// 1920× 1280
};


// UDP视频浏览参数
enum emStreamType
{
	em_streamtype_vedio = 0,	// 视频
	em_streamtype_audio,		// 音频
	em_streamtype_both,			// 视频 + 音频
	em_streamtype_ps			// PS流
};

// 呼叫传输类型
enum emTransType
{
	em_transtype_rtp_udp = 0,	// rtp-udp
	em_transtype_rtp_tcp,		// rtp-tcp
	em_transtype_rtsp_tcp,		// rtsp-tcp
	em_transtype_rtsp_udp		// rtsp-udp
};

// 码流传输类型
enum emPlayVideoType
{
	type_udp =  0,     // udp
	type_tcp,          // tcp
	type_unknow,       
};


// 连接状态
enum emConnectState
{
	em_connect_ok = 0,	// 连接成功
	em_connect_no,		// 连接失败
};

// IP分配方式
enum emIpAddrType
{
	em_ipaddr_static = 0,//静态
	em_ipaddr_dymatic,   //动态
};

// IP地址的类型
enum emIPAddressType
{
	em_ipaddrtype_ip = 0,		// 整型，dword值
	em_ipaddrtype_url			// url地址，字符串
};
// 本地录像文件保存格式
enum emLocalRecType
{
	em_localrectype_asf = 0,     //asf格式
};
// 图片编码类型 
enum emPictureType
{
	
	em_picture_jpeg = 0,// jpeg格式
	em_picture_bmp,		// bmp格式
};
// 录像事件类型
enum emRecType
{
	em_record_alarm = 0, //事件录像
	em_record_time,      //时间录像
	em_record_manual,    //手动录像
};
// 录像播放速度
enum emRecPlayRate
{
	em_recplay_rate1 = 0,// 初始速度
	em_recplay_rate2,    // 2倍速度
	em_recplay_rate4,    
	em_recplay_rate8,
	em_recplay_rate16,
	em_recplay_rate1_2,  // 1/2倍速度
	em_recplay_rate1_4,
	em_recplay_rate1_8,
};

// 自动校时模式
enum EAUTOTIMEMODE 
{
	em_autotime_ntp = 0,			// 1024× 768
	em_autotime_inputprotocol,			// 1920× 1280
};
/********************************************************************************/

/**********************************升级状态t************************************/
enum emUploadState
{
	em_uploadstate_idle = 0,     // 空闲
	em_uploadstate_checkdata,    // 校验
	em_uploadstate_pre,			 // 准备升级
	em_uploadstate_uploading,	 // 正在升级
};

/********************************************************************************/

/***********************************port************************************/
#define PORT_HTTP                  (80)   // http端口
#define PORT_RTSP                  (554)  // http端口
#define PORT_PRINT                 (3700)  // 本地打印端口


/********************************************************************************/

/**********************************default************************************/
#define DEF_                  (80)   // 


/********************************************************************************/


/**********************************Normal define***********************************/
#define NORMAL_BEGIN            0   // 
#define VIEW_TCP               	(NORMAL_BEGIN + 2)   // TCP
#define VIEW_UDP                (NORMAL_BEGIN + 3)   // UDP浏览
#define REC_TCP               	(NORMAL_BEGIN + 2)   // TCP
#define VIEW_UDP                (NORMAL_BEGIN + 3)   // UDP浏览

/*********************************IPCSDK回调原型定义e***********************************/


/*=================================================================
函数名称: cbfConnectDetect
功    能: 连接探测回调回调    ，IPC_AddConnectDetect
参数说明: 
		  dwIP		-- 设备IP 
		  wPort		-- 设备http端口 
		  dwCBconnectType	-- 连接状态 emConnectState
		  dwData	-- 回传数据
		  dwDataLen -- 回传数据长度
		  pContext	-- 上下文
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
#ifdef WIN32
typedef void (__stdcall *cbfConnectDetect)(u32 dwIP, u16 wPort, u32 dwHandle,u32 dwCBconnectType, u32 dwDataLen, u32 dwData, void* pContext);
#else
typedef void (CDECL *cbfConnectDetect)(u32 dwIP, u16 wPort, u32 dwHandle,u32 dwCBconnectType, u32 dwDataLen, u32 dwData, void* pContext);
#endif



#pragma pack()

#endif
