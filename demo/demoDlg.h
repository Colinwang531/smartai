
// demoDlg.h : header file
//

#pragma once

#include <vector>
#include "cuviddec.h"
#include "nvcuvid.h"
//#include <cuda_runtime_api.h>
#include <cuda_d3d11_interop.h>

// CdemoDlg dialog
class CdemoDlg : public CDialogEx
{
// Construction
public:
	CdemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
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
	afx_msg void OnOpenFile();
	DECLARE_MESSAGE_MAP()

private:
	void initCudaDriver(void);
	void initCudaDecoder(void);
	void initCudaD3d11Renderer(void);
	void insertLogItem(const CString function, const CUresult error);
	static DWORD WINAPI decodeFrameThread(LPVOID lpThreadParameter = NULL);

private:
	CMenu menu;
	CListCtrl logListCtrl;
	CStatusBarCtrl statusBarCtrl;
	CString openFilePath;

	IDXGIAdapter* dxgiAdapter;
	std::vector<CUdevice> cuDevices;
	std::vector<CUcontext> cuContexts;
};
