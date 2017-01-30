
// CommunicationReaderDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MsgWrite.h"
#include "resource.h"

// CCommunicationReaderDlg �Ի���
class CCommunicationReaderDlg : public CDialogEx
{
// ����
public:
	CCommunicationReaderDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CCommunicationReaderDlg();// { xls.End(); }

// �Ի�������
	enum { IDD = IDD_COMMUNICATIONREADER_DIALOG };
	enum {
		EXCELMONITOR = 1, EXCELMONITOR1 = 2, GTMONITOR = 3, IOSCANNER = 4,//GT��Ӧ��
		DATAMONITOR = 5,
		EXCELMONITOR_TIMEOUT = 20, EXCELMONITOR1_TIMEOUT = 2000
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CRITICAL_SECTION	m_csCommunicationSync;		  //�������ٽ���,��
	CRITICAL_SECTION	m_csCommunicationSync1;		  //�������ٽ���

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT  OnCommunication(WPARAM ch, LPARAM port);
	afx_msg LRESULT  OnCommunication1(WPARAM ch, LPARAM port);
	//when getting data from gt and show string on the screen
	void showStr();
	char *msg;
	double GetMinValue(vector<double>& value_check);
	double GetMaxValue(vector<double>& value_check);
	void startRecord();
	//����������������������˰��²�������������˶���Ȼ���ȡ���ݣ�Ȼ�������ֵ�Ĺ���
	int Procedure();
	//���Ҫ��д���źŽ������������Լ�¼�������źź����һ���̣߳�
	vector<CString> Split(CString string);



	CWinThread *tProcedureMonitor;
	bool BeginProcedureMonitor();
	static UINT ProcedureMonitor(LPVOID param);

	//��ΪҪ������λ�������ԡ������忨��ʼ����ʱ�򲻶���λ����set

	void decodeMsg(char *msg);
	void ClearData();
	void ClearData1();
	void SetValue();
	void SetValue(int sheetNo);
	void IO_scanner();
	void showError();
	void showClear();

	CString m_camerares1;//��edit������ʾ�ַ�
	CString m_camerares2;
//	CString m_camerares3;
//	CString m_camerares4;
	CEdit m_cameraedit1;//�����ַ�������
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
	//�洢��ֵ��Ϊ�˱Ƚϣ��ֱ������ֵ�����ޣ�����
	vector<double> compvalue1;
	vector<double> adjustValue1;//for adjust
	vector<double> compvalue2;
	vector<double> compvalue3;
	vector<double> compvalue4;

public:
	//��־����������ָʾ�ַ����Ƿ�������
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

