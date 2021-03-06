
// MediaPlayDemoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MediaPlayDemo.h"
#include "MediaPlayDemoDlg.h"
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


// MediaPlayDemoDlg dialog



MediaPlayDemoDlg::MediaPlayDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEDIAPLAYDEMO_DIALOG, pParent), alarmType{ AlarmType::ALARM_TYPE_NONE }
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MediaPlayDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MediaPlayDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REGISTER_HELMET, &MediaPlayDemoDlg::OnBnClickedRegisterHelmet)
	ON_BN_CLICKED(IDC_REGISTER_PHONE, &MediaPlayDemoDlg::OnBnClickedRegisterPhone)
	ON_BN_CLICKED(IDC_REGISTER_FIGHT, &MediaPlayDemoDlg::OnBnClickedRegisterFight)
	ON_BN_CLICKED(IDC_REGISTER_SLEEP, &MediaPlayDemoDlg::OnBnClickedRegisterSleep)
	ON_BN_CLICKED(IDC_REGISTER_FACE, &MediaPlayDemoDlg::OnBnClickedRegisterFace)
END_MESSAGE_MAP()


// MediaPlayDemoDlg message handlers

BOOL MediaPlayDemoDlg::OnInitDialog()
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

//	JPEGENCODER_RegisterPostJpegEncodeCallback(&MediaPlayDemoDlg::postJpegEncodeCallback, this);
	// 	MEDIAPLAY_StartPlayback("D:\\video\\IP Camera7_NEW VANGUARD_NEW VANGUARD_20190522083340_20190522084338_7170782.mp4", GetDlgItem(IDC_STATIC2)->GetSafeHwnd());
//  	MEDIAPLAY_StartPlayback("D:\\video\\Avengers Endgame.2019.HD1080P.X264.AAC.English.CHS-ENG.Mp4Ba.mp4", GetDlgItem(IDC_STATIC1)->GetSafeHwnd());
//  	MEDIAPLAY_StartPlayback("D:\\video\\xXx.Return.of.Xander.Cage.2017.1080p.BluRay.x264.Atmos.TrueHD.7.1-HDChina.mkv", GetDlgItem(IDC_STATIC2)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\Camera.mp4", GetDlgItem(IDC_STATIC3)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\Camera.mp4", GetDlgItem(IDC_STATIC4)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\Avengers Endgame.2019.HD1080P.X264.AAC.English.CHS-ENG.Mp4Ba.mp4", GetDlgItem(IDC_STATIC5)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\xXx.Return.of.Xander.Cage.2017.1080p.BluRay.x264.Atmos.TrueHD.7.1-HDChina.mkv", GetDlgItem(IDC_STATIC6)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\Camera.mp4", GetDlgItem(IDC_STATIC7)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\Avengers Endgame.2019.HD1080P.X264.AAC.English.CHS-ENG.Mp4Ba.mp4", GetDlgItem(IDC_STATIC8)->GetSafeHwnd());
// 	MEDIAPLAY_StartPlayback("D:\\video\\xXx.Return.of.Xander.Cage.2017.1080p.BluRay.x264.Atmos.TrueHD.7.1-HDChina.mkv", GetDlgItem(IDC_STATIC9)->GetSafeHwnd());
// 	MEDIAPLAY_StartLivestream(
// 		"admin", "eaton12345", "192.168.30.12", 8000, 1, GetDlgItem(IDC_STATIC7)->GetSafeHwnd(), &MediaPlayDemoDlg::postMediaDataCallback, this);
// 	MEDIAPLAY_StartLivestreamPlay(
// 		"admin", "eaton12345", "192.168.30.12", 8000, 0, GetDlgItem(IDC_STATIC3)->GetSafeHwnd(), &MediaPlayDemoDlg::postMediaDataCallback, this);

	MEDIAPLAY_OpenStream(
		"livestream://admin:eaton12345@192.168.30.12:8000?channel=1&stream=1&sms=192.168.30.76:27820", GetDlgItem(IDC_STATIC7)->GetSafeHwnd(), &MediaPlayDemoDlg::postMediaDataCallback, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MediaPlayDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void MediaPlayDemoDlg::OnPaint()
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
HCURSOR MediaPlayDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void MediaPlayDemoDlg::postMediaDataCallback(
	const int playID /* = 0 */, const unsigned char frameType /* = 0 */, const unsigned char* mediaData /* = NULL */, const int dataBytes /* = 0 */, void* userData /* = NULL */)
{
	MediaPlayDemoDlg* demo{ reinterpret_cast<MediaPlayDemoDlg*>(userData) };
	if (1 == playID/* && AlarmType::ALARM_TYPE_NONE != demo->alarmType*/)
	{
		MediaPlayDemoDlg* demo{ reinterpret_cast<MediaPlayDemoDlg*>(userData) };
//		ARITHMETIC_InputImageData(demo->alarmType, mediaData, dataBytes);
	}
}

void MediaPlayDemoDlg::postDetectAlarmInfoCallback(const AlarmInfo alarmInfo, const unsigned char* mediaData /* = NULL */, const int dataBytes /* = 0 */, void* userData /* = NULL */)
{
	if (AlarmType::ALARM_TYPE_FACE == alarmInfo.type)
	{
		char text[2048]{ 0 };
		sprintf_s(text, 2048, "Face detected ID = %d, similarity = %f.\r\n", alarmInfo.faceID, alarmInfo.similarity);
		OutputDebugStringA(text);
	}
	else
	{
		char text[2048]{ 0 };
		sprintf_s(text, 2048, "Helmet alarm x = %d, y = %d, w = %d, h = %d, label = %d.\r\n",
			alarmInfo.x, alarmInfo.y, alarmInfo.w, alarmInfo.h, alarmInfo.status);
		OutputDebugStringA(text);

		//JPEGENCODER_EncodeJpegPicture(mediaData, dataBytes);
	}
}

void MediaPlayDemoDlg::postJpegEncodeCallback(const unsigned char* jpegData, const int jpegBytes, void* userData)
{
	char text[2048]{ 0 };
	sprintf_s(text, 2048, "Save JPEG picture %d bytes.\r\n", jpegBytes);
	OutputDebugStringA(text);
}

void MediaPlayDemoDlg::OnBnClickedRegisterHelmet()
{
	// TODO: Add your control notification handler code here

//	ARITHMETIC_RegisterAlarmNotifyCallback(AlarmType::ALARM_TYPE_HELMET, &MediaPlayDemoDlg::postDetectAlarmInfoCallback, this);
	alarmType = AlarmType::ALARM_TYPE_HELMET;
}


void MediaPlayDemoDlg::OnBnClickedRegisterPhone()
{
	// TODO: Add your control notification handler code here
//	ARITHMETIC_RegisterAlarmNotifyCallback(AlarmType::ALARM_TYPE_PHONE, &MediaPlayDemoDlg::postDetectAlarmInfoCallback, this);
	alarmType = AlarmType::ALARM_TYPE_PHONE;
}


void MediaPlayDemoDlg::OnBnClickedRegisterFight()
{
	// TODO: Add your control notification handler code here
//	ARITHMETIC_RegisterAlarmNotifyCallback(AlarmType::ALARM_TYPE_FIGHT, &MediaPlayDemoDlg::postDetectAlarmInfoCallback, this);
	alarmType = AlarmType::ALARM_TYPE_FIGHT;
}


void MediaPlayDemoDlg::OnBnClickedRegisterSleep()
{
	// TODO: Add your control notification handler code here
//	ARITHMETIC_RegisterAlarmNotifyCallback(AlarmType::ALARM_TYPE_SLEEP, &MediaPlayDemoDlg::postDetectAlarmInfoCallback, this);
	alarmType = AlarmType::ALARM_TYPE_SLEEP;
}


void MediaPlayDemoDlg::OnBnClickedRegisterFace()
{
	// TODO: Add your control notification handler code here
//	ARITHMETIC_RegisterAlarmNotifyCallback(AlarmType::ALARM_TYPE_FACE, &MediaPlayDemoDlg::postDetectAlarmInfoCallback, this);
	alarmType = AlarmType::ALARM_TYPE_FACE;

// 	int ret{ ARITHMETIC_AddFaceImage("C:\\Users\\CPTAI\\Desktop\\wangkewei.jpg", 1) };
// 	char text[2048]{ 0 };
// 	sprintf_s(text, 2048, "Add face picture result %d, ID %d.\r\n", ret, 1);
// 	OutputDebugStringA(text);
}
