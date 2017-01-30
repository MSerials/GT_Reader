
// CommunicationReaderDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MsgWrite.h"
#include "resource.h"

// CCommunicationReaderDlg 对话框
class CCommunicationReaderDlg : public CDialogEx
{
// 构造
public:
	CCommunicationReaderDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CCommunicationReaderDlg();// { xls.End(); }

// 对话框数据
	enum { IDD = IDD_COMMUNICATIONREADER_DIALOG };
	enum {
		EXCELMONITOR = 1, EXCELMONITOR1 = 2, GTMONITOR = 3, IOSCANNER = 4,//GT感应器
		DATAMONITOR = 5,
		EXCELMONITOR_TIMEOUT = 20, EXCELMONITOR1_TIMEOUT = 2000
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCamera1();
	afx_msg void OnBnClickedCamera2();
	afx_msg void OnBnClickedCamera3();
	afx_msg void OnBnClickedCamera4();


	void InitData();
	void InitCom();
	void InitExcel();
	void CriticalExam();
	char *m_msg;
	BOOL isRecord;
	BOOL isRecord1;
	int m_msgIndex;
	DWORD TickCount;
	DWORD TickCount1;
	CRITICAL_SECTION	m_csCommunicationSync;		  //定义了临界区,给
	CRITICAL_SECTION	m_csCommunicationSync1;		  //定义了临界区

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT  OnCommunication(WPARAM ch, LPARAM port);
	afx_msg LRESULT  OnCommunication1(WPARAM ch, LPARAM port);
	//when getting data from gt and show string on the screen
	void showStr();
	char *msg;
	double GetMinValue(vector<double>& value_check);
	double GetMaxValue(vector<double>& value_check);
	void startRecord();
	//工序函数，这个函数是描述了按下测量键后马达下运动，然后读取数据，然后测量数值的过程
	int Procedure();
	//马达要和写入信号进行阻塞，所以记录发出的信号和马达一个线程？
	vector<CString> Split(CString string);



	CWinThread *tProcedureMonitor;
	bool BeginProcedureMonitor();
	static UINT ProcedureMonitor(LPVOID param);

	//因为要进行限位处理，所以。。。板卡初始化的时候不对限位进行set

	void decodeMsg(char *msg);
	void ClearData();
	void ClearData1();
	void SetValue();
	void SetValue(int sheetNo);
	void IO_scanner();
	void showError();
	void showClear();

	CString m_camerares1;//在edit框里显示字符
	CString m_camerares2;
//	CString m_camerares3;
//	CString m_camerares4;
	CEdit m_cameraedit1;//控制字符下拉的
	CEdit m_cameraedit2;
//	CEdit m_cameraedit3;
//	CEdit m_cameraedit4;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	CListCtrl m_list1;
	int list_col;
//	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
//	CListCtrl m_list2;
	afx_msg void OnEnChangeEdit2();
//	CListCtrl m_list3;
//	CListCtrl m_list4;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_SubItemEdit;
	CString cstrItemTextEdit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetListItemText(void);
	int listSelFlag[3];



	afx_msg void OnKillfocusEditSub();

	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
#if 0
	afx_msg void OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkList2(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDblclkList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkList3(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDblclkList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkList4(NMHDR *pNMHDR, LRESULT *pResult);
#endif
	//存储了值，为了比较，分别是设计值，上限，下限
	vector<double> compvalue1;
	vector<double> adjustValue1;//for adjust
	vector<double> compvalue2;
	vector<double> compvalue3;
	vector<double> compvalue4;

public:
	//标志变量，用于指示字符串是否接受完毕
	bool rec_flags;//////
	bool rec_flags1;//////
	void EnableCamera(int flags);//////
	CFont font; //////
	afx_msg void OnSize(UINT nType, int cx, int cy);//////
	CRect m_rect;
	int m_font_height;
	int m_font_width;
	//------------------------
	void ReSize();
	POINT old;
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnBnClickedStartDetect();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedButtonResetdlg();
	afx_msg void OnBnClickedStartTozero();

	void showOK();
	void showNG();

	CWinThread *tAdjustZero;
	bool BeginAdjustZeroMonitor();
	static UINT AdjustZeroMonitor(LPVOID param);

	void copy_data(char** dst);
	void ButtonDisable();
	void ButtonEnable();
	//afx_msg void OnBnClickedButtonUp();
	//afx_msg void OnBnClickedButtonDown();
	//afx_msg void OnSetfocusButtonUp();

	afx_msg void OnBnClickedButtonDowntoceliang();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnClickedGetthreshold();
};

