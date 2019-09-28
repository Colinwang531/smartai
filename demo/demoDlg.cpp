
// demoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"

#include "MediaDecoder/CUDA/CUDAVideoDecoder.h"
using CUDAVideoDecoder = NS(decoder, 1)::CUDAVideoDecoder;
using MediaDecoder = NS(decoder, 1)::MediaDecoder;

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
	: CDialogEx(IDD_DEMO_DIALOG, pParent), dxgiAdapter{ NULL }
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
	ON_COMMAND(ID_OPEN_FILE, &CdemoDlg::OnOpenFile)
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

	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	DWORD currentLogListStyle{ logListCtrl.GetExtendedStyle() };
	logListCtrl.SetExtendedStyle(currentLogListStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	logListCtrl.InsertColumn(0, L"Function Name");
	logListCtrl.InsertColumn(1, L"Invoke result");
	logListCtrl.InsertColumn(2, L"Message");
	CRect currentLogListRect;
	logListCtrl.GetClientRect(&currentLogListRect);
	const int displayWidthPerCol{ currentLogListRect.Width() / 3 };
	logListCtrl.SetColumnWidth(0, displayWidthPerCol);
	logListCtrl.SetColumnWidth(1, displayWidthPerCol);
	logListCtrl.SetColumnWidth(2, displayWidthPerCol);

	statusBarCtrl.Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0xFDEF);
	int statusBarParts[]{ 180, 360, 200, 200, 200, 200, 200, 200, -1 };
	statusBarCtrl.SetParts(9, statusBarParts);

	initCudaDriver();

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

void CdemoDlg::initCudaDriver(void)
{
	CUresult cuResult{ cuInit(0) };
	insertLogItem(L"cuInit", cuResult);

	int driverVersion{ 0 };
	cuResult = cuDriverGetVersion(&driverVersion);
	insertLogItem(L"cuDriverGetVersion", cuResult);
	CString driverVersionStr;
	driverVersionStr.Format(L"CUDA driver version : %d", driverVersion);
	statusBarCtrl.SetText(driverVersionStr, 0, 0);

	int deviceCount{ 0 };
	cuResult = cuDeviceGetCount(&deviceCount);
	insertLogItem(L"cuDeviceGetCount", cuResult);
	CString driverCountStr;
	driverCountStr.Format(L"CUDA driver count : %d", deviceCount);
	statusBarCtrl.SetText(driverCountStr, 1, 0);

	for (int i = 0; i != deviceCount; ++i)
	{
		CUdevice cuDevice{ 0 };
		cuResult = cuDeviceGet(&cuDevice, i);
		insertLogItem(L"cuDeviceGet", cuResult);

		CUcontext cuContext{ NULL };
		cuResult = cuCtxCreate(&cuContext, CU_CTX_SCHED_AUTO, cuDevice);
		insertLogItem(L"cuCtxCreate", cuResult);

		if (-1 < cuDevice)
		{
			cuDevices.push_back(cuDevice);
			cuContexts.push_back(cuContext);

			char deviceName[256]{ 0 };
			cuResult = cuDeviceGetName(deviceName, 256, cuDevice);
			insertLogItem(L"cuDeviceGetName", cuResult);

			if (CUDA_SUCCESS == cuResult)
			{
				CString deviceNameStr{ deviceName };
				HTREEITEM hTreeParent{ ((CTreeCtrl*)GetDlgItem(IDC_DEVICE_LIST))->InsertItem(deviceNameStr) };

				CUuuid cuUUID{ 0 };
				cuResult = cuDeviceGetUuid(&cuUUID, cuDevice);
				insertLogItem(L"cuDeviceGetUuid", cuResult);
				if (CUDA_SUCCESS == cuResult)
				{
					CString deviceUUIDStr;
					deviceUUIDStr.Format(L"UUID : [%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d]", 
						cuUUID.bytes[0], cuUUID.bytes[1], cuUUID.bytes[2], cuUUID.bytes[3],
						cuUUID.bytes[4], cuUUID.bytes[5], cuUUID.bytes[6], cuUUID.bytes[7], 
						cuUUID.bytes[8], cuUUID.bytes[9], cuUUID.bytes[10], cuUUID.bytes[11], 
						cuUUID.bytes[12], cuUUID.bytes[13], cuUUID.bytes[14], cuUUID.bytes[15]);
					((CTreeCtrl*)GetDlgItem(IDC_DEVICE_LIST))->InsertItem(deviceUUIDStr, hTreeParent);
				}

				std::size_t deviceTotalMemory{ 0 };
				cuResult = cuDeviceTotalMem(&deviceTotalMemory, cuDevice);
				insertLogItem(L"cuDeviceTotalMem", cuResult);
				if (CUDA_SUCCESS == cuResult)
				{
					CString deviceTotalMemStr;
					deviceTotalMemStr.Format(L"Memory size : [%llu (Bytes)]", deviceTotalMemory);
					((CTreeCtrl*)GetDlgItem(IDC_DEVICE_LIST))->InsertItem(deviceTotalMemStr, hTreeParent);
				}

				((CTreeCtrl*)GetDlgItem(IDC_DEVICE_LIST))->Expand(hTreeParent, TVE_EXPAND);
			}
		}
	}

}

void CdemoDlg::initCudaDecoder(void)
{
// 	for (int i = 0; i != cuDevices.size(); ++i)
// 	{
// 		CUcontext ctx{ NULL };
// 		CUresult cuResult{ cuCtxCreate(&ctx, CU_CTX_SCHED_AUTO, cuDevices[i]) };
// 		insertLogItem(L"cuCtxCreate", cuResult);
// 
// 		if (CUDA_SUCCESS == cuResult)
// 		{
// 			cuResult = cuvidCtxLockCreate(&cuVideoCtxLock, ctx);
// 			insertLogItem(L"cuvidCtxLockCreate", cuResult);
// 
// 		}
// 
// 		cuContext.push_back(ctx);
// 	}
}

void CdemoDlg::initCudaD3d11Renderer(void)
{
// 	IDXGIFactory* dxgiFactory{ NULL };
// 	HRESULT hResult{ CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& dxgiFactory) };
// 
// 	if (SUCCEEDED(hResult))
// 	{
// 		for (int i = 0; i != 100; ++i)
// 		{
// 			IDXGIAdapter* adapter{ NULL };
// 			hResult = dxgiFactory->EnumAdapters(i, &adapter);
// 
// 			if (FAILED(hResult))
// 			{
// 				break;
// 			}
// 
// 			int cudaDevice{ -1 };
// 			cudaError error = cudaD3D11GetDevice(&cudaDevice, adapter);
// 			insertLogItem(L"cudaD3D11GetDevice", error);
// 
// 			if (cudaSuccess == error)
// 			{
// 				dxgiAdapter = adapter;
// 				dxgiAdapter->AddRef();
// 			}
// 
// 			adapter->Release();
// 		}
// 	}
}

void CdemoDlg::insertLogItem(const CString function, const CUresult error)
{
	int currentColumeNumber{ logListCtrl.InsertItem(logListCtrl.GetItemCount(), function) };
	CString errorNum, errorStr;
	errorNum.Format(L"%d", error);
//	errorStr.Format(L"%s", cudaGetErrorString(error));
	logListCtrl.SetItemText(currentColumeNumber, 1, errorNum);
//	logListCtrl.SetItemText(currentColumeNumber, 2, errorStr);
}

DWORD CdemoDlg::decodeFrameThread(LPVOID ctx /* = NULL */)
{
	CdemoDlg* demo = reinterpret_cast<CdemoDlg*>(ctx);

	if (demo)
	{
		MediaDecoder* mediaDecoder = new CUDAVideoDecoder(demo->cuContexts[0]);

		if (mediaDecoder)
		{
			int len = WideCharToMultiByte(CP_ACP, 0, demo->openFilePath, demo->openFilePath.GetLength(), NULL, 0, NULL, NULL);
			unsigned char filePath[MAX_PATH]{ 0 };
			WideCharToMultiByte(CP_ACP, 0, demo->openFilePath, demo->openFilePath.GetLength(), (char*)filePath, len, NULL, NULL);
			mediaDecoder->decode(filePath);
			delete mediaDecoder;
		}
	}

	return 0;
}


void CdemoDlg::OnOpenFile()
{
	// TODO: Add your command handler code here

	CFileDialog fd(TRUE);
	if (IDOK == fd.DoModal())
	{
		openFilePath = fd.GetPathName();

		if (!openFilePath.IsEmpty())
		{
			DWORD threadID{ 0 };
			CreateThread(NULL, 0, &CdemoDlg::decodeFrameThread, this, 0, &threadID);
		}
	}
}
