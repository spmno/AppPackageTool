// AppPackageDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DecompressManager.h"
// CAppPackageDlg �Ի���
class CAppPackageDlg : public CDialog, private DecompressDelegate
{
// ����
public:
	CAppPackageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	virtual void decompressError(DECOMPRESS_ERROR errorCode);
	virtual void updateZipInfo(DWORD zipFileCount);
	virtual void updateStep(const TCHAR* fileName);
	virtual void decompressFinish();
	enum { IDD = IDD_APPPACKAGE_DIALOG };

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
