#pragma once
#include "afxwin.h"


// CExitDialog �Ի���

class CExitDialog : public CDialog
{
	DECLARE_DYNAMIC(CExitDialog)

public:
	CExitDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CButton exitButton_;
public:
	afx_msg void OnBnClickedOk();
};
