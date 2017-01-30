
#pragma once
#include "stdafx.h"

// ResetDlg �Ի���

class ResetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResetDlg)

public:
	ResetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ResetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReset();


	CWinThread *RunFirstBackThread;
	bool StartFirstBackThread();
	static UINT FirstBackThread(LPVOID lParam);
	afx_msg void OnClose();
};
