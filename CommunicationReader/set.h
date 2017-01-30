#pragma once
#include "afxwin.h"


// set 对话框

class set : public CDialogEx
{
	DECLARE_DYNAMIC(set)

public:
	set(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~set();

// 对话框数据
	enum { IDD = IDD_SET };
	enum {
		EXCELMONITOR = 1, EXCELMONITOR1 = 2, GTMONITOR = 3, IOSCANNER = 4,//GT感应器
		DATAMONITOR = 5,
		EXCELMONITOR_TIMEOUT = 20, EXCELMONITOR1_TIMEOUT = 2000
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose1();
	afx_msg void OnSelchangeComboselcom1();
	afx_msg void OnSelchangeComboselbit1();
	afx_msg void OnSelchangeComboseldatalen1();
	afx_msg void OnSelchangeComboselstop1();
	afx_msg void OnSelchangeComboselpar1();
//	afx_msg void OnSelchangeComboselcom2();
//	afx_msg void OnSelchangeComboselbit2();
//	afx_msg void OnSelchangeComboseldatalen2();
//	afx_msg void OnSelchangeComboselpar2();
//	afx_msg void OnSelchangeComboselstop2();
//	afx_msg void OnBnClickedButtonClose2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();

private:
	void GetDlgData();
	void reFreshData();
	void InitData();
	int GetNumFromComStr(CString str);
public:
	afx_msg void OnBnClickedButtonSelmodel();
	CString m_SelCom1;
//	CString m_SelCom2;
	CString m_SelBit1;
//	CString m_SelBit2;
	CString m_SelDataLen1;
//	CString m_SelDataLen2;
	CString m_SelStop1;
//	CString m_SelStop2;
	CString m_SelPar1;
//	CString m_SelPar2;
	CComboBox m_cSelCom1;
//	CComboBox m_cSelCom2;
	CComboBox m_cSelBit1;
//	CComboBox m_cSelBit2;
	CComboBox m_cSelDataLen1;
//	CComboBox m_cSelDataLen2;
	CComboBox m_cSelStop1;
//	CComboBox m_cSelStop2;
	CComboBox m_cSelPar1;
//	CComboBox m_cSelPar2;
	afx_msg void OnBnClickedButtonSend1();
//	afx_msg void OnBnClickedButtonSend2();
	LRESULT OnCommunication(WPARAM ch, LPARAM port);
//	LRESULT OnCommunication1(WPARAM ch, LPARAM port);
	CString m_rev1;
//	CString m_rev2;
	CString m_send1;
//	CString m_send2;
	CString m_s1;
//	CString m_s2;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int TimeLimit;
	afx_msg void OnEnChangeEditRev1();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	int m_row;
	int m_col;
	int m_row1;
	int m_col1;
	CString model_path;
	CEdit m_recv1;
	CEdit m_recv2;



private:
	CComboBox m_spinwise;
	int m_speed;
	float m_acc;
	int m_autopuls;
	int m_backspeed;
	int m_manpuls;

public:
	afx_msg void OnBnClickedButtonBack();
	int m_compensate;
	afx_msg void OnSelchangeComboseWise();
	afx_msg void OnBnClickedButtonComf();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
