// AppPackageDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DecompressManager.h"
// CAppPackageDlg 对话框
class CAppPackageDlg : public CDialog, private DecompressDelegate
{
// 构造
public:
	CAppPackageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	virtual void decompressError(DECOMPRESS_ERROR errorCode);
	virtual void updateZipInfo(DWORD zipFileCount);
	virtual void updateStep(const TCHAR* fileName);
	virtual void decompressFinish();
	enum { IDD = IDD_APPPACKAGE_DIALOG };

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
private:
	DecompressManager decompressManager_;
	CProgressCtrl progressBar_;
	CStatic displayInfo_;
};
