
// demoDlg.h : header file
//

#pragma once

#include <vector>
#include "cuviddec.h"
#include "nvcuvid.h"

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
	DECLARE_MESSAGE_MAP()

private:
	void insertLogItem(const CString function, const CUresult result);
	static DWORD WINAPI decodeFrameThread(LPVOID lpThreadParameter = NULL);
	/**
	*   @brief  Callback function to be registered for getting a callback when decoding of sequence starts
	*/
	static int CUDAAPI handleVideoSequenceCallback(void* ctx = NULL, CUVIDEOFORMAT* cuVideoFormat = NULL);
	/**
	*   @brief  Callback function to be registered for getting a callback when a decoded frame is ready to be decoded
	*/
	static int CUDAAPI handlePictureDecodeCallback(void* ctx = NULL, CUVIDPICPARAMS* cuvidPictureParams = NULL);
	/**
	*   @brief  Callback function to be registered for getting a callback when a decoded frame is available for display
	*/
	static int CUDAAPI handlePictureDisplayCallback(void* ctx = NULL, CUVIDPARSERDISPINFO* cuvidParserDisplayInfo = NULL);
	/**
*   @brief  This function gets called when a sequence is ready to be decoded. The function also gets called
	when there is format change
*/
	int videoSequenceProcess(CUVIDEOFORMAT* cuVideoFormat = NULL);
	/**
	*   @brief  This function gets called when a picture is ready to be decoded. cuvidDecodePicture is called from this function
	*   to decode the picture
	*/
	int pictureDecodeProcess(CUVIDPICPARAMS* cuvidPictureParams = NULL);
	/**
	*   @brief  This function gets called after a picture is decoded and available for display. Frames are fetched and stored in
		internal buffer
	*/
	int pictureDisplayProcess(CUVIDPARSERDISPINFO* cuvidParserDisplayInfo = NULL);

private:
	std::vector<CUdevice> cuDevices;
	std::vector<CUcontext> cuContext;
	CUvideoctxlock cuVideoCtxLock;
	CUvideoparser cuVideoParser;
	CUvideodecoder cuVideoDecoder;
	CListCtrl logListCtrl;
};
