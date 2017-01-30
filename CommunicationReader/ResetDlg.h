
#pragma once
#include "stdafx.h"

// ResetDlg 对话框

class ResetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResetDlg)

public:
	ResetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ResetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_RESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReset();


	CWinThread *RunFirstBackThread;
	bool StartFirstBackThread();
	static UINT FirstBackThread(LPVOID lParam);
	afx_msg void OnClose();
};
