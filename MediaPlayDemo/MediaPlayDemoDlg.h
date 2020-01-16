
// MediaPlayDemoDlg.h : header file
//

#pragma once

#include <vector>
#include "MediaPlay/MediaPlay.h"
#include "Arithmetic/Arithmetic.h"
#include "JpegEncoder/JpegEncoder.h"

// MediaPlayDemoDlg dialog
class MediaPlayDemoDlg : public CDialogEx
{
// Construction
public:
	MediaPlayDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIAPLAYDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	static void CALLBACK postMediaDataCallback(
		const int playID = 0, const unsigned char* mediaData = NULL, const int dataBytes = 0, void* userData = NULL);
	static void CALLBACK postDetectAlarmInfoCallback(
		const AlarmInfo* alarmInfos, const unsigned int count = 0, const unsigned char* mediaData = NULL, const unsigned long long  dataBytes = 0, void* userData = NULL);
	static void CALLBACK postJpegEncodeCallback(
		const unsigned char* jpegData, const int jpegBytes, void* userData);

public:
	afx_msg void OnBnClickedRegisterHelmet();
	afx_msg void OnBnClickedRegisterPhone();
	afx_msg void OnBnClickedRegisterFight();
	afx_msg void OnBnClickedRegisterSleep();
	afx_msg void OnBnClickedRegisterFace();

private:
	AlarmType alarmType;
public:
	afx_msg void OnBnClickedButton1();
};
