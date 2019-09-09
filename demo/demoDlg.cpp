
// demoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"

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

