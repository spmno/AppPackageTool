#pragma once
#include "afxwin.h"


// CExitDialog 对话框

class CExitDialog : public CDialog
{
	DECLARE_DYNAMIC(CExitDialog)

public:
	CExitDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CButton exitButton_;
public:
	afx_msg void OnBnClickedOk();
};
