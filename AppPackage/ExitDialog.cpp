// ExitDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AppPackage.h"
#include "ExitDialog.h"


// CExitDialog �Ի���

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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CExitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, exitButton_);
}


BEGIN_MESSAGE_MAP(CExitDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CExitDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CExitDialog ��Ϣ�������

void CExitDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitProcess(0);
	OnOK();
}
