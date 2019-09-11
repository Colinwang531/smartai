
// demoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"

#include "FFmpegDemuxer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdemoDlg dialog



CdemoDlg::CdemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LIST, logListCtrl);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CdemoDlg message handlers

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	DWORD currentLogListStyle{ logListCtrl.GetExtendedStyle() };
	logListCtrl.SetExtendedStyle(currentLogListStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	logListCtrl.InsertColumn(0, L"Function Name");
	logListCtrl.InsertColumn(1, L"Invoke result");
	CRect currentLogListRect;
	logListCtrl.GetClientRect(&currentLogListRect);
	const int displayWidthPerCol{ currentLogListRect.Width() / 2 };
	logListCtrl.SetColumnWidth(0, displayWidthPerCol);
	logListCtrl.SetColumnWidth(1, displayWidthPerCol);

	//cuInit
	CUresult result{ cuInit(0) };
	insertLogItem(L"cuInit", result);

	//cuDeviceGetCount
	int deviceCount{ 0 };
	result = cuDeviceGetCount(&deviceCount);
	insertLogItem(L"cuDeviceGetCount", result);

	for (int i = 0; i != deviceCount; ++i)
	{
		CUdevice cuDevice{ NULL };

		//cuDeviceGet
		result = cuDeviceGet(&cuDevice, i);
		insertLogItem(L"cuDeviceGet", result);

		if (CUDA_SUCCESS == result)
		{
			char deviceName[100]{ 0 };
			//cuDeviceGetName
			result = cuDeviceGetName(deviceName, sizeof(deviceName), cuDevice);
			insertLogItem(L"cuDeviceGetName", result);

			if (CUDA_SUCCESS == result)
			{
				CString deviceNameUsingWideBytes{ deviceName };
				((CTreeCtrl*)GetDlgItem(IDC_DEVICE_LIST))->InsertItem(deviceNameUsingWideBytes);
				cuDevices.push_back(cuDevice);
			}
		}
	}

	for (int i = 0; i != cuDevices.size(); ++i)
	{
		CUcontext ctx{ NULL };

		//cuCtxCreate
		result = cuCtxCreate(&ctx, CU_CTX_SCHED_AUTO, cuDevices[i]);
		insertLogItem(L"cuCtxCreate", result);
		if (CUDA_SUCCESS == result)
		{
			//cuvidCtxLockCreate
			result = cuvidCtxLockCreate(&cuVideoCtxLock, ctx);
			insertLogItem(L"cuvidCtxLockCreate", result);

			if (CUDA_SUCCESS == result)
			{
				CUVIDPARSERPARAMS videoParserParameters{};
				videoParserParameters.CodecType = cudaVideoCodec_H264;
				videoParserParameters.ulMaxNumDecodeSurfaces = 1;
				videoParserParameters.ulMaxDisplayDelay = 0;// bLowLatency ? 0 : 1;
				videoParserParameters.pUserData = this;
				videoParserParameters.pfnSequenceCallback = handleVideoSequenceCallback;
				videoParserParameters.pfnDecodePicture = handlePictureDecodeCallback;
				videoParserParameters.pfnDisplayPicture = handlePictureDisplayCallback;

				//cuvidCreateVideoParser
				result = cuvidCreateVideoParser(&cuVideoParser, &videoParserParameters);
				insertLogItem(L"cuvidCreateVideoParser", result);

				DWORD threadID{ 0 };
				CreateThread(NULL, 0, &CdemoDlg::decodeFrameThread, this, 0, &threadID);
			}
		}

		cuContext.push_back(ctx);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdemoDlg::insertLogItem(const CString function, const CUresult result)
{
	int currentColumeNumber{ logListCtrl.InsertItem(logListCtrl.GetItemCount(), function) };
	CString resultStr;
	resultStr.Format(L"%d", result);
	logListCtrl.SetItemText(currentColumeNumber, 1, resultStr);
}

int CdemoDlg::handleVideoSequenceCallback(void* ctx /* = NULL */, CUVIDEOFORMAT* cuVideoFormat /* = NULL */)
{ 
	return ((CdemoDlg*)ctx)->videoSequenceProcess(cuVideoFormat);
}

int CdemoDlg::handlePictureDecodeCallback(void* ctx /* = NULL */, CUVIDPICPARAMS* cuvidPictureParams /* = NULL */)
{ 
	return ((CdemoDlg*)ctx)->pictureDecodeProcess(cuvidPictureParams);
}

int CdemoDlg::handlePictureDisplayCallback(void* ctx /* = NULL */, CUVIDPARSERDISPINFO* cuvidParserDisplayInfo /* = NULL */)
{ 
	return ((CdemoDlg*)ctx)->pictureDisplayProcess(cuvidParserDisplayInfo);
}

int CdemoDlg::videoSequenceProcess(CUVIDEOFORMAT* cuVideoFormat /*= NULL*/)
{
	//cuCtxPushCurrent
	CUresult result{ cuCtxPushCurrent(cuContext[0]) };
	insertLogItem(L"cuCtxPushCurrent", result);

	if (CUDA_SUCCESS == result)
	{
		CUVIDDECODECREATEINFO cuvidDecoderCreateInfo{ 0 };
		cuvidDecoderCreateInfo.ulWidth = cuVideoFormat->coded_width;
		cuvidDecoderCreateInfo.ulHeight = cuVideoFormat->coded_height;
		cuvidDecoderCreateInfo.ulNumDecodeSurfaces = 1;// 20;
		cuvidDecoderCreateInfo.CodecType = cudaVideoCodec_H264;
		cuvidDecoderCreateInfo.ChromaFormat = cuVideoFormat->chroma_format;
		cuvidDecoderCreateInfo.ulCreationFlags = cudaVideoCreate_PreferCUVID;
		cuvidDecoderCreateInfo.bitDepthMinus8 = cuVideoFormat->bit_depth_luma_minus8;
		cuvidDecoderCreateInfo.ulIntraDecodeOnly = 0;
		cuvidDecoderCreateInfo.ulMaxWidth = 4096;// cuVideoFormat->coded_width;
		cuvidDecoderCreateInfo.ulMaxHeight = 2160;// cuVideoFormat->coded_height;
		cuvidDecoderCreateInfo.OutputFormat = cudaVideoSurfaceFormat_NV12;
		cuvidDecoderCreateInfo.DeinterlaceMode = cudaVideoDeinterlaceMode_Weave;
		cuvidDecoderCreateInfo.ulNumOutputSurfaces = 1;// 2;
		cuvidDecoderCreateInfo.vidLock = cuVideoCtxLock;

		//cuvidCreateDecoder
		result = cuvidCreateDecoder(&cuVideoDecoder, &cuvidDecoderCreateInfo);
		insertLogItem(L"cuvidCreateDecoder", result);
		//cuCtxPopCurrent
		result = cuCtxPopCurrent(&cuContext[0]);
		insertLogItem(L"cuCtxPopCurrent", result);
	}

	return 1;
}

int CdemoDlg::pictureDecodeProcess(CUVIDPICPARAMS* cuvidPictureParams /*= NULL*/)
{
	if (cuVideoDecoder)
	{
		//cuCtxPopCurrent
		CUresult result{ cuvidDecodePicture(cuVideoDecoder, cuvidPictureParams) };
		if (CUDA_SUCCESS != result)
		{
			insertLogItem(L"cuvidDecodePicture", result);
		}
	}

	return 1;
}

int CdemoDlg::pictureDisplayProcess(CUVIDPARSERDISPINFO* cuvidParserDisplayInfo /*= NULL*/)
{
	return 0;
}

DWORD CdemoDlg::decodeFrameThread(LPVOID lpThreadParameter /* = NULL */)
{
	CdemoDlg* demo = reinterpret_cast<CdemoDlg*>(lpThreadParameter);

	if (demo)
	{
// 		FFmpegDemuxer ffDemuxer{
// 			"d:\\download\\xXx.Return.of.Xander.Cage.2017.1080p.BluRay.x264.Atmos.TrueHD.7.1-HDChina.mkv"};
		FFmpegDemuxer ffDemuxer{
			"d:\\download\\IP Camera7_NEW VANGUARD_NEW VANGUARD_20190522083340_20190522084338_7170782.mp4" };
		AVCodecID codecID{ ffDemuxer.GetVideoCodec() };
		const int imageWidth{ ffDemuxer.GetWidth() }, imageHeight{ ffDemuxer.GetHeight() };
		long long totalFileDataBytes = 0;

		while(1)
		{
			uint8_t* demuxVideoData{ NULL };
			int demuxVideoBytes{ 0 };
			ffDemuxer.Demux(&demuxVideoData, &demuxVideoBytes);
			totalFileDataBytes += demuxVideoBytes;

			CUVIDSOURCEDATAPACKET cuvidSourceDataPacket{ 0 };
			cuvidSourceDataPacket.payload = demuxVideoData;
			cuvidSourceDataPacket.payload_size = demuxVideoBytes;
			cuvidSourceDataPacket.flags |= CUVID_PKT_TIMESTAMP;
			cuvidSourceDataPacket.timestamp = GetTickCount64();

			//cuvidCreateVideoParser
			CUresult result{ cuvidParseVideoData(demo->cuVideoParser, &cuvidSourceDataPacket) };
//			demo->insertLogItem(L"cuvidParseVideoData", result);

			if (!demuxVideoData && !demuxVideoBytes)
			{
				break;
			}
		}
	}

	return 0;
}
