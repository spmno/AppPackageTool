// AppPackageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AppPackage.h"
#include "AppPackageDlg.h"
#include "ExitDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAppPackageDlg 对话框

CAppPackageDlg::CAppPackageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppPackageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppPackageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_PACKAGE, progressBar_);
	DDX_Control(pDX, IDC_STATIC_INFO, displayInfo_);
}

BEGIN_MESSAGE_MAP(CAppPackageDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CAppPackageDlg 消息处理程序

BOOL CAppPackageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HDC hdc = ::GetDC(NULL);
	unsigned short screen_width = GetDeviceCaps(hdc, HORZRES);
	unsigned short screen_height = GetDeviceCaps(hdc, VERTRES);
	this->MoveWindow(0, 0, screen_width, screen_height);
	progressBar_.MoveWindow(0, screen_height/4, screen_width, screen_height/4);
	displayInfo_.MoveWindow(screen_width/3, screen_height/2, screen_width/3, screen_height/4);
	decompressManager_.decompressWithDelegate(this, true);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CAppPackageDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_APPPACKAGE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_APPPACKAGE_DIALOG));
	}
}
#endif

void CAppPackageDlg::updateZipInfo(DWORD zipFileCount)
{
	progressBar_.SetRange(0, zipFileCount);
	progressBar_.SetStep(1);
}

void CAppPackageDlg::updateStep(const TCHAR *fileName)
{
	progressBar_.StepIt();
}

void CAppPackageDlg::decompressError(DECOMPRESS_ERROR errorCode)
{
	TCHAR errorString[MAX_PATH];
	_stprintf(errorString, L"error: %d", errorCode);
	MessageBox(errorString);
}

void CAppPackageDlg::decompressFinish()
{
	ExitProcess(0);
}