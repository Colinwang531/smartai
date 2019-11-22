//////////////////////////////////////////////////////////////////////////
// ipcv7struct.h 

#ifndef IPCSTRUCT_H_
#define IPCSTRUCT_H_

#pragma warning(disable: 4103)
#pragma pack(push, 1)
//#include "ipcdefine.h"

#define MAX_USER_NAME                64   // 最大用户名长度(64位)
#define MAX_USER_PASSWORD           64   // 最大用户密码长度(64位)
#define MAX_EXPAND_PATH              1024 // 最大扩展长度（1024）
// Handle关联
typedef struct tagHandleParam
{
	unsigned long dwIpAddr;
	unsigned short wPort;
	char szUserNmae[MAX_USER_NAME];
	char szUserPassword[MAX_USER_PASSWORD];
	u32 dwReserved;
} THANDLEPARAM, *PTHANDLEPARAM;
// 网络参数
typedef struct tagNetParam
{
	unsigned long   dwIpAddr;	                                      // IP地址(网络序)
	unsigned short  wPort;		                                      // 端口号(1024-65535)
} TNET_PARAM, *PTNET_PARAM;

// 浏览返回结构体参数(UDP)
typedef struct tagPlayVideoInfo
{
	u16	wRtcpVideoPort;	     // 返回的视频rtcp端口(穿net使用)
	u16 wRtcpAudioPort;	     // 返回的音频rtcp端口(穿net使用) 
}TPLAYVIDEOINFO, *PTPLAYVIDEOINFO;


// 浏览结构体参数
typedef struct tagPlayVideoParam
{
	u8   byVideoSouce;		// 视频源ID
	char szStreamType[17];	// 请求的码流类型 ---vedio,audio,both,ps
	char szTransType[17];	// 传输协议类型   ---rtp_udp,rtp_tcp,rtsp_tcp,rtsp_udp
	u8   byStreamChn;		// 码流通道
	char szAdmin[32];		// 前端返回的账号
	char szPassword[32];	// 前端返回的密码
	u32  dwSrcIP;			// 本地接收IP
	u16	 wRtspPort;			// 前端监听端口
	u16  wRtpVideoPort;		// 视频 RTP端口
	u16  wRtcpVideoPort;	// 视频 RTCP端口
	u16  wRtpAudioPort;		// 音频 RTP端口
	u16  wRtcpAudioPort;	// 音频 RTP端口
	u16  wRtpAudioPort2;	// 音频2 RTP端口
	u16  wRtcpAudioPort2;	// 音频2 RTP端口
	char szMediaURL[MAX_PATH];// url
	u32	 hPlayWnd;			// 播放窗口句柄
}TPLAYVIDEOPARAM, *PTPLAYVIDEOPARAM;

// 呼叫状态
typedef struct tagRTCPCallStateInfo
{
	bool bCallState;
}TRTCPCALLSTATEINFO, PRTCPCALLSTATEINFO;

// 准备呼叫返回结构体参数
typedef struct tagRTCPPrepareCallInfo 
{
	u32	dwID;
	u16	wRtpPort;
	u16 wRtcpPort;
	char szStreamType[17];
}TRTCPPREPARECALLINFO, *PRTCPPREPARECALLINFO;

// RTSP浏览信息
typedef struct tagRTSPParam
{
	u8	byVideoSource;	// 视频源ID
	u16 wVideoChanID;	// 码流通道
}TRTSPPARAM, *PRTSPPARAM;

// RTSP 录像信息
typedef struct tagRTSPRecParam
{
	u8	byVideoSource;	// 视频源ID
	u16 wVideoChanID;	// 码流通道
	u32 nPlayID;		// 播放ID
	u32 nID;			// 录像块ID
	char  szStartTime[32];	// 录像起始时间
	char  szEndTime[32];		// 录像结束时间
	char  szSeekTime[32];		// 录像播放时间
}TRTSPRECPARAM, *PRTSPRECPARAM;

// RTSP浏览返回信息
typedef struct tagRTSPInfo
{
	char szurl[MAX_PATH];
	u16	wRtspPort;	
	bool bDoubleAudio;  // 是否支持双音频
}TRTSPINFO, *PRTSPINFO;

// 音量参数
typedef struct tagRTCPVideoVolumeInfo
{
	bool bAudioEnable;		// 是否有音频
	u8	 byVolume;			// 音量值
}TRTCPVIDEOVOLUMEINFO, *PRTCPVIDEOVOLUMEINFO;

//视频
typedef struct tagMultiVideoModeInfo
{
	char szMode[17];	//单码流，双码流，三码流，四码流， "1,2,3,4"
}TMULTIVIDEOMODEINFO, *PTMULTIVIDEOMODEINFO;

typedef struct tagMultiVideoModeParam
{
	u8	byVideoSource;
	TMULTIVIDEOMODEINFO	tMultiVideoInfo;
}TMUTIVIDEOMODEPARAM, *PTMULTIVIDEOMODEPARAM;

// 视频编码
typedef struct tagVideoFrameCapList
{
	char szFrameRateCap[64];			// 编码格式能力
	u8	 byMin;							// 最小编码帧率
	u8	 byMax;							// 最大编码帧率
	u8	 byDef;							// 默认编码帧率
}TVIDEOFRAMECAPLIST, *PVIDEOFRAMECAPLIST;

typedef struct tagVideoCapList
{
	u8	 byStreamTypeCap;				// 码流类型
	u8	 byStreamIdCap;					// 码流ID
	char szResolutionCap[128];			// 分辨率类型能力
	char szEncoderComplexityCap[64];	// 编码复杂度能力
	TVIDEOFRAMECAPLIST tVideoCapList[32]; //帧率能力链表
}TVIDEOCAPLIST, *PVIDEOCAPLIST;

typedef struct tagVideoEnc
{
	char		szResolution[17];		// 当前分辨率
	char		szDataRateType[17];		// 数据格式类型
	u16			wMaxDataRate;			// 码率上限(最大最小值详见接口的szAssistInfo变量)
	char		szQuality[17];			// opt=”lowest,lower,low,middle,hight,highest”
	u8			byFrameRate;			// (能力详见tagVideoFrameCapList)
	char		szVideoType[17];		//(具体能力详见接口的szAssistInfo变量)
	char		szEncoderComplexity[17];
	u16			wIFrameSpeed;			//i帧间隔(最大最小值详见接口的szAssistInfo变量)
}TVIDEOENC, *PTVIDEOENC;

typedef struct tagVideoEncInfo
{
	TVIDEOCAPLIST	tVideoCapList[15];	// 最多15个
	TVIDEOENC		tVideoEnc;
}TVIDEOENCINFO, *PTVIDEOENCINFO;

typedef struct tagVideoEncParam
{
	u8	byVideoSource;
	u8	byStreamId;		// 主码流(1),辅码流(2),三码流(3),四码流(4)
	u8  byStreamType;   // 单码流(1),双码流(2),三码流(3),四码流(4)
	TVIDEOENC	tVideoEnc;
}TVIDEOENCPARAM, *PTVIDEOENCPARAM;

// 音频编码
typedef struct tagAudioEncInfo
{
	u8	byVolume;	// 编码音量0-100
	bool  bAec;		// 混音录像
	char  szType[17];	// 编码格式“PCMA/PCMU/ADPCM/G722/G728/G7221c/AACLC”
	char  szInputMode[17];	//音频输入模式“linein,micin”
	char  szSampling[17];	//当前采样率
}TAUDIOENCINFO, *PTAUDIOENCINFO;

typedef struct tagAudioEncParam
{
	u8	byVideoSource;
	u8	byStreamId;		// 主码流(1),辅码流(2),三码流(3),四码流(4)
	TAUDIOENCINFO	tAudioEncInfo;
}TAUDIOENCPARAM, *PTAUDIOENCPARAM;

// 音频解码
typedef struct tagAudioDecInfo
{
	u8	byVolume;		// 解码音量 0-100
	bool bMixerRec;		// 混音录像
	bool bModeEnable;	// 音频输入是否启用
	char szTypeList[64];	//输出类型列表, 只读
	char szOutputType[17];	//输出类型 lineout,speaker
}TAUDIODECINFO, *PTAUDIODECINFO;

typedef struct tagAudioDecParam
{
	u8	byVideoSource;
	u8	byStreamId;		// 主码流(1),辅码流(2),三码流(3),四码流(4)
	TAUDIODECINFO	tAudioDecInfo;
}TAUDIODECPARAM, *PTAUDIODECPARAM;

// 图像遮蔽
typedef struct tagShieldArea
{
	char szName[17];	// 区域类型名字 目前不支持
	char szColor[17];	// 区域的颜色"gray,red,yellow,blue,orange,green,transparent,half-transparent,mosaic" 目前不支持
	char szType[17];	// 目前支持rect
	u8	byPointNum;		// 点的个数，rect为4个点
	u16	x[8];			// x
	u16	y[8];			// y
}TSHIELDAREA, *PTSHLELDAREA;

typedef struct tagVideoShield
{
	bool bEnable;		// 启用
	u16	 wVgaWidth;		// 设置区域宽度，默认为10000
	u16	 wVgaHeight;	// 设置区域高度，默认为10000
	u16	 wMinWidth;		// 最小区域宽度，不可修改
	u16	 wMinHeight;	// 最小区域高度，不可修改
	char szAllColor[17];// 所有区域的颜色gray,red,yellow,blue,orange,green,transparent,half-transparent,mosaic
	u8	 byAreaNum;		// 区域个数，目前最多支持4个
}TVIDEOSHIELD, *PTVIDEOSHIELD;

typedef struct tagVideoShieldInfo
{	
	TVIDEOSHIELD	tVideoShield;
	TSHIELDAREA		tShieldArea[4];		// 目前支持4个区域
}TVIDEOSHIELDINFO, *PTVIDEOSHIELDINFO;

typedef struct	tagVideoShieldParam
{
	u8		byVideoSource;
	TVIDEOSHIELD	tVideoShield;
	TSHIELDAREA		tShieldArea[4];		// 目前支持4个区域
}TVIDEOSHIELDPARAM, *PVIDEOSHIELDPARAM;

// ROI
typedef struct tagVideoRoi
{
	bool	bEnable;	// 启用
	u16		wVgaWidth;	// 设置区域宽度，默认为10000
	u16		wVgaHeight;	// 设置区域高度，默认为10000
	u16		wMinWidth;	// 最小区域宽度，不可修改--1250
	u16		wMinHeight;	// 最小区域高度，不可修改--1250
	char	szCodeLevel[17]; // low , middle , high
}TVIDEOROI, *PTVIDEOROI;

typedef struct tagRoiArea
{
	char szType[17];	// 目前支持rect
	u8	byPointNum;		// 点的个数，rect为4个点
	u16	x[4];			// x
	u16	y[4];			// y
}TROIAREA, *PTROIAREA;

typedef struct tagVideoRoiInfo
{	
	TVIDEOROI		tVideoRoi;
	TROIAREA		tRoiArea;		// 目前支持1个区域
}TVIDEOROIINFO, *PTVIDEOROIINFO;

typedef struct	tagVideoRoiParam
{
	u8		byVideoSource;
	TVIDEOROI	tVideoRoi;
	TROIAREA	tRoiArea;
}TVIDEOROIPARAM, *PVIDEOROIPARAM;

// 编码裁剪
typedef struct tagVideoEncCut
{
	bool	bEnable;	// 启用
	u16		wVgaWidth;	// 设置区域宽度，默认为10000
	u16		wVgaHeight;	// 设置区域高度，默认为10000
	u16		wMinWidth;	// 最小区域宽度，不可修改--5500
	u16		wMinHeight;	// 最小区域高度，不可修改--5500
}TVIDEOENCCUT, *PTVIDEOENCCUT;

typedef struct tagEncCutArea
{
	u16		wX;			// 左顶点x值
	u16		wY;			// y值
	u16		wWidth;		// 宽度
	u16		wHeight;	// 高度
}TENCCUTAREA, *PTENCCUTAREA;

typedef struct tagVideoEncCutInfo
{
	TVIDEOENCCUT	tVideoEncCut;
	TENCCUTAREA		tEncCutArea;
}TVIDEOENCCUTINFO, *PTVIDEOENCCUTINFO;

typedef struct tagVideoEncCutParam
{
	u8	byVideoSource;
	TVIDEOENCCUT	tVideoEncCut;
	TENCCUTAREA		tEncCutArea;
}TVIDEOENCCUTPARAM, *PTVIDEOENCCUTPARAM;

// OSD
typedef struct tagOsdStateInfo
{
	char szState[17];		//”prepare,uploading,idle”
	u8	byProcess;			// 进度0-100
}TOSDSTATEINFO,*PTOSDSTATEINFO;

typedef struct tagOsdPrepareParam
{
	u8	 byVideoSource;
	char szPicId[17];		// 图片ID opt=”osd,label,time,alarm, move,shieldalarm”
	bool bIsAlarm;			// 是否告警输入
	u8	 byPicSn;			// 图片序号 osd, alarmin专用
	u16	 wFileLen;			// 图片文件长度
	u16	 wSendLenMax;		// 单次发送长度数据
}TOSDPREPAREPARAM,*PTOSDPREPAREPARAM;

typedef struct tagOsdCapInfo
{
	char szTimeFormat[33];		// 支持的时间显示格式”MM-DD-YYYY,YYYY-MM-DD,DD-MM-YYYY”
	char szFontName[33];		// 支持的字体 "blcak song"
	char szFontColor[33];		// 支持的字体颜色”black, white,red,blue”
	char szFontBkColor[17];		// 支持的背景颜色 "black, white,red,blue"
	char szOsdType[65];			// 支持的字幕类型”osd,lable,time,alarm,ptz,3g”
}TOSDCAPINFO, *PTOSDCAPINFO;

typedef struct tagOsdUploadFileParam
{
	u8	byVideoSource;
	u16	wStartPos;	// 起始数据位置
	u16 wDataLen;	// 数据长度
	char* pData;	// 数据
}TOSDUPLOADFILEPARAM, *PTOSDUPLOADFILEPARAM;

typedef struct tagOsdList
{
	char	szOsdType[17];		// time,osd,alarm,label,ptz,3g
	bool	bVisiable;
	u8		bySn;				// osd类型字幕专用1-6
	u16		wOsdX;				// 子OSD的x值和y值
	u16		wOsdY;
	u16		wOsdWidth;			// 子OSD的高度和宽度
	u16		wOsdHeight;
	u8      bynindex;
	char	szContext[65];		// 字幕内容
}TOSDLIST, *PTOSDLIST;

typedef struct tagOsdListInfo
{
	TOSDLIST	tOsdList[16];
}TOSDLISTINFO, *PTOSDLISTINFO;

typedef struct tagSmartOsd
{
	char szType[32]; //value: (vis_suspect_fire,vis_fire,vis_suspect_smoke,vis_smoke,the_high_temp,the_fire,warn_manual)
	u32 dwX;
	u32 dwY;
	u32 dwWidth;
	u32 dwHeight;
}TOSDSMART,*PTOSDSMART;

typedef struct tagSmartOsdInfo
{
	TOSDSMART tSmartOsd[24];
}TOSDSMARTINFO,*PTOSDSMARTINFO;

typedef struct tagSmartOsdParam
{
	u8 byVideoSource;
	u8 bySmartOsdNum;  //最多24个
	TOSDSMART tOsdSmartList[24];
}TOSDSMARTPARAM,*PTOSDSMARTPARAM;

typedef struct tagOsdListParam
{
	u8		byVideoSource;
	u8		byOsdNum;			//设置OSD个数
	TOSDLIST	tOsdList[16];
}TOSDLISTPARAM, *PTOSDLISTPARAM;

typedef struct tagOsdInfo
{
	bool	bUploadOsdPic;			// 是否上传图片
	u16		wResolutionWidth;		// 分辨率宽度
	u16		wResolutionHeight;		// 分辨率高度
	char	szTimeFormat[33];		// 时间字幕格式MM-DD-YYYY,YYYY-MM-DD,DD-MM-YYYY
	bool	bTimeSingleRow;			// 是否单行
	char	szFontName[17];			// 字体 song black
	char	szFontColor[17];		// 字体颜色默认	white， 支持black, white,red,blue
	char	szFontBkColor[17];		// 背景颜色默认 black， 支持black, white,red,blue
	bool	bFontTransparent;		// 字体透明
	bool	bFontEdge;				// 字体粗体
	u8		byBackGuageTop;			// 上边距（n个字符的高度，用于边缘对齐，0～16）
	u8		byBackGuageBotton;		// 下边距
	u8		byBackGuageLeft;		// 左边距
	u8		byBackGuageRight;		// 右边距
}TOSDINFO, *PTOSDINFO;

typedef struct tagOsdParam
{
	u8		byVideoSource;
	char	szTimeFormat[33];		// 时间字幕格式MM-DD-YYYY,YYYY-MM-DD,DD-MM-YYYY
	bool	bTimeSingleRow;			// 是否单行
	char	szFontName[17];			// 字体 song black
	char	szFontColor[17];		// 字体颜色默认	white， 支持black, white,red,blue
	char	szFontBkColor[17];		// 背景颜色默认 black， 支持black, white,red,blue
	bool	bFontTransparent;		// 字体透明
	bool	bFontEdge;				// 字体粗体
	u8		byBackGuageTop;			// 上边距（n个字符的高度，用于边缘对齐，0～16）
	u8		byBackGuageBotton;		// 下边距
	u8		byBackGuageLeft;		// 左边距
	u8		byBackGuageRight;		// 右边距
}TOSDPARAM, *PTOSDPARAM;

// 图像
typedef struct tagImgDefParam
{
	u8	byVideoSource;		// 视频ID
	char szcolorrecov[17];	// 1.恢复全部:all, 2.恢复颜色值:color
}TIMGDEFPARAM, *PTIMGDEFPARAM;

// 图像--颜色调节
typedef struct tagColorInfo
{
	u8	byBrightnessLevel;	//亮度0-100
	u8	bycContrastLevel;	//对比度0-100
	u8  bySaturationLevel;	//饱和度0-100
	u8	byHueLevel;			//锐度0-100
}TCOLORINFO, *PTCOLORINFO;

typedef struct tagColorParam
{
	u8	byVideoSource;
	TCOLORINFO tColorInfo;
}TCOLORPARAM, *PTCOLORPARAM;

// 图像-聚焦参数
typedef struct tagFocusInfo
{
	char szFocusStyle[17];	//single,continu,auto,manual
	char szFocusMin[17];	//最小聚焦距离 1m,2m
	char szFocusMax[17];	//最大聚焦距离 1m,2m
}TFOCUSINFO, *PTFOCUSINFO;

typedef struct tagFocusParam
{
	u8	byVideoSource;					//
	TFOCUSINFO tFocusInfo;
}TFOCUSPARAM, *PTFOCUSPARAM;

// 图像-白平衡参数
typedef struct tagWhiteBlanceInfo
{
	char szWhiteBlaneceStyle[17]; // 白平衡类型
	u8	byBlanceRed;			  // 红增益值0-100
	u8  byBlanceBlue;			  // 蓝增益值0-100
}TWHITEBLANCEINFO, *PTWHITEBLANCEINFO;

typedef struct tagWhiteBlanceParam
{
	u8	byVideoSource;
	TWHITEBLANCEINFO tWhiteBlanceInfo;
}TWHITEBLANCEPARAM, *PTWHITEBLANCEPARAM;

// 图像-日夜切换参数
typedef struct tagIrcutFixTime
{
	char szstarttime[17];
	char szendtime[17];
	bool bEnable;		// 启用true, false
}TIRCUTFIXTIME, *PTIRCUTFIXTIME;

typedef struct tagIrcutWeekTime
{
	TIRCUTFIXTIME	tIrcutWeekdayTime[7];	// 周日到周六
}TIRCUTWEEKTIME, PTIRCUTWEEKTIME;

typedef struct tagIrcutFilterInfo
{
	char szircutfiltertype[17];		// 转换类型 day,night,auto_gain,auto_photo,time
	char sztrigger[17];				// 触发状态day night
	char szdaytonightlevel[17];		// 等级 低 中 高
	char szdaytonighttime[17];		// 时间 (5-20)
	u8	 byPhotoLevel;				// 0-100
	//只有在szircutfiltertype为time(定时触发)时才需要设置以下的时间结构体
	TIRCUTFIXTIME	tIrcutTimeAll;	// szEnable参数tIrcutTimeAll和tIrcutWeekdayTime互斥
	TIRCUTWEEKTIME	tIrcutTimeWeekDay;	// 周日到周六
}TRCUTFILTERINFO, *PTIRCUTFILTERINFO;

typedef struct tagIrcutFilterParam
{
	u8 byVideoSource;
	TRCUTFILTERINFO tImageIrcutFilterInfo;
}TTRCUTFILTERPARAM, *PTIRCUTFILTERPARAM;

// 图像--光圈
typedef struct tagIrisInfo
{
	char szIrisMode[17];		//光圈模式“auto, manual,dc_irisauto, dc_irismanual, p_irisauto, p_irismanual”
	u8	 byIrisSize;			//光圈大小
	u8	 byIrisLevel;			//光圈等级
}TIRISINFO, *PTIRISINFO;

typedef struct tagIrisParam
{
	u8	byVideoSource;
	TIRISINFO	tIrisInfo;
}TIRISPARAM, *PTIRISPARAM;

// 图像--快门
typedef struct tagShutterInfo
{
	char szshutermode[17]; // 快门模式
	char szshutermin[17];  // 快门下限
	char szshuterlevel[17];// 灵敏度-手动
}TSHUTTERINFO, *PTSHUTTERINFO;

typedef struct tagShutterParam
{
	u8	byVideoSource;
	TSHUTTERINFO	tShutterInfo;
}TSHUTTERPARAM, *PTSHUTTERPARAM;

// 图像-- 增益
typedef struct tagGainInfo
{
	char szgainmode[17];	// 增益模式
	u8	 byGainMax;			// 增益上限 0-100
	u8	 byGainLevel;		// 增益等级 0-100
}TGAININFO, *PTGAININFO;

typedef struct tagGainParam
{
	u8 byVideoSource;
	TGAININFO tGainInfo;
}TGAINPARAM, PTGAINPARAM;

// 图像-- 红外
typedef struct tagInfraredInfo
{
	char szControlMode[17];	// 启用模式--关闭，自动开启，远灯开启，中灯开启，近灯开启，超近灯开启
	u8	byInfraredLevel;	// 红外强度
}TINFRAREDINFO, *PTINFRAREDINFO;

typedef struct tagInfraredParam
{
	u8	byVideoSource;
	TINFRAREDINFO	tInfraredInfo;
}TINFRAREDPARAM, *PTINFRAREDPARAM;

//图像-- 激光
typedef struct tagLaserInfo
{
	char szControlMode[17];	 // open, close
	u8	byLaserLevel;		 // 激光强度
	char szSpotMode[17];	 // 聚光强度 默认，光斑小模式，光斑大模式，自定义模式1，自定义模式2
}TLASERINFO, *PTLASERINFO;

typedef struct tagLaserParam
{
	u8	byVideoSource;
	TLASERINFO	tLaserInfo;
}TLASERPARAM, *PTLASERPARAM;

// 图像 --走廊模式
typedef struct tagCorridoModeInfo
{
	char	szCorridoMode[17];	// 走廊模式close, left,right
}TCORRIDOMODEINFO, *PTCORRIDOMODEINFO;

typedef struct tagCorridoModeParam
{
	u8	byVideoSource;
	TCORRIDOMODEINFO	tCorridoModeInfo;
}TCORRIDOMODEPARAM, *PTCORRIDOMODEPARAM;

// 图像 --镜像模式
typedef struct tagImageModeInfo
{
	char szImageMode[17];	//镜像模式close, left,right,topbottom
}TIMAGEMODEINFO, *PTIMAGEMODEINFO;

typedef struct tagImageModeParam
{
	u8	byVideoSource;
	TIMAGEMODEINFO	tImageModeInfo;
}TIMAGEMODEPARAM, *PTIMAGEMODEPARAM;

// 图像 --2D降噪
typedef struct tag2DNoise
{
	char szNoiseReduce2DMode[17]; //2D降噪模式auto,open,close
	u8	 byNoiseReduce2DLevel;
}T2DDENOISE, *PT2DDENOISE;

typedef struct tag2DNoiseParam
{
	u8	byVideoSource;
	T2DDENOISE	tNoiseReduceInfo;
}T2DDENOISEPARAM, *PT2DDENOISEPARAM;

// 图像 --3D降噪
typedef struct tagNoiseReduce3DInfo
{
	char	szNoiseReduce3DMode[17];	// 3D降噪模式auto,open,close
	u8		byNoiseReduce3DLevel;
}T3DDENOISE, *PT3DDENOISE;

typedef struct	tagNoiseReduce3DParam
{
	u8	byVideoSource;
	T3DDENOISE	tNoiseReduceInfo;
}T3DDENOISEPARAM, *PT3DDENOISEPARAM;

// 图像 -- 本地回显
typedef struct tagLocalEchoInfo
{
	char	szLocalEchoMode[17];	// close, pal,ntsc,line,interleave
}TLOCALECHOINFO, *PTLOCALECHOINFO;

typedef struct tagLocalEchoParam
{
	u8	byVideoSource;
	TLOCALECHOINFO	tLocalEchoInfo;
}TLOCALECHOPARAM, *PTLOCALECHOPARAM;

// 图像 -- 防闪烁
typedef struct tagPowerLineFrequencyInfo
{
	char szPowerLineFrequencyMode[17];	// 50hz,60hz,auto
}TPOWERLINEFREQUENCYINFO, PTPOWERLINEFREQUENCYINFO;

typedef struct tagPowerLineFrequencyParam
{
	u8	byVideoSource;
	TPOWERLINEFREQUENCYINFO	tPowerLineFrequencyInfo;
}TPOWERLINEFREQUENCYPARAM, *PTPOWERLINEFREQUENCYPARAM;

// 图像 -- 动态组合
//动态调节区域
typedef struct tagDynamicArea
{
	char szType[17];	// 区域类型：矩形rect
	u8	byPointNum;		// 区域的顶点
	u16	x[16];			// x坐标	顶点坐标，顺时针写入
	u16	y[16];			// y坐标
}TDYNAMICAREA, *PTDYNAMICAREA;

typedef struct tagDynamicModeInfo
{
	char szMode[17];		// 动态调节模式，关闭(close)/宽动态(wdr)/强光抑制(hlc)/背光补偿自动(autoblc)/
							// 背光补偿手动(regionblc)/透雾(fogthrough)/Gamma(gamma)/smartIR(smartir)
	u8	nLevel;				// 灵敏度
	char szPosition[17];	// 调节方位 自定义custom,上下左右中心 top,left,right,bottom,center
	u16	wMinWidth;			// 最小宽度
	u16 wMinHeight;			// 最小高度
	u16	wVGAWidth;			// 区域的宽度
	u16	wVGAHeight;			// 区域的高度
	TDYNAMICAREA	tDynamicArea;
}TDYNAMICMODEINFO, *PTDYNAMICMODEINFO;

typedef struct tagDynamicModeParam
{
	u8	byVideoSource;
	TDYNAMICMODEINFO	tDynamicModeInfo;
}TDYNAMICMODEPARAM, *PTDYNAMICMODEPARAM;

// 红外补光灯
typedef struct tagFillLight
{
	char szMode[7];   // 补光灯模式: 关闭(close),自动(auto),手动(manual)
	u16  wBrightness; // 亮度(0~100)
	u16  wThreshold;  // 阈值(0~48)
}TDFILLLIGHT, *PTDFILLLIGHT;

// 动态插件状态
typedef struct tagDynPlugin
{
	bool bEnable;   // true-开启,false-关闭
}TDDYNMAICPLUGIN, *PTDDYNMAICPLUGIN;

// 设备信息
typedef struct tagDeviceInfo
{
	char szDevName[MAX_DEV_NAME+1];						// 设备名称
	char szDevType[MAX_DEV_NAME+1];						// 设备类型
	char szSerial[MAX_DEV_NAME+1];						// 序列号
	char szVerHard[MAX_DEV_NAME+1];						// 硬件版本号
	char szVerSoft[MAX_DEV_NAME+1];						// 软件版本号
	char szSeriesName[MAX_DEV_NAME+1];					// 系列号
	u8   byNumVideo;									// 视频源个数
	u8   byCardNum;										// 卡编号(MSP1000编码卡在母版上的编号,参数只读)
	u8   bySlotNum;                                     // 槽道号(MSP1000编码卡母版在机箱上的槽道号,参数只读)
	u8   byCaseNum;                                     // 机箱号(MSP1000编码卡母版机箱编号,参数只读)
}TDEVICEINFO,  *PTDEVICEINFO;

//eeprom信息
typedef struct tagEepromInfo
{
	int	nMsgType;     // 消息类型
	u8	byChanID;     // 默认0即可
	char szDevName[128];  //设备名称
	char szPid[128];	// pid
	char szHid[128];     //hid
	u32 dwHwVer;        // 硬件版本号
	u32 deHwSubVer;     // 硬件子版本号
} TEEPROMINFO, *PTEEPROMINFO;   

typedef struct tagUserDef
{
	char szTitle[64+1];
	char szContent[128+1];
} TUSERDEF,*PUSERDEF;

typedef struct tagCheckPoint
{
	char szName[64+1];
	char szDevnum[64+1];
	TUSERDEF tUserdeflist[10];
	u8 byUserdefNum;     //最多10个
} TCHECKPOINT,*PCHECKPOINT;

//PointOsd信息
typedef struct tagCheckPointInfo
{
	s32 dwCurindex;
	TCHECKPOINT tCheckpointlist[10];
	bool bIsshowname;
	bool bIsshowplateno;
	bool bIsshowdevnum;
	bool bIsshowtime;
	bool bIsshowuserdef;
	u8	 byCheckPointNum;	//最多10个
} TCHECKPOINTINFO,*PCHECKPOINTINFO;

//PointOsd信息
typedef struct tagCheckPointParam
{
	u8	byVideoSource;
	s32 dwCurindex;
	TCHECKPOINT tCheckpointlist[10];
	bool bIsshowname;
	bool bIsshowplateno;
	bool bIsshowdevnum;
	bool bIsshowtime;
	bool bIsshowuserdef;
	u8	 byCheckPointNum;	//最多10个
} TCHECKPOINTPARAM,*PCHECKPOINTPARAM;


typedef struct tagManualSnap
{
	s32 dwIndex;
	bool bEnable;
	char szSnaptype[64+1];
	u32 dwSnapnum;
	s32 dwSnapinterval;
	bool bVidrecenable;
	s32 dwVidrectime;
	char szSavePathType[9];//opt:"local ,frontend ,pubsec"
}TMANUALSNAP,*PMANUALSNAP;

//event manual snap
typedef struct tagManualSnapInfo
{
	TMANUALSNAP snapInfo[6];
	u8   byManualSnapNum;  
}TMANUALSNAPINFO,*PMANUALSNAPINFO;


//devseq信息
typedef struct tagDevSeqInfo
{
	int	typeID;     // 类型 ID
	int	byChanID;     // 默认0即可
	char devseq[128];	// dev SEQ
} TDEVSEQINFO, *PDEVSEQINFO; 

//工具信息
typedef struct tagTToolInfo
{
	char toolType[16];  /*zoomrate,irate,collimation,pistart,piclear,pinoclear,pizoomin,pizoomout,pifocusfar,pifocusnear,
	                      startcptest,endcptest,iristest,whiteptest,blackptest*/
	s32 toolValue;     // 如果type类型为zoomrate或者irate的话，需要添加value值
} TTOOLINFO, *PTTOOLINFO;

// 时间获取
typedef struct tagSysTimeInfo
{
	char szTimeZone[8];									//时区, 对应的时间偏移值,单位秒
	u32  dwYear;
	u32  dwMonth;
	u32  dwDay;
	u32  dwHour;
	u32  dwMinute;
	u32  dwSecond;
}TSYSTIMEINFO,  *PSYSTIMEINFO;

typedef struct tagTimeAutoInfo
{
	bool  bAuto;
	u8   byMode;										// ntp, inputprotocol,要定义宏
	char szNtpUrl[MAX_PATH];
	u16  wNtpPort;
	u16  wNtpTimeSpeed;									// 秒
}TTIMEAUTOINFO,  *PTIMEAUTOINFO;

typedef struct tagAutoAdaptInfo
{
	bool bCheck;                                        //是否支持
	char szMode[9+1];									//ntp,vsip,gb28181-1,gb28181-2
}TAUTOADAPTINFO,*PAUTOADAPTINFO;

union unMode
{
	u8 byMode;											//旧接口
	char szMode[9+1];									// any,ntp,vsip,gb28181-1,gb28181-2
};

typedef struct tagTimeAutoInfoEx
{
	bool  bAuto;                                        //自动校时间
	//char  szMode[9+1];									// any,ntp,vsip,gb28181-1,gb28181-2
	unMode unModeInfo;									// 兼容老的接口和新的接口
	TAUTOADAPTINFO autoAdaptInfo[4];                    //如果szMode为any，开启此功能，否则禁用,支持ntp,vsip,gb28181-1,gb28181-2
	bool bNtpEnable;                                    //是否开启NTP协议校时
	u16 wAdaptlocktime;                                 //校时保护时间,如果szMode为any，开启此功能，否则禁用
	char szNtpUrl[MAX_PATH];							//服务器地址
	u16  wNtpPort;                                      //NTP端口
	u16  wNtpTimeSpeed;									// 校对时间间隔 单位：秒
}TTIMEAUTOINFOEX,  *PTIMEAUTOINFOEX;

// 夏令时
typedef struct tagDSTInfo
{
	bool	bEnable;									// 启用
	u8  byStartMonth;									// 起始月
	u8  byStartWeek;									// 起始周，基于月（1～5）
	u8  byStartDay;										// 起始天，基于周（1～7）
	u8  byStartHour;									// 起始小时（1～24）
	u8  byEndMonth;										// 结束月
	u8  byEndWeek;										// 结束周，基于月（1～5）
	u8  byEndDay;										// 结束天，基于周（1～7）
	u8  byEndHour;										// 结束小时（1～24
	u8	byOffMinute;									// 偏移时间,分(0~120)
} TDSTINFO,  * PTDSTINFO;

#define MAX_PROTOCOL_NAME			 32					// 最大协议名称长度(32位)
typedef struct tagRSInfo
{
	char  	szType[33];									// 串口类型, rs232,rs485
	u32  	dwBaudrate;									// 波特率，2400 4800, 9600, 19200, 38400, 57600 , 76800 ,  115200
	u32  	dwDataBit;									// 数据位，5,6,7,8
	u32  	dwStopBit;;									// 停止位，1,2
	u8  	byAddressNum;								// 地址码(1~255)
	char  	szParityBit[17];							// 校验位none , even , odd
	char  	szStreamControl[17];						// 流控，none, hardctrl, softctrl
	char	szControlProtocol[MAX_PROTOCOL_NAME + 1];	// 控制协议 名称
} TRSINFO,  * PTRSINFO;

typedef struct tagLedInfo
{
	bool  	bOpen;										// 是否启用
	char* 	szTime;										// 闪烁的时间，5s,10s,30s,1m,30m,60m,24h
} TLEDINFO,  * PTLEDINFO;

typedef struct tagUserActiveInfo
{
	char	szUserNmae[MAX_USER_NAME];              // 用户名
	char	szUserPassword[MAX_USER_PASSWORD];		// 用户密码
	char    szMailAddr[128];						// 激活邮箱地址
} TUSERACTIVEINFO,  *PTUSERACTIVEINFO;

typedef struct tagUploadInfo
{
	u32  nLenFile;										// 是否启用
	u32  nMaxLen_Send;									// 单包发送的最大长度
	char    szFilePath[MAX_PATH];						// 文件路径
} TUPLOADINFO,  *PTUPLOADINFO;

typedef struct tagDownloadInfo
{
	u32  nLenFile;										// 是否启用
	u32  nMaxLen_Send;									// 单包发送的最大长度
	char    szFilePath[MAX_PATH];						// 保存文件路径
} TDOWNLOADINFO,  *PTDOWNLOADINFO;

typedef struct tagDdnsInfo
{
	bool  bDdnsEnable;									//启用DDNS
	char  szDdnsType[8];								//DDNS类型
	char  szDevurl[MAX_PATH] ;							//设备域名 
	char  szUserName[MAX_USER_NAME + 1];				//用户名
	char  szPassWord[MAX_USER_PASSWORD + 1];			//密码
}TDDNSINFO, *PTDDNSINFO;

typedef struct tagUploadState
{
	char  szState[17];									// 升级状态 "prepare,uploading,idel"integer 
	u32	  nProgress;									// 升级进度 0～100
} TUPLOADSTATE, *PTUPLOADSTATE;

// 权限
typedef struct tagPowerInfo
{
	char	szType[17];		// 用户类型: administrator, operator, viewer
	bool	bView;			// 浏览
	bool	bRecanPic;		// 回放/图片抓拍
	bool	bPtz;			// 云台控制
	bool	bParamSet;		// 参数配置
	bool	bMemory;		// 存储
	bool	bReboot;		// 重启、发现、自动维护
	bool	bSystem;		// 系统配置-升级，配置导入导出
}TPOWERINFO,PTPOWERINFO;

// 用户权限信息
typedef struct tagUserInfo
{
	char	szUserName[MAX_USER_NAME + 1];				// 用户姓名
	char	szOldUserName[MAX_USER_NAME + 1];			// 旧用户姓名(获取用户信息时为空)
	char	szUserPassword[MAX_USER_PASSWORD+ 1];		// 用户密码
	char	szOldUserPassword[MAX_USER_PASSWORD+ 1];	// 旧用户密码(获取用户信息时为空)
	char	szUserType[17];								// 用户类型"administrator, operator, viewer"
	bool	bView;										// 浏览
	bool	bRecanPic;									// 回放/图片抓拍
	bool	bPtz;										// 云台控制
	bool	bParamSet;									// 参数配置
	bool	bMemory;									// 存储
	bool	bReboot;									// 重启、发现、自动维护
	bool	bSystem;									// 系统配置-升级，配置导入导出
}TUSERINFO,PTUSERINFO;

// 删除用户
typedef struct tagUserDelInfo
{
    char    szUserName[MAX_USER_NAME  + 1];				// 用户姓名
	char	szUserPassword[MAX_USER_PASSWORD+ 1];		// 用户密码
	char	szUserType[17];								// 用户类型"administrator, operator, viewer"
}TUSERDELINFO,PTUSERDELINFO;

// 修改用户
typedef struct tagUserUpdateInfo
{
    char    szUserName_Old[MAX_USER_NAME  + 1];			// 旧用户姓名
	char	szUserPassword_Old[MAX_USER_PASSWORD+ 1];	// 旧用户密码
    char    szUserName_New[MAX_USER_NAME  + 1];			// 新用户姓名
	char	szUserPassword_New[MAX_USER_PASSWORD+ 1];	// 新用户密码
	char	szUserType[17];								// 用户类型"administrator, operator, viewer"
	bool	bView;										// 浏览
	bool	bRecanPic;									// 回放/图片抓拍
	bool	bPtz;										// 云台控制
	bool	bParamSet;									// 参数配置
	bool	bMemory;									// 存储
	bool	bReboot;									// 重启、发现、自动维护
	bool	bSystem;									// 系统配置-升级，配置导入导出
}TUSERUPDATEINFO,PTUSERUPDATEINFO;


typedef struct tagAdvancedInfo
{
	char  	szKVispTimeStamp[33];	// KEDAVISIP码流时间戳标准：标准(normal)、旧标(old)
	char  	szKVispKeepLive[33];	// KEDAVISIP音视频保活：自动(auto)、开启(open)、关闭(close)
	char  	szOnvifKeepLive[33];	// Onvif音视频保活：自动(auto)、开启(open)、关闭(close)
	char  	szGB28181KeepLive[33];	// GB28181音视频保活：自动(auto)、开启(open)、关闭(close)
	char  	szCGIKeepLive[33];		// CGI音视频保活：自动(auto)、开启(open)、关闭(close)
	char  	szKVispOSDPicSource[33];	// KEDAVISIP OSD图片来源：客户端(client)，设备(dev)
	char  	szKVispOSDFontName[33];	// KEDAVISIP OSD字体：宋体(song)，黑体(black)
	bool	bTCControl;			// TC流控
	char  	szMTCFMode[33];		// 工作模式：超宽动态模式(hdr)， 60帧模式(fps60)，四流模式(stream4)，4k 4K模式(4k)，Div4 4K四分屏模式(div4),普通（normal），HDSDI-普通模式（sdsdi）
	char  	szAccMode[33];		// 工作模式：自动低延时模式(autolow)，普通模式(normal)，强制低延时模式(low)
} TADVANCEDINFO,  * PTADVANCEDINFO;

//DDNS状态参数
typedef struct tagDdnsState
{
	char szDdnsState[10];  //状态
}TDDNSSTATE, *PTDDNSSTATE;

//TCP\IP信息
typedef struct tagTCPIPv6Info
{
	u8    byIpMode;            //IP模式  0-自动auto 1-手动manual 3-路线route
	bool   bDnsAutoEnable;     //启用自动DNS分配(TRUE,FALSE)
	u32   dwSubNetMask;        //子网掩码
	u32   dwDefGateway;        //缺省网关
	char   szIpAddr[MAX_PATH];       //IP地址
	char   szMultCastAddr[MAX_PATH]; //多播地址(最大64)
	char   szDNSFirst[MAX_PATH];     //首选DNS服务器
	char   szDNSAlter[MAX_PATH];     //备选DNS服务器(最大64字节)
}TIPV6INFO, *PTIPV6INFO;

typedef struct tagTCPIPv4Info
{
	u8   byAddrType;           //地址类型  0-静态，1-动态
	bool   bDnsAutoEnable;     //启用自动DNS分配(TRUE,FALSE)
	u32   dwSubNetMask;        //子网掩码
	u32   dwDefGateway;        //缺省网关
	u32   dwIP;       		   //IP地址
	char   szMultCastAddr[MAX_PATH]; //多播地址
	char   szDNSFirst[MAX_PATH];     //首选DNS服务器(最大64字节)
	char   szDNSAlter[MAX_PATH];     //备选DNS服务器(最大64字节)
}TIPV4INFO, *PTIPV4INFO;

typedef struct tagIPInfo
{
	char        szMac[18];      //Mac地址  
	u32         dwMtu;	        //Mtu(500~4000)
	TIPV4INFO  	tIpv4Info;	    //Ipv4参数结构
	TIPV6INFO   tIpv6Info;      //Ipv6参数结构
}TIPINFO, *PTIPINFO;


//端口
typedef struct tagPortInfo
{
	u16  wHttpPort;            //http端口(0~65535)
	u16  wHttpsPort;           //https端口(0~65535)
	u16  wRtspPort;			   //rstp端口（0~65535）
	u16  wMulticastPort;       //多播端口 (0~65535)
}TPORTINFO, *PTPORTINFO;

//Wlan信息
typedef struct tagWlanInfo
{
	char        szMode[5];      //模式(wlan)  
	char        szSSid[33];	    //ssid
	char        szSecurity[5];  //安全模式(wpk2)  
	char        szEncrypt[5];   //加密模式(aes) 
	char        szPassword[17];	//密码
	u16         wChannel;       //信道(1~11)  
	char        szApband[5];    //(2.4g或者5g）
}TWLANINFO, *PTWLANINFO;

//upnp信息
typedef struct tagUPnPInfo
{
	bool 	bEnable;               		// 是否支持
	char  	szAlias[MAX_USER_NAME];  	// 别名
	char  	szPortMapMode[17];          // 端口映射方式 0-auto 1-manual
}TUPNPINFO, *PTQUPNPINFO;

//upnp端口映射信息
typedef struct tagUPnPPortMap
{
	bool 	bCheck;    		    // 是否选择
	char  	szProtocolName[17]; // 协议名称 http/rtsp/sdk
	char    szState[17]; 		// 状态valid/invalid
	char    szMode[5];          // 传输模式 tcp/udp
	u16     wPort;			    // 外部端口 0~65535
	u32     dwIp;		 	    // ip
} TUPNPPORTMAP, *PTUPNPPORTMAP;

//pppoe
typedef struct tagPPPoEInfo
{
	bool 	bEnable;          		              // 启用PPPOE
	u32  	dwDynamicIp;            	          // 设备动态IP(只读) 
	char  	szUserName[MAX_USER_NAME + 1]; 		  // 用户名
	char  	szPassWord[MAX_USER_PASSWORD + 1];    // 密码
}TPPPOEINFO, * PTPPPOEINFO;

//K-SNMP
typedef struct tagKSnmpInfo
{
	u16  wSvrPort;      	// 网管服务器端口（只读）(0~65535)
	u32  dwSvrIP;     		// 网管服务器地址
	u32  dwCpuVpt;    	    // cpu利用率阀值(0~100)
	u32  dwMemeryVPT; 	    // 内存利用率阀值(0~100)
	u32  dwLostVPT;     	// 视频丢包率阀值(0~100)
	char szDevAdress[33]; 	// 设备位置信息(最多32)
}TKSNMPINFO, *PTKSNMPINFO;

//802.1X
typedef struct tag8021xInfo
{
	bool 	bEnable;    		               // 启用
	char  szProtocoltype[17];                  // 协议类型(只读),eap-tls,eap-ttls,eap-peap,eap-leap,eap-fast,eap-md5
	char  szEapolVersion[17];  	               // eapol版本“1, 2”
	char  szUserName[MAX_USER_NAME+1];    	   // 用户名
	char  szPassWord[MAX_USER_PASSWORD+1];       // 密码
}T8021XINFO, *PT8021XINFO;

//Qos
typedef struct tagQosInfo
{
	bool 	bEnable;    		// 是否支持
	u32  	dwVideoDscpValue; 	// 音视频dscp(0~63)
	u32     dwManagerDscpValue; // 管理dscp(0~63)
}TQOSINFO, *PTQOSINFO;

//Vsip
typedef struct tagKVispInfo
{
	bool	bEnable;			        // 启用 	
	bool	bSendNatPac;			    // 是否发送nat探测包
	bool	bSelfNetWork;		        // 是否支持自动组网
	u8	    byAddrtype;			        // 域名类型(ip,url)	
	u16	    wIpPort;			        // 端口
	u16	    wLinkCheckTimeSpeed;	    // 链路检测时间(10~1000)	
	u32	    dwIpv4;			            // ipv4地址	
	char	szUuid[33];			        // 入网设备uuid
	char	szNetPassWord[33];		    // 入网设备密码
	char	szUrl[MAX_PATH];		    // url(最大260)
	char	szIpv6[MAX_PATH];			// ipv6地址
}TKVISPINFO, *PTKVISPINFO;

//Onvif
typedef struct tagONVIFInfo
{
	bool	byOnvifEnable;		        // 启用 
	char	szUrl[MAX_PATH];		    // 服务器url
}TONVIFINFO,PTONVIFINFO;

//AuthMode
typedef struct tagAuthModeInfo
{
	char	szAuthtication [8];		// 鉴权方式 none,digest
}TAUTHMODEINFO,*PTAUTMODEINFO;

// 编码通道信息
typedef struct tagGBEncChnInfo
{
	u32	    dwIndex;		          // 节点号
	u8	    byStramChn;		          // 码流通道
	char	szId[33];		          // 通道id(最大32)
	char	szName[33];		          // 名称(最大32)
}TGBENCCHNINFO,*PTGBENCCHNINFO;
// 告警通道信息
typedef struct tagGBAlarmChnInfo
{
	u32	    dwIndex;		          // 节点号
	char	szId[33];		          // 通道id(最大32)
	char	szName[33];		          // 名称(最大32)
}TGBALARMCHNINFO,*PTGBALARMCHNINFO;
// GB28181信息
typedef struct tagGB28181Info
{
	bool	 bEnable;		              // 启用 
	u16	    wLocalPort;		          // 本地端口  (0~65535)
	u16  	wPlatPort;		              // 平台端口(0~65535)
	u32	    dwReletTime;		          // 续租时间	(1~1440)
	u32	    dwHeartBeat;		          // 心跳间隔	(1~100)
	u32	    dwIpv4;		                  // 平台ipv4地址
	char	szPlatId[33];		          // 平台ID  (1~16)
	char	szNetid[33];		          // 入网ID  (最大32)	
	char	szDevOwner[33];	              // 设备归属	(最大32)
	char	szCivilCode[33];	          // 行政区域   (最大32)
	char	szPoliceRgn[33];	          // 警区       (最大32)
	char	szSetupAddress[33];           // 安装地址   (最大32)
	char	szExpand[MAX_EXPAND_PATH];	  // 保留       (最大1024)
	char	szIpv6[MAX_PATH];             // 平台ipv6地址
	char	szDevName[MAX_DEV_NAME];      // 设备名称  (最大32)
	char	szUserName[MAX_USER_NAME];	  // 注册用户名
	char	szPassWord[MAX_USER_PASSWORD];// 注册密码
}TGB28181INFO, *PTGB28181INFO;

//DPSS
typedef struct tagDpssInfo
{
	bool	bEnable;			        // 启用 	
	u16	    wIpPort;			        // 端口(0~65535)
	u32	    dwIpv4;			            // ipv4地址	
	char	szUuid[35];			        // 入网设备uuid(6~32)
	char	szIpv6[MAX_PATH];			// ipv6地址
}TDPSSINFO, *PTDPSSINFO;

//4g
typedef struct tagMobilenetwork
{
	bool	bEnable;			    // 启用 
	bool	bUseDefaultApn;         // 启用默认apn
	char	szApn[33];
	char	szNum[33];
	char	szUser[MAX_USER_NAME];
	char	szPassword[MAX_USER_PASSWORD];
	char	szCarrieroperator[16];//运营商   "none,chinamobile,chinaunicom,chinatelecom "
	char	szAuthentication[16]; //验证协议 "none,pap,chap,auto"
	char	szNetworksystem[16];  //网络制式
	char	szNetworkstatus[16];  //网络状态 "link,unlink"
	char	szSigstatus[16];      //信号强度 "good,nomal,bad,none"
	char	szImei[16];           //imei码
	char	szIpaddress[16];
	char	szSubnetmask[16];
	char	szDefaultgateway[16];
	char	szDns1[16];
	char	szDns2[16];
	char	szSimCard[16];        //sim卡状态 "yes,no"
}TMOBILENETWORK, *PTMOBILENETWORK;

//视图库
typedef struct tagPubsec
{
	bool	bEnable;			         // 启用 
	char	szNetid[33];                 // 入网id
	char	szUser[MAX_USER_NAME];
	char	szPassword[MAX_USER_PASSWORD];
	char	szSetupaddress[MAX_PATH];   //安装地址
	char	szCivilcode[MAX_PATH];      //行政区域
	char	szPlatAddr[16];            //ip
	u16     wPort;                      //端口1024~65535
	u16     wHeartbeat;                 //心跳间隔 1~90
}TPUBSECPARAM, *PTPUBSECPARAM;

//磁盘状态获取
typedef struct tagStorageState
{
	char	szState[33];		// 磁盘(null),磁盘使用中(disker),速度慢(diskrslowish), 空闲(idel), 
	                            // 没有格式化(noformat),格式化中(formatting),识别中(identifying)
}TSTORAGESTATE,*PTSTORAGESTATE;

// 存储管理
// 磁盘信息
typedef struct tagDiskInfo
{
	u8	bySN;			          // 磁盘序号
	u32	dwDiskCapacity;	          // 磁盘容量(KB)
	u32	dwRisidual;		          // 剩余空间
	char	szState[17];		  // 磁盘(null),磁盘使用中(disker),速度慢(diskrslowish), 空闲(idel), 没有格式化(noformat),格式化中(formatting),识别中(identifying)
	char	szDiskType[17];	      // 本地磁盘(stlocal),网盘(net)
	char	szAttribute[17];	  // 只读(readonly),所有操作(all)
}TDISKINFO,*PTDISKINFO;
// 磁盘策略
typedef struct tagStorageManager
{
	char	szFullMode[17];	      // 磁盘满模式(stop),覆盖(over)
	int     nDiskMun;             // 磁盘槽口数
	TDISKINFO tDiskInfo[5];
}TSTORAGEMANAGER,*PTSTORAGEMANAGER;

// 格式化磁盘
typedef struct tagStorageFomat
{
	u8	byDiskSn;		// 磁盘序号
}TSTORAGEFOMAT,*PTSTORAGEFOMAT;

// 格式化进度
typedef struct tagFomatState
{
	bool	bFomating;		// 是否正在格式化中
	u16	    wProgress;		// 格式化进度
}TFOMATSTATE,*PTFOMATSTATE;

// 录像计划
typedef struct tagRecSchedule
{
	bool	bEnable;		         // 启用录像计划
	char	szPrepareTime[9];	     // 预录时间，秒；
	char	szDelayTime[9];	         // 延迟时间，秒；
	char	szRecType[7];		     // 录像类型，(主流、辅流、三流、四流)；
	char	szESPS [17];             // 打包格式：es格式(es)，ps格式(ps)
}TRECSCHEDULE,*PTRECSCHEDULE;


// 前端抓拍
typedef struct tagSnapPicConfig
{
	char	szPicType[9];		 // 图片格式, ”jpeg, jpg,bmp”
	char    szPicResolution[10];  // 图片分辨率 “1280_720，1920_1280”
	char	szPicQuality[17];	 // 图片质量”low, middle, hight”
}TSNAPPICCONFIG,*PTSNAPPICCONFIG;

typedef struct tagSnapTime
{
	bool	bEnable;		// 启用
	char   szSnapType[5];     // 定时抓拍类型 "time,num,"
	u16	   wSnapSpeed;		// 抓拍间隔
	u16	   wSnapNum;		// 抓拍数量
}TSNAPTIME,PTSNAPTIME;

typedef struct tagSnapEvent
{
	bool	bEnable;		// 启用
	u16	   wSnapSpeed;		// 抓拍间隔
	u16	   wSnapNum;		// 抓拍数量
}TSNAPEVENT,PTSNAPEVENT;

//常规联动方式
typedef struct tagLinkMode
{
	bool	bLinkUploadCenter;	// 上传中心
	bool	bLinkShowOsd;	    // 显示字幕
	bool    bLinkAudioOut;      // 声音报警
	bool	bLinkRec;		    // 录像联动 
	bool	bLinkSnap;		    // 抓拍
	bool    bLinkSmtp;
	bool    bLinkPtzEnb;
	u8      byLinkPtzPreset;
}TLINKMODE,*PTLINKMODE;
// a->1链表
typedef struct tagA1List
{
	bool bEnable; //
	u16  wIndex;  //
}TA1LIST,*PTA1LIST;
//其他联动方式
typedef struct tagOtherLinkMode
{
	TA1LIST tA1List[5];		// 报警输出链表 
	u32     dwA1Num;        // 报警输出个数
	bool	bPtz;		    //
	u8	    byPreset;		// 预置位，ptz联动有效
}TOTHERLINKMODE,*PTOTHERLINKMODE;

//事件触发区域
typedef struct tagDetectPoint
{
	u32	x[4];			// x
	u32	y[4];			// y
}TDETECTPOINT,*PTDETECTPOINT;

// 移动侦测信息
typedef struct tagDetectInfo
{
	bool bEnable;			        // 启用移动侦测
	u32	dwVgaWidth;			        // 设置区域宽度，默认10000, 
	u32	dwVgaHeight;			    // 设置区域高度，默认10000
	u8	byLevel;				    // 灵敏度
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TDETECTINFO,*PTDETECTINFO;

// 遮蔽报警信息
typedef struct tagAlarmInfo
{
	bool 	bEnable;		        // 启用遮蔽
	u32	dwVgaWidth;		            // 设置区域宽度，默认10000, 
	u32	dwVgaHeight;		        // 设置区域高度，默认10000
	u8	byLevel;		            // 灵敏度
	u8  byAreaLevel;                // 触发区域等级
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TALARMINFO,*PTALARMINFO;

// 警戒线信息
typedef struct tagWarningLineParam
{
	u16 wIndex;                     // 警戒线编号
	u32	dwLevel;		            // 灵敏度
	u32 dwRatio;					// 过滤区域占比
	u32	dwRatioLeftPtX;				// 过滤区域左上点x坐标
	u32	dwRatioLeftPtY;             // 过滤区域左上点y坐标
	u32	dwRatioRightPtX;			// 过滤区域右下点x坐标
	u32	dwRatioRightPtY;            // 过滤区域右下点y坐标
	char szDirectionType[5];		// 定时抓拍类型 "ab,ba,abba"
	bool bMediaMode;                // 是否支持场景模式
	char szMediaMode[5];			// 场景模式类型 "hor,ver"
	u32	dwLineLeftPtX;				// 警戒线左上点x坐标
	u32	dwLineLeftPtY;              // 警戒线左上点y坐标
	u32	dwLineRightPtX;				// 警戒线右下点x坐标
	u32	dwLineRightPtY;             // 警戒线右下点y坐标
	u32	dwPtAX;						// A点x坐标
	u32	dwPtAY;						// A点y坐标
	u32	dwPtBX;						// B点x坐标
	u32	dwPtBY;						// B点y坐标
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TWARNINGLINEPARAM,*PTWARNINGLINEPARAM;

typedef struct tagWarningLineInfo
{
	bool 	bEnable;		        // 启用警戒线
	u32	dwVgaWidth;		            // 设置区域宽度，默认10000, 
	u32	dwVgaHeight;		        // 设置区域高度，默认10000
	u32 dwLevelMax;					// 灵敏度最大值
    u32 dwLevelMin;					// 灵敏度最小值
	u32 dwRatioMax;					// 比例区域最大占比
	u32 dwRatioMin;					// 比例区域最小占比
	u32 dwSelected;                 // 当前选择警戒线(1,2,3,4)
	TWARNINGLINEPARAM tWarningLine[4];// 警戒线参数
}TWARNINGLINEINFO,*PTWARNINGLINEINFO;

// 智能触发区域点坐标链表
typedef struct tagAreaPointList
{
	u32	dwPtX;		        //  点x坐标
	u32	dwPtY;		        //  点y坐标
}TAREAPOINTLIST,*PTAREAPOINTLIST;
// 区域检测信息(入侵,进入,离开)
typedef struct tagAreaDetectParam
{
	u16 wIndex;                     // 区域编号
	u32	dwLevel;		            // 灵敏度
	u32 dwRatio;					// 过滤区域占比
	u32 dwThreshold;                // 时间阈值
	u32	dwRatioLeftPtX;				// 过滤区域左上点x坐标
	u32	dwRatioLeftPtY;             // 过滤区域左上点y坐标
	u32	dwRatioRightPtX;			// 过滤区域右下点x坐标
	u32	dwRatioRightPtY;            // 过滤区域右下点y坐标
	bool bMediaMode;                // 是否支持场景模式
	char szMediaMode[5];			// 场景模式类型 "hor,ver"
	u32 dwPiontNum;                 // 围成区域的点数目(最多6个点,最少3个点)
	TAREAPOINTLIST tPtList[6];      // 围成区域的点坐标链表
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TAREADETECTPARAM,*PTAREADETECTPARAM;

typedef struct tagAreaDetectInfo
{
	bool 	bEnable;		        // 启用区域检测
	u32	dwVgaWidth;		            // 设置区域宽度，默认10000, 
	u32	dwVgaHeight;		        // 设置区域高度，默认10000
	u32 dwLevelMax;					// 灵敏度最大值
	u32 dwLevelMin;					// 灵敏度最小值
	u32 dwRatioMax;					// 比例区域最大占比
	u32 dwRatioMin;					// 比例区域最小占比
	u32 dwThresholdMax;				// 时间阈值最大值(为0时代表不支持这个能力)
	u32 dwThresholdMin;				// 时间阈值最小值
	u32 dwSelected;                 // 当前选择入侵区域(1,2,3,4)
	TAREADETECTPARAM tAreaDetect[4];// 入侵区域参数
}TAREADETECTINFO,*PTAREADETECTINFO;
// 物品检测(遗留,拿取)
typedef struct tagObjectDetectParam
{
	u16 wIndex;                     // 区域编号
	u32	dwLevel;		            // 灵敏度
	u32 dwRatio;					// 过滤区域占比
	u32 dwThreshold;                // 时间阈值
	u32	dwRatioLeftPtX;				// 过滤区域左上点x坐标
	u32	dwRatioLeftPtY;             // 过滤区域左上点y坐标
	u32	dwRatioRightPtX;			// 过滤区域右下点x坐标
	u32	dwRatioRightPtY;            // 过滤区域右下点y坐标
	u32 dwPiontNum;                 // 围成区域的点数目(最多6个点,最少3个点)
	TAREAPOINTLIST tPtList[6];      // 围成区域的点坐标链表
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TOBJECTDETECTPARAM,*PTOBJECTDETECTPARAM;

typedef struct tagObjectDetectInfo
{
	bool 	bEnable;		        // 启用物品检测
	u32	dwVgaWidth;		            // 设置区域宽度，默认10000, 
	u32	dwVgaHeight;		        // 设置区域高度，默认10000
	u32 dwLevelMax;					// 灵敏度最大值
	u32 dwLevelMin;					// 灵敏度最小值
	u32 dwRatioMax;					// 比例区域最大占比
	u32 dwRatioMin;					// 比例区域最小占比
	u32 dwThresholdMax;				// 时间阈值最大值(为0时代表不支持这个能力)
	u32 dwThresholdMin;				// 时间阈值最小值
	u32 dwSelected;                 // 当前选择检测区域(1,2,3,4)
	TOBJECTDETECTPARAM tObjectDetect[4];// 物品检测区域参数
}TOBJECTDETECTINFO,*PTOBJECTDETECTINFO;
// 人员检测(聚集)
typedef struct tagPersontDetectParam
{
	u16 wIndex;                     // 区域编号
	u32 dwRatio;					// 过滤区域占比
	u32 dwPiontNum;                 // 围成区域的点数目(最多6个点,最少3个点)
	TAREAPOINTLIST tPtList[6];      // 围成区域的点坐标链表
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TPERSONDETECTPARAM,*PTPERSONDETECTPARAM;

typedef struct tagPersonDetectInfo
{
	bool 	bEnable;		        // 启用人员检测
	u32	dwVgaWidth;		            // 设置区域宽度，默认10000, 
	u32	dwVgaHeight;		        // 设置区域高度，默认10000
	u32 dwRatioMax;					// 比例区域最大占比
	u32 dwRatioMin;					// 比例区域最小占比
	u32 dwSelected;                 // 当前选择检测区域(1,2,3,4)
	TPERSONDETECTPARAM tPersonDetect[4];// 人员检测区域参数
}TPERSONDETECTINFO,*PTPERSONDETECTINFO;
// 虚焦检测
typedef struct tagVirtualFocusInfo
{
	bool 	bEnable;		        // 启用虚焦检测
	u32 dwLevelMax;					// 灵敏度最大值
	u32 dwLevelMin;					// 灵敏度最小值
	u32	dwLevel;		            // 灵敏度
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TVIRTUALFOCUSINFO,*PTVIRTUALFOCUSINFO;
// 场景变更
typedef struct tagSightChangeInfo
{
	bool 	bEnable;		        // 启用场景变更
	u32 dwLevelMax;					// 灵敏度最大值
	u32 dwLevelMin;					// 灵敏度最小值
	u32	dwLevel;		            // 灵敏度
	char szChkMode[16];             // 检测模式(software)
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TSIGHTCHANGEINFO,*PTSIGHTCHANGEINFO;
// 声音异常
typedef struct tagAudioExceptInfo
{
	bool 	bEnable;		        // 启用声音异常
	u32 dwThresholdMax;				// 声音强度阈值最大值
	u32 dwThresholdMin;				// 声音强度阈值最小值
	u32	dwThreshold;		        // 声音强度阈值
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TAUDIOEXCEPTINFO,*PTAUDIOEXCEPTINFO;
// 人脸检测
typedef struct tagFaceDetectInfo
{
	bool 	bEnable;		        // 启用人脸检测
}TFACEDETECTINFO,*PTFACEDETECTINFO;
// 报警输入信息
typedef struct tagAlarmInInfo
{
	bool 	bEnable;		        // 启用报警输入
	u32 	dwAlarmNum;		        // 报警输入编号
	char	szAlarmName[33];	    // 报警名称
	char	szAlarmType[17];	    // 报警类型，常开(open)，常关(close)
	char    szCopyToAlarm[4];       // 复制到报警(all)
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TALARMININFO,*PTALARMININFO;

// 报警输出信息
typedef struct tagAlarmOutInfo
{
	char   szType[4];               // 输出类型 all,0~1
	char   szDelyTime[17];		    // 延迟时间 5s/10s/30s/1m/2m/5min/10min
}TALARMOUTINFO,*PTALARMOUTINFO;

// 异常类型配置
typedef struct  tagExceptType 
{
	char szType[32];               //  异常类型  “diskfull,diskerr,disconnect,iperr,illegalaccess”
}TEXCEPTTYPE,*PTEXCEPTTYPE;

// 异常配置信息
typedef struct tagExceptInfo
{
	bool bEnable;		            // 启用异常配置
	char szType[32];                // 异常类型  “diskfull,diskerr,disconnect,iperr,illegalaccess”
	TLINKMODE tLinkMode;	        // 常规联动方式
	TOTHERLINKMODE tOtherLinkMode;	// 其他联动方式
}TEXCEPTINFO,*PTEXCEPTINFO;

// 云台移动
typedef struct tagPtzMove
{
	char	szType[17];		// 上(moveup)、下(movedown)、左(moveleft)、右(moveright)、复位(ptz_reset)
							// 右上(move_rightup)、右下(move_rightdown)、左上(move_leftup)、左下(move_leftdown)
							// 停止移动(movestop)
							// 视野拉近(zoomtele)、视野拉远(zoomwide)、停止缩放(zoomstop)
	                        // 聚近(focusnear)、聚远(focusfar)、停止聚焦(focusstop)、自动聚焦(focusauto)
	int	    nSpeed;		    // 速度1～100
	int     nTilSpeed;      // 垂直速度(用于斜向运动)
}TPTZMOVE,*PTPTZMOVE;

//安奎拉延迟定位设置
typedef struct tagPtzAquillaPtMove
{
	u32  dwHorizontal;          // 想要达到的目标位置的水平坐标  min="0" max="36000"
	int  nVertical;				// 想要达到的目标位置的垂直坐标 min="-2000" max="9000"
	u8   bymutiple;             // 10的幂次方数 1,2,3,4,5,6......(默认添2)
	int	 nPanSpeed;				// 到达目标位置时需要具有的水平速度
	int  nTilSpeed;				// 到达目标位置时需要具有的垂直速度
	u32  dwTime;                // 需要多长时间到达目标位置(us)
	char szEvtType[32];         // 安奎拉事件 延迟时间到达位置(aquilla_set_ptmove)
}TPTZAQUILLAPTMOVE,*PTPTZAQUILLAPTMOVE;

//安奎定位设置
typedef struct tagPtzAquillaPtzAngle
{
	u32  dwHorizontal;          // 水平坐标  min="0" max="36000"
	int  nVertical;				// 垂直坐标 min="-2000" max="9000"
	u32  dwRatio;               // 倍率 min ="100" max="3300"
	u8   bymutiple;             // 10的幂次方数 1,2,3,4,5,6......(默认添2)	
	char szEvtType[32];         // 安奎拉事件 安奎拉定位(aquilla_set_ptzangle)
}TPTZAQUILLAPTZANGLE,*PTPTZAQUILLAPTZANGLE;

//云台误差校验结果
typedef struct tagPtzDevCheckRlt
{
	char szMac[MAX_DEV_NAME+1];  //mac地址
	u32  nHvalue;		//水平
	u32  nVvalue;		//垂直
	bool bFinish;       //是否检测结束
} TPTZDEVCHECKRLT, *PTPTZDEVCHECKRLT; 

// 云台巡航
typedef struct tagPtzScan
{
	char	szType[17];		// 水平巡航(horizon_scan),目前只支持一种巡航
	char    szMode[17];     // 开始巡航(open),停止巡航(close)
}TPTZSCAN,*PTPTZSCAN;

// 云台区域缩放
typedef struct tagPtzZoneZoom
{
	char	szType[17];		 // 区域放大(zoompart)，区域缩小(zoomwhole)
	u32	    dwWidth;		 // 区域宽
	u32     dwHeight;        // 区域高
	u32     dwX;             // 区域中间点坐标点--x
	u32     dwY;             // 区域中间点坐标点--y
}TPTZZONEZOOM,*PTPTZZONEZOOM;

// 云台居中移动
typedef struct tagPtzGotoPoint
{
	char	szType[17];		 // 居中(gotopoint)
	u8      byX;             // 坐标点--x(0,255)
	u8      byY;             // 坐标点--y(0,255)
							 // 说明:真实坐标轴以中心点为0点,横纵坐标范围都是(-127~127),由于在结构体中传递的是u8类型
							 // 故要转一下，如-1，传下来是254,如-127，传下来就是128
}TPTZGOTOPOINT,*PTPTZGOTOPOINT;

// 雨刷开启/关闭
typedef struct tagPtzWiper
{
	bool bEnable;           // 开启/关闭
}TPTZWIPER,*PTPTZWIPER;

// 预置位
typedef struct tagPtzPreset
{
	u16	    wSn;		    // 预置位序号,
	bool	bEnable;	    // 是否启用
	char	szName[33];	    // 预置位名称
	char    szEvtType[33];  // 预位置事件类型 设置(preset_set),调用(preset_load),删除(preset_remove)
}TPTZPRESET,*PTPTZPRESET;

typedef struct tagPtzPresetAll		// 全部预置位信息
{
	TPTZPRESET tPtzPreset[256];
}TPTZPRESETALL,*PTPTZPRESETALL;
// 云台基本参数
typedef struct tagPtzBase
{
	char    szPtzVer[32];             // 云台版本(只读)
	u16     wScanRate;               // 扫描速度等级(0~40)
	bool     bEnableDepth;           // 景深比例启用
}TPTZBASE,*PTPTZBASE;
// 云台断电维护
typedef struct tagPtzMainTain
{
	bool   bEnableOutage;         //断电恢复启动
	char   szOutageMemery[7];     //恢复方式outage, preset
	u16    wOutageTime;          // 恢复时间(10~360)
	u16    wPreSet;               // 预置位(1~256)
}TPTZMAINTAIN,*PTPTZMAINTAIN;
// 云台恢复出厂设置
typedef struct tagPtzRefactory
{
	char    szEvtType[32];  // 恢复出厂设置事件(restore_factory_settings)
}TPTZREFACTORY,*PTPTZREFACTORY;
// 云台守望任务
typedef struct tagPtzWatchOn
{
	bool   bEnable;                //守望任务启动
	char   szWatchmode[16];        //守望模式outage, preset
								   //”hscan,vscan,preset,pathcruise,framescan,
								   //randscan,fullviewscan,syncscan”
	u32      dwWaitTime;           // 等待时间(1~36000)
	u16       wPreset;             // 预置位(1~256)
	u8        byPathCruise;        // 扫描路径(1~8)
	u8        bySyncScan;          // 花样扫描路径(1~4)
}TPTZWATCHON,*PTPTZWATCHON;
// 云台限位
typedef struct tagPtzLimit
{
	bool  bKeyLimit;             // 是否启动扫描限位
	bool  bScanLimit;            // 是否启动键控限位
	char  szEvtType[32];         // 限位事件类型 :左限位(limit_horizonalleft) 右限位(limit_horizonalright) 
								 //	键控限位(manuallimitswitch_set) 扫描限位(scanlimitswitch_set)
}TPTZLIMIT,*PTPTZLIMIT;
// 云台巡航扫描
typedef struct tagPtzPathCruise
{
	int	nIndex;	                // 扫描路径索引(1~8)
	int nResvePos;              // 预置点（1~1024）
	u16	wStayTime;	            // 停留时间 (5~1800s)
	bool	bEnable;	        // 是否启用
	char	szName[33];	        // 预置位名称
}TPTZPATHCRUISE,*PTPTZPATHCRUISE;
// 云台巡航扫描事件
typedef struct tagPtzPathCruiseEvt
{
	int	    nIndex;	                // 扫描路径索引(1~8)
	char    szEvtType[32];          // 巡航扫描事件  调用(path_cruise_start) 停止(path_cruise_end)
}TPTZPATHCRUISEEVT,*PTPTZPATHCRUISEEVT;

// 定时任务
typedef struct tagTimeTask
{
	bool  bEnable;            // 是否启动定时任务
	u32   dwWaitTime;       // 时间显示(1~36000)
}TTIMETASK,*PTTIMETASK;

typedef struct tagTimeTaskList
{
	u8	      byWeekDay;	     // 1～7：周日～周六
	u8	      byTimeIndex;	     // 时间段索引
	bool      bEnable;           // 是否启用时间段
	char      szStartTime[32];   // 开始时间
	char      szEndTime[32];     // 结束时间
	char      szCruiseMode[16];  // 守望模式
	u16       wPreset;           // 预置位(1~256)
	u8        byPathCruise;      // 扫描路径(1~8)
	u8        bySyncScan;        // 花样扫描路径(1~4)
	char      szAssParam1[17];   // 辅助参数1,如”alarm,manual,time,trigger”。常规布防不需要填写  
}TTIMETASKLIST,*PTTIMETASKLIST;

//定位
typedef struct tagPtzCoorDinate
{
	u32  dwHorizontal;          // 水平坐标  min="0" max="36000"
	int  nVertical;            // 垂直坐标 min="-2000" max="9000"
	u8   bymutiple;             // 10的幂次方数 1,2,3,4,5,6......
	char szEvtType[32];         // 定位事件 零位校正(turnto_machinezero),水平定位(panposion_set),垂直定位(tiltposion_set)
								// 水平垂直同时定位(ptposion_set)
}TPTZCOORDINATE,*PTPTZCOORDINATE;
//云台倍率
typedef struct tagPtzRatio
{
	u32  dwRatio;              // 倍率
	int  nRatioMin;            // 倍率最小值
	int  nRatioMax;            // 倍率最大值
	char szEvtType[32];        // 倍率事件 (set_zoomratio)
}TPTZRATIO,*PTPTZRATIO;

//定位+云台倍率 (同时设置)
typedef struct tagPtzCoorDinateEx
{
	u32  dwHorizontal;          // 水平坐标  min="0" max="36000"
	int  nVertical;             // 垂直坐标 min="-2000" max="9000"
	u32  dwRatio;               // 倍率	
	u8   bymutiple;             // 10的幂次方数 1,2,3,4,5,6......
	char szEvtType[32];         // ptz同时定位(ptz_set)
}TPTZCOORDINATEEX,*PTPTZCOORDINATEEX;

//云台视场角
typedef struct tagPtzHVangle
{
	u32  dwX;                    // 水平方位信息 
	u32  dwY;                    // 垂直方位信息
	int  nHangleMin;            // 视场角水平最小值
	int  nHangleMax;            // 视场角水平最大值
	int  nVangleMin;            // 视场角垂直最小值
	int  nVangleMax;            // 视场角垂直最大值
}TPTZHVANGLE,*PTPTZHVANGLE;

//websocket服务器上传(用于ptz信息自动获取)
typedef struct tagPtzWsInfo
{
	char szIP[32];					//websocket服务器ip
	u16  wPort;						//websocket服务器端口 	
	char szUrl[MAX_PATH];			//websocket服务器url
}TPTZWSINFO,*PTPTZWSINFO;

// 花样扫描
typedef struct tagPtzPatternsInfo
{
	char  szPatternState[10];         //状态 ”set,scan,idel”
	u16	   wIndex;	                   //扫描或预览的索引
	bool    patternslist[4];          //使用的路径列表
}TPTZPATTERNSINFO,PTPTZPATTERNSINFO;
// 花样扫描事件
typedef struct tagPtzPatternEvt
{
	u8   byPatternPath;          // 花样扫描路径 1,2,3,4
	char szEvtType[32];          // 花样扫描事件 开始记录(syncscan_rec),停止记录(syncscan_stoprec),清除记录(syncscan_delete)
								 //              预览(syncscan_preview),停止(syncscan_stoppreview)
}TPTZPATTERNEVT,*PTPTZPATTERNEVT;

// 布防时间信息
typedef struct tagEventTime
{
	u8	   byWeekDay;		// 1～7：周日～周六
	u8	   byTimeIndex;		// 时间段索引
	bool   bEnable;         // 是否启用时间段
	u16	   wStartTime_Year;	// 起始时间
	u16	   wStartTime_Month;// 
	u16	   wStartTime_Day;	// 
	char   szStartTime[32];
	u16	   wEndTime_Year;	// 结束时间
	u16	   wEndTime_Month;	// 
	u16	   wEndTime_Day;	// 
	char   szEndTime[32];
	char   szAssParam1[17];	// 辅助参数1,如” alarm,manual,time,trigger”。常规布防不需要填写
}TEVENTTIME,*PTEVENTTIME;

//
typedef struct tagTimeEx
{
	char   szTime[32];// 年-月-日 时-分-秒(utc时间)
}TTIMEEX,*PTTIMEEX;
// 月视图结构体
typedef struct tagRecMonthInfo
{
	bool   byHadRec[32];		// 是否有录像
} TRECMONTHINFO, *PTRECMONTHINFO;

// 获取录像数据参数
typedef struct tagRecParam
{
	u32  dwRecType[12];	// 录像类型， emRecType,最多搜索12种指定类型，目前支持:”all,alarm,time,manual”
	TTIMEEX	tStartTime;	// 开始时间
	TTIMEEX 	tEndTime;	// 停止时间
} TRECPARAM, * PTRECPARAM;
// 录像数据结构体
typedef struct tagRecData
{
	u32  dwIndex;		// 索引
	u32  dwRecId;		// 录像id
	u32  dwRecType;		// 录像类型
	TTIMEEX	tStartTime;	// 开始时间
	TTIMEEX 	tEndTime;	// 停止时间
} TRECDATA, * PTRECDATA;

// 开始放像参数
typedef struct tagRecStartPlayParam
{
	u32	dwStartID;
	u32	dwRcvIp;
	u16	wRtpVideoPort;
	u16	wRtcpVideoPort;
	u16	wRtpAudioPort;
	u16	wRtcpAudioPort;
	u16	wRtpAudioPort2;
	u16	wRtcpAudioPort2;
	TTIMEEX	tStartTime;	// 开始时间
	TTIMEEX 	tEndTime;	// 停止时间
	TTIMEEX 	tPlayTime;	// 播放时间
} TRECSTARTPLAYPARAM, * PTRECSTARTPLAYPARAM;

// 放像信息
typedef struct tagRecStartPlayInfo
{
	u32	dwPlayID;
	u16	wRtcpVideoPort;
	u16	wRtcpAudioPort;
	u16	wRtcpAudioPort2;
} TRECSTARTPLAYINFO, * PTRECSTARTPLAYINFO;

// 放像进度信息
typedef struct tagRecPlayStateInfo
{
	char       szState [10]; // 放像状态
	TTIMEEX    tPlayTime;	// 播放时间
	u32        dwTimeStamp; //时间戳
} TRECPLAYSTATEINFO, * PTRECPLAYSTATEINFO;

//
typedef struct KedaTeleZoomAlgoParam
{	
	u32 dwTeleZoomInRatio;  //缩放比例
	u32 dwTeleCurvLevel;     //上部曲度
	u32 dwTeleMultVal;       //上部斜率乘数
	u32 dwWideRatio;        //下部比例
	u32 dwMidWidth;        //畸变校正输出宽度
	u32 dwMidHeight;        //畸变校正输出高度
	u32 dwAdjHeight;       //远端放大调整起始点
	float f32WidthScale;       //宽度适应缩放比例
}KedaTeleZoomAlgoParam_T;

typedef struct tagTeleZoomParam
{	
	u8    bOpen;
	u8    bLevel;
	KedaTeleZoomAlgoParam_T tTeleZoom;
}TTELEZOOMPARAM, *PTTELEZOOMPARAM;

// 畸形矫正
typedef struct tagMalfAlgoParam
{	
	u8 bOpen;			    // 0-关闭，非0-开启
	u32 dwMalfParam1;		 // 畸形矫正主参数
	u32 dwMalfParam2;		// 畸形矫正辅参数
}TMALFPARAM, *PTMALFPARAM;

// 工作模式
typedef struct tagMtcfInfoParam
{
	u8 bMtcf;              // 0-stream2 普通二码流模式 1-mtcf畸变矫正远端放大
}TMTCFPARAM, *PTMTCFPARAM;

// 电池状态
typedef struct tagBatStatusInfo
{
	u16 wBatteryStatus;        // 电池电量(0 ~ 100)
}TBATSTATUSINFO, *PTBATSTATUSINFO;

// wifi探针
typedef struct tagWifiProbeInfo
{
	bool	bEnable;		              // 启用wifi探针
	bool	bReportPlatform;		      // 是否上传平台
	bool	bDisable;					  // 部分功能是否可以设置(此值不可更改)
										  // 为true则bMacEnable,bSignalEnable,bTimeEnable,dwLongitude,dwLatitude均不可设置

	bool	bMacEnable;					  // 是否显示移动终端Mac地址
	bool	bSignalEnable;		          // 是否显示信号强度
	bool	bTimeEnable;		          // 是否显示探针时间
	int	    nLongitude;		              // 经度
	int	    nLongitudeMax;		          // 经度最大值
	int	    nLongitudeMin;		          // 经度最小值
	int	    nLatitude;					  // 纬度
	int	    nLatitudeMax;				  // 纬度最大值
	int	    nLatitudeMin;				  // 纬度最小值
	char	szProvinceCode[MAX_PATH];     // 所在省代码(只能是数字和字母)
	char	szCityCode[MAX_PATH];		  // 所在市代码(只能是数字和字母)
	char	szAreaCode[MAX_PATH];		  // 所在地区代码(只能是数字和字母)
	char	szAddress[MAX_PATH];		  // 所在地点名称(不能为特殊字符)
	int	    dwDeviceNum;			      // 搜索到的终端数目(用来开辟tagWifiProbeListInfo的内存)
}TWIFIPROBEINFO, *PTWIFIPROBEINFO;

// wifi探针设备列表
typedef struct tagWifiProbeListInfo
{
	char	szMac[MAX_PATH];          // 移动终端Mac地址
	char	szSignal[MAX_PATH];		  // 信号强度("bad,middle,good")
	char	szTime[MAX_PATH];		  // 探针时间
}TWIFIPROBELISTINFO, *PTWIFIPROBELISTINFO;

// 调色板
typedef struct tagColorSize
{
	char	szColor[64];			// whitehot,blackhot,light,rainbow,north,hotiron,delaminate,medical,red
}TCOLORSIZE, *PTCOLORSIZE;

// 热点追踪状态
typedef struct tagHotState
{
	char	szState[64];			// open,close
}THOTSTATE, *PTHOTSTATE;

// 地理位置信息
typedef struct tagPositionInfo
{
	bool	bEnable;		              // 启用
	char	szPosmode[32];				  // 地理位置模式
	int	    nRtpInterval;			      // 数据上传间隔
	char	szEWindicador[32];			  // 东经或西经
	char	szNSindicador[32];			  // 北纬或南纬	
	char	szSpdUnit[8];		          // 速度单位
	char	szLocation[32];		          // 当前位置
	int     nSpeedMile;
	int     nSpeedKm;
}TPOSITIONINFO, *PTPOSITIONINFO;

//基于虚拟零位的当前云台角度信息
typedef struct tagBasicPosInfo
{
	s32 nHpos;					//云台水平位置
	s32 nVpos;					//云台垂直位置
	s32 nZoomValue;				//zoom值
}TBASICPOSINFO,*PTBASICPOSINFO;

//登陆信息
typedef struct tagLoginInfo
{
	u32 dwIP;
	u16 wPort;
	char szUser[64];
	char szPasswd[64];
	u16 wTime;			//检测是否在线间隔时间，单位：豪秒
}TLOGININFO,*PTLOGININFO;

typedef struct tagDeployPicUploadParam
{
	char  szPicName[128];	//图片名称
	u32   dwPicSize;	//图片大小
	char* szPicData;	//图片数据
}TDEPLOYUPLOADPARAM, *PTDEPLOYUPLOADPARAM;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif
