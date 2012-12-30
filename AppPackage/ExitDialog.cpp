// ExitDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "AppPackage.h"
#include "ExitDialog.h"


// CExitDialog 对话框

IMPLEMENT_DYNAMIC(CExitDialog, CDialog)

CExitDialog::CExitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDialog::IDD, pParent)
{

}

CExitDialog::~CExitDialog()
{
}

BOOL CExitDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	HDC hdc = ::GetDC(NULL);
	unsigned short screen_width = GetDeviceCaps(hdc, HORZRES);
	unsigned short screen_height = GetDeviceCaps(hdc, VERTRES);
	this->MoveWindow(0, 0, screen_width, screen_height);
	exitButton_.MoveWindow(0, 0, screen_width, screen_height);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CExitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, exitButton_);
}


BEGIN_MESSAGE_MAP(CExitDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CExitDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CExitDialog 消息处理程序

void CExitDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitProcess(0);
	OnOK();
}
