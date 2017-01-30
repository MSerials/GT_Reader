
// CommunicationReaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommunicationReader.h"
#include "CommunicationReaderDlg.h"
#include "afxdialogex.h"
#include "set.h"
#include "StringDeal.h"
#include "MsgWrite.h"
#include "ResetDlg.h"

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCommunicationReaderDlg 对话框

CCommunicationReaderDlg::CCommunicationReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommunicationReaderDlg::IDD, pParent)
	, m_camerares1(_T(""))
	, m_camerares2(_T(""))
//	, m_camerares3(_T(""))
//	, m_camerares4(_T(""))
	, cstrItemTextEdit(_T(""))
	, tAdjustZero(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	msg = (char*)malloc( BUFFSIZE);
}

CCommunicationReaderDlg::~CCommunicationReaderDlg()// 
{
	xls.End();
	g.act.CloseCard();
}

void CCommunicationReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_camerares1);
	DDX_Text(pDX, IDC_EDIT4, m_camerares2);
//	DDX_Text(pDX, IDC_EDIT2, m_camerares3);
//	DDX_Text(pDX, IDC_EDIT3, m_camerares4);
	DDX_Control(pDX, IDC_EDIT1, m_cameraedit1);
	DDX_Control(pDX, IDC_EDIT4, m_cameraedit2);
//	DDX_Control(pDX, IDC_EDIT2, m_cameraedit3);
//	DDX_Control(pDX, IDC_EDIT3, m_cameraedit4);
	DDX_Control(pDX, IDC_LIST1, m_list1);
//	DDX_Control(pDX, IDC_LIST2, m_list2);
//	DDX_Control(pDX, IDC_LIST3, m_list3);
//	DDX_Control(pDX, IDC_LIST4, m_list4);
	DDX_Control(pDX, IDC_EDIT_SUB, m_SubItemEdit);
	DDX_Text(pDX, IDC_EDIT_SUB, cstrItemTextEdit);
}

BEGIN_MESSAGE_MAP(CCommunicationReaderDlg, CDialogEx)
	ON_MESSAGE(WM_COMM_RXCHAR, &CCommunicationReaderDlg::OnCommunication)
	ON_MESSAGE(WM_COMM_RXCHAR1, &CCommunicationReaderDlg::OnCommunication1)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SAVE, &CCommunicationReaderDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CAMERA1, &CCommunicationReaderDlg::OnBnClickedCamera1)
	ON_BN_CLICKED(IDC_CAMERA2, &CCommunicationReaderDlg::OnBnClickedCamera2)
	ON_BN_CLICKED(IDC_CAMERA3, &CCommunicationReaderDlg::OnBnClickedCamera3)
	ON_BN_CLICKED(IDC_CAMERA4, &CCommunicationReaderDlg::OnBnClickedCamera4)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CCommunicationReaderDlg::OnBnClickedButton1)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCommunicationReaderDlg::OnLvnItemchangedList)
	ON_EN_CHANGE(IDC_EDIT2, &CCommunicationReaderDlg::OnEnChangeEdit2)
//	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CCommunicationReaderDlg::OnClickList1)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCommunicationReaderDlg::OnLvnItemchangedList1)
	ON_EN_KILLFOCUS(IDC_EDIT_SUB, &CCommunicationReaderDlg::OnKillfocusEditSub)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCommunicationReaderDlg::OnDblclkList1)/*左键双击list空白添加*/
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST1, &CCommunicationReaderDlg::OnRdblclkList1)/*右键双击list空白删除*/
#if 0
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CCommunicationReaderDlg::OnDblclkList2)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST2, &CCommunicationReaderDlg::OnRdblclkList2)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &CCommunicationReaderDlg::OnDblclkList3)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST3, &CCommunicationReaderDlg::OnRdblclkList3)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST4, &CCommunicationReaderDlg::OnDblclkList4)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST4, &CCommunicationReaderDlg::OnRdblclkList4)
#endif
	ON_WM_SIZE()

	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
	ON_BN_CLICKED(IDC_START_DETECT, &CCommunicationReaderDlg::OnBnClickedStartDetect)
	ON_EN_CHANGE(IDC_EDIT4, &CCommunicationReaderDlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON_RESETDLG, &CCommunicationReaderDlg::OnBnClickedButtonResetdlg)
	ON_BN_CLICKED(IDC_START_TOZERO, &CCommunicationReaderDlg::OnBnClickedStartTozero)
	ON_BN_CLICKED(IDC_BUTTON_DOWNTOCELIANG, &CCommunicationReaderDlg::OnBnClickedButtonDowntoceliang)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CCommunicationReaderDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC__GETTHRESHOLD, &CCommunicationReaderDlg::OnClickedGetthreshold)
END_MESSAGE_MAP()





vector<CString> CCommunicationReaderDlg::Split(CString string)
{
	CString oriStr = string;
	vector<CString> strVec;
	while (true)
	{
		CString n = oriStr.SpanExcluding(L",");//获取.之前的字符串  
		strVec.push_back(n);
		oriStr = oriStr.Right(oriStr.GetLength() - n.GetLength() - 1);//从右边1开始获取从右向左前 nCount 个字符  
		if (oriStr.IsEmpty())
		{
			break;
		}
	}
	return strVec;
}


// CCommunicationReaderDlg 消息处理程序

BOOL CCommunicationReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	TickCount1 = TickCount = GetTickCount();

	// 将“关于...”菜单项添加到系统菜单中。、

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
#ifdef MYDEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
	AfxGetMainWnd()->SetWindowText(L"零件视觉检测系统-浙江爱易特智能技术有限公司");

	InitData();
	//非常重要，该地方检查了相关参数，如果丢失配置文件，不进行初始化文件的话，可能造成电机速度过快
	CriticalExam();


	InitCom();

	InitExcel();

	BeginProcedureMonitor();
	//init ui
	m_SubItemEdit.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	CRect rect;
	m_list1.GetClientRect(&rect);
	CStringDeal Cd;
	//vector<CString> str;
	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_list1.InsertColumn(0, _T("设计值"), LVCFMT_LEFT, rect.Width() / 3, 0);
	m_list1.InsertColumn(1, _T("上限"), LVCFMT_LEFT, rect.Width() / 3, 1);
	m_list1.InsertColumn(2, _T("下限"), LVCFMT_LEFT, rect.Width() / 3, 2);
	list_col = 3;
	//Cd.cutString((LPSTR)(LPCSTR)(CStringA)ini.CompareValue1, str);
	vector<CString> str = Cd.Split(ini.CompareValue1);
	compvalue1.clear();
	for (size_t i = 0;i < str.size();compvalue1.push_back(atof((LPSTR)(LPCSTR)(CStringA)str[i])), i++);//初始化的是把ini文件中 设置值，上下限提取出来
	int maxrow = str.size() / 3;
	for (size_t i = 0; i < maxrow; i++)
	{
		m_list1.InsertItem(i, str[3 * i]);
		m_list1.SetItemText(i, 1, str[3 * i + 1]);
		m_list1.SetItemText(i, 2, str[3 * i + 2]);
	}

	//调用矫正值
	str.clear();
	str = Cd.Split(ini.adjustValue1);
	//Cd.cutString((LPSTR)(LPCSTR)(CStringA)ini.adjustValue1, str);
	for (size_t i = 0; i < str.size(); adjustValue1.push_back(atof((LPSTR)(LPCSTR)(CStringA)str[i])), i++);//初始化的是把ini文件中 设置值，上下限提取出来
	
	CString tmpstr;
	tmpstr.Format(_T("%f"),ini.m_value_threshold);
	GetDlgItem(IDC_EDIT_THRES)->SetWindowText(tmpstr);
	showOK();

	//m_list2.GetClientRect(&rect);
	//m_list2.SetExtendedStyle(m_list2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_list2.InsertColumn(0, _T("设计值"), LVCFMT_LEFT, rect.Width() / 3, 0);
	//m_list2.InsertColumn(1, _T("上限"), LVCFMT_LEFT, rect.Width() / 3, 1);
	//m_list2.InsertColumn(2, _T("下限"), LVCFMT_LEFT, rect.Width() / 3, 2);
	//Cd.cutString((LPSTR)(LPCSTR)(CStringA)ini.CompareValue2, str);
	//for (int i = 0;i < str.size();compvalue2.push_back(atof((LPSTR)(LPCSTR)(CStringA)str[i])), i++);//初始化的是把ini文件中 设置值，上下限提取出来
	// maxrow = str.size() / 3;
	//for (int i = 0;i < maxrow;i++)
	//{
	//	m_list2.InsertItem(i, str[3 * i]);
	//	m_list2.SetItemText(i, 1, str[3 * i + 1]);
	//	m_list2.SetItemText(i, 2, str[3 * i + 2]);
	//}

	//m_list3.GetClientRect(&rect);
	//m_list3.SetExtendedStyle(m_list3.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_list3.InsertColumn(0, _T("设计值"), LVCFMT_LEFT, rect.Width() / 3, 0);
	//m_list3.InsertColumn(1, _T("上限"), LVCFMT_LEFT, rect.Width() / 3, 1);
	//m_list3.InsertColumn(2, _T("下限"), LVCFMT_LEFT, rect.Width() / 3, 2);
	//Cd.cutString((LPSTR)(LPCSTR)(CStringA)ini.CompareValue3, str);
	//for (int i = 0;i < str.size();compvalue3.push_back(atof((LPSTR)(LPCSTR)(CStringA)str[i])), i++);//初始化的是把ini文件中 设置值，上下限提取出来
	// maxrow = str.size() / 3;
	//for (int i = 0;i < maxrow;i++)
	//{
	//	m_list3.InsertItem(i, str[3 * i]);
	//	m_list3.SetItemText(i, 1, str[3 * i + 1]);
	//	m_list3.SetItemText(i, 2, str[3 * i + 2]);
	//}

	//m_list4.GetClientRect(&rect);
	//m_list4.SetExtendedStyle(m_list4.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_list4.InsertColumn(0, _T("设计值"), LVCFMT_LEFT, rect.Width() / 3, 0);
	//m_list4.InsertColumn(1, _T("上限"), LVCFMT_LEFT, rect.Width() / 3, 1);
	//m_list4.InsertColumn(2, _T("下限"), LVCFMT_LEFT, rect.Width() / 3, 2);
	//Cd.cutString((LPSTR)(LPCSTR)(CStringA)ini.CompareValue4, str);
	//for (int i = 0;i < str.size();compvalue4.push_back(atof((LPSTR)(LPCSTR)(CStringA)str[i])), i++);//初始化的是把ini文件中 设置值，上下限提取出来
	// maxrow = str.size() / 3;
	//for (int i = 0;i < maxrow;i++)
	//{
	//	m_list4.InsertItem(i, str[3 * i]);
	//	m_list4.SetItemText(i, 1, str[3 * i + 1]);
	//	m_list4.SetItemText(i, 2, str[3 * i + 2]);
	//}

	rec_flags = false;
	rec_flags1 = false;

	EnableCamera(TRUE);

	//////

	m_font_height = 15;
	m_font_width = 11;

	font.CreateFont(m_font_height, m_font_width, 0, 0, FW_THIN, false, false, false,
		CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
		FF_MODERN, L"宋体");

	/*设置列表不可见，需要的时候启用*/
	m_list1.ShowWindow(TRUE);
//	m_list2.ShowWindow(FALSE);
//	m_list3.ShowWindow(FALSE);
//	m_list4.ShowWindow(FALSE);


//	CRect rect;
	GetClientRect(&rect);   //取客户区大小    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

//	ShowWindow(SW_MAXIMIZE);//全屏显示

	SetTimer(EXCELMONITOR, EXCELMONITOR_TIMEOUT, NULL);//////
	//SetTimer(EXCELMONITOR1, EXCELMONITOR1_TIMEOUT, NULL);//////
	SetTimer(GTMONITOR,200, NULL);
	SetTimer(IOSCANNER, 20, NULL);
	g.act.InitCard();
	//if (0 != g.act.InitCard()){ exit(0); }
	//write_output(FIRST_SRV,ON);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CCommunicationReaderDlg::CriticalExam()
{


}

void CCommunicationReaderDlg::InitExcel()
{
	xls.Init();
	g.row = xls.LastLineIndex();
#ifdef MYDEBUG
	cout<<"last line is ";
	cout<<g.row<<endl;
#endif
}

void CCommunicationReaderDlg::InitData()
{

	isRecord = FALSE;
	m_msg = (char*)malloc(BUFFSIZE + 1); //4M缓存空间
	for (int i = 0; i <= BUFFSIZE; i++) m_msg[i] = '\0';
	m_msgIndex = 0;

	CString AppPath;
	::GetModuleFileName(GetModuleHandle(NULL), AppPath.GetBuffer(300), 300);
	AppPath.ReleaseBuffer();
	AppPath = AppPath.Left(AppPath.ReverseFind('\\'));
	AppPath = AppPath + L"\\" + L"Configuration";
	ini.SetIniDir(AppPath, false);
	if (0 == ini.SetIniFile(L"PrgParameter.ini"))
	{
		ini.SaveParaFile(PARA_ALL);
	}
	ini.LoadParaFile(PARA_ALL);
	g.RunDisable = NOT_ALREADY;
}

void CCommunicationReaderDlg::InitCom()
{
	Port1.EnableSerialMessage(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
	Port2.EnableSerialMessage(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
	Sleep(10);
	CString str,str1;
	int portnum = ini.portnum;
	int portnum1 = ini.portnum1;
	str.Format(_T("COM%d"), portnum);
	str1.Format(_T("COM%d"), portnum1);
	
	if (!Port1.InitPort(this, (LPSTR)(LPCSTR)(CStringA)str, (UINT)ini.baud, (LPSTR)(LPCSTR)(CStringA)ini.parity, (UINT)ini.databits, (LPSTR)(LPCSTR)(CStringA)ini.stopbits, EV_RXCHAR, 512, 512))
	{
		g.com_state1 = false;
#ifdef MYDEBUG
		printf("COM1 Init Failed!\n");
#endif
	}
	else
	{
		g.com_state1 = true;
		Port1.StartMonitoring();
#ifdef TEST
		cout << "start com 1" << endl;
#endif
	}
	if (!Port2.InitPort(this, (LPSTR)(LPCSTR)(CStringA)str1, (UINT)ini.baud1, (LPSTR)(LPCSTR)(CStringA)ini.parity1, (UINT)ini.databits1, (LPSTR)(LPCSTR)(CStringA)ini.stopbits1, EV_RXCHAR, 512, 512))
	{
		g.com_state2 = false;
#ifdef MYDEBUG
		printf("COM1 Init Failed!\n");
#endif
	}
	else
	{
		g.com_state2 = true;
		Port2.StartMonitoring();
#ifdef TEST
		cout << "start com 2" << endl;
#endif
	}

}

void CCommunicationReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCommunicationReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect   rect;
		CPaintDC   dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect, RGB(32, 32, 35));
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCommunicationReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCommunicationReaderDlg::OnBnClickedSave()
{
	// TODO:  在此添加控件通知处理程序代码
	set s;
	s.DoModal();
}


void CCommunicationReaderDlg::OnBnClickedCamera1()
{
	// TODO:  在此添加控件通知处理程序代码
	EnableCamera(FALSE);

	if (!g.com_state1)
	{
		m_camerares1 = L"没打第一个串口，请在设置里打开！";
		UpdateData(FALSE);
		return;
	}
	char *s = "T1\r\n";
	ClearData();
	g.msgIndex = 0;
	Port1.WriteToPort(s, strlen(s));

	//SetEvent(g.hCamHand1);
	//::WaitForSingleObject(g.hCamHand1, INFINITE);
	//SetValue(1);//向表格1里写数据
}
void CCommunicationReaderDlg::OnBnClickedCamera2()
{
	// TODO:  在此添加控件通知处理程序代码
	EnableCamera(FALSE);

	if (!g.com_state1)
	{
		m_camerares2 = L"没打第一个串口，请在设置里打开！";
			UpdateData(FALSE);
		return;
	}
	char *s = "T2\r\n";
	ClearData();
	g.msgIndex = 0;

	Port1.WriteToPort(s, strlen(s));

	//SetEvent(g.hCamHand2);
	//::WaitForSingleObject(g.hCamHand2, INFINITE);
	//SetValue(2);//向表格2里写数据
}


void CCommunicationReaderDlg::OnBnClickedCamera3()
{
	// TODO:  在此添加控件通知处理程序代码
	EnableCamera(FALSE);

	if (!g.com_state2)
	{
//		m_camerares3 = L"没打第二个串口，请在设置里打开！";
		UpdateData(FALSE);
		return;
	}
	//记录过程
	char *s = "T1\r\n";
	ClearData1();
	g.msgIndex1 = 0;
	Port2.WriteToPort(s, strlen(s));
}


void CCommunicationReaderDlg::OnBnClickedCamera4()
{
	// TODO:  在此添加控件通知处理程序代码
	EnableCamera(FALSE);

	if (!g.com_state2)
	{
//		m_camerares4 = L"没打第二个串口，请在设置里打开！";
		UpdateData(FALSE);
		return;
	}

	char *s = "T2\r\n";
	ClearData1();
	g.msgIndex1 = 0;

	Port2.WriteToPort(s, strlen(s));
}


void CCommunicationReaderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
#ifdef _GX //基恩士系列的图像系统
	ExpData expdata;
	CString rescstr;

	if (EXCELMONITOR == nIDEvent)
	{
		if (rec_flags)	//
		{
			rec_flags = false;
			expdata = MsgStoreInFile(g.msg);
			rescstr = DealvCstr(expdata.vCStr); //合成分割后的消息字符串，用于显示在屏幕上

			if (!expdata.flags) //零件有问题
			{
				if (expdata.Camera == COM1T1)
				{
					GetDlgItem(IDC_EDIT1)->SetFont(&font);	
					GetDlgItem(IDC_EDIT1)->SetWindowText(L"Camera1 - NG\r\n" + rescstr);
				}
				else if (expdata.Camera == COM1T2)
				{
					GetDlgItem(IDC_EDIT4)->SetFont(&font);
					GetDlgItem(IDC_EDIT4)->SetWindowText(L"Camera2 - NG\r\n" + rescstr);
				}
			}
			else  //零件正常
			{

				if (expdata.Camera == COM1T1)
				{
					GetDlgItem(IDC_EDIT1)->SetFont(&font);
					GetDlgItem(IDC_EDIT1)->SetWindowText(L"Camera1 - OK\r\n" + rescstr);
				}
				else if (expdata.Camera == COM1T2)
				{
					GetDlgItem(IDC_EDIT4)->SetFont(&font);
					GetDlgItem(IDC_EDIT4)->SetWindowText(L"Camera2 - OK\r\n" + rescstr);
				}
			}

			EnableCamera(TRUE);

		}
		if (rec_flags1)
		{
			rec_flags1 = false;
			expdata = MsgStoreInFile(g.msg1);
			rescstr = DealvCstr(expdata.vCStr); //合成分割后的消息字符串，用于显示在屏幕上

			if (!expdata.flags) //零件有问题
			{
				if (expdata.Camera == COM2T1)
				{
					GetDlgItem(IDC_EDIT2)->SetFont(&font);
					GetDlgItem(IDC_EDIT2)->SetWindowText(L"Camera3 - NG\r\n" + rescstr);
				}
				else if (expdata.Camera == COM2T2)
				{
					GetDlgItem(IDC_EDIT3)->SetFont(&font);
					GetDlgItem(IDC_EDIT3)->SetWindowText(L"Camera4 - NG\r\n" + rescstr);
				}
			}
			else  //零件正常
			{
				if (expdata.Camera == COM2T1)
				{
					GetDlgItem(IDC_EDIT2)->SetFont(&font);
					GetDlgItem(IDC_EDIT2)->SetWindowText(L"Camera3 - OK\r\n" + rescstr);
				}
				else if (expdata.Camera == COM2T2)
				{
					GetDlgItem(IDC_EDIT3)->SetFont(&font);
					GetDlgItem(IDC_EDIT3)->SetWindowText(L"Camera4 - OK\r\n" + rescstr);
				}
			}

			EnableCamera(TRUE);

		}
	}

	if (EXCELMONITOR1 == nIDEvent)
	{
		EnableCamera(TRUE); //避免无数据从串口返回或数据返回出错后按钮一直处于禁用状态
	}
#endif

#ifdef _GT //GT系列感应器
	//每200ms向感应器发送MS加回车键字符，于是可以搜到信号
	//我得确保上一次的信号收集完毕才能进行下一次信号的收集
	if (GTMONITOR == nIDEvent)
	{
		std::lock_guard<std::mutex> lck(g.mtx);
		char *s = "MS\r\n";
		ClearData();
		g.msgIndex = 0;
		if(g.com_state1)Port1.WriteToPort(s, strlen(s)); //debug

		CString str;
		str.Format(_T("当前电机脉冲位置:%d"), d1000_get_command_pos(FIRST_MOTOR));
		GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	}

	if (IOSCANNER == nIDEvent)
	{
		//监IO口用
		IO_scanner();
	}

#endif

	CDialogEx::OnTimer(nIDEvent);
}

void CCommunicationReaderDlg::IO_scanner()
{
	CString Tip;
	//下降检测的上升沿信号
	g.In_New_bits = read_input_state(IN_START_DECT);
	if (g.In_New_bits && (g.In_New_bits != g.In_Old_bits))
	{	
		if (!g.RunDisable && read_input_state(IN_HAVE_GUIDE_BOARD))		//因为下降之前，肯定有导板了，所以只需要上升沿去触发
			g.evtMotorProcedure.SetEvent();//由于是阻塞的所以无法同时两次出发，
		else
		{
			Tip += L"伺服报警中或没启动或者没有导板\r\n";
			//GetDlgItem(IDC_EDIT1)->SetWindowText(L"伺服报警中或没启动或者没有导板");
			UpdateData(FALSE);
		}
	}
	g.In_Old_bits = g.In_New_bits;
	/*
	//急停按钮的上升沿信号
	g.In_New_bits_emergency = !read_input_state(IN_EMGENCY);
	if (g.In_New_bits_emergency && (g.In_New_bits_emergency != g.In_Old_bits_emergency))
	{

	}
	g.In_Old_bits_emergency = g.In_New_bits_emergency;
	*/
	if (!read_input_state(IN_EMGENCY))
	{
		g.evtEmgencyStop.SetEvent();
		g.RunDisable |= NOT_ALREADY;
		Tip += L"急停按钮后请复位\r\n";
		//GetDlgItem(IDC_EDIT1)->SetWindowText(L"急停按钮后请复位");
	}

	//读取伺服电机报警的上升沿信号
	if (!read_input_state(IN_MOTOR_ALM))
	{
		write_output(OUT_ALM, ON);
		g.RunDisable |= MOTOR_ALM;
		g.RunDisable |= NOT_ALREADY;
		Tip += L"伺服电机报警\r\n";
		//GetDlgItem(IDC_EDIT1)->SetWindowText(L"急停按钮后请复位");
	//	g.act.FirstSrvAlready = 0;
	}
	
	if (g.RunDisable)
	{
		Tip += L"机器不在工作中，请勿开启其他机器";
//		showError();
	}
//	else
//	{
		//showClear();
//	}

	if (!Tip.IsEmpty()) GetDlgItem(IDC_EDIT1)->SetWindowText(Tip);
}

void CCommunicationReaderDlg::SetValue(int sheetNo)
{
	CStringDeal Cd;
	switch (sheetNo) {
	case 1:
		xls.SelectSheet(1);

		break;
	case 2:
		xls.SelectSheet(2);

		break;
	case 3:

		xls.SelectSheet(3);
		break;
	case 4:

		xls.SelectSheet(4);
		break;
	default:break;
	}
}
void CCommunicationReaderDlg::EnableCamera(int flags)
{
//	GetDlgItem(IDC_CAMERA1)->EnableWindow(flags);
//	GetDlgItem(IDC_CAMERA2)->EnableWindow(flags);
//	GetDlgItem(IDC_CAMERA3)->EnableWindow(flags);
//	GetDlgItem(IDC_CAMERA4)->EnableWindow(flags);
}
LRESULT CCommunicationReaderDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	try{
		static int count = 0;
		//g.mtx.lock();
		//std::lock_guard<std::mutex> lck(g.mtx, std::adopt_lock);
		//加锁，防止接收数据的时候数据被篡改
		//函数执行完毕就解锁
		std::lock_guard<std::mutex> lck(g.mtx);
		//g.isReciveOver = false;
		if (g.msgIndex >= BUFFSIZE)
			g.msgIndex = 0; //缓存区满以后从头覆盖
		g.msg[g.msgIndex] = ch;
		++g.msgIndex;

#ifdef _GX
		if (ch == '\r')//////
		{
			++count;
			if (count >= 2) //由发送Tx返回的数据正常返回会有两个回车符
			{
				count = 0;
				g.msg[g.msgIndex] = '\0';  

				g.msgIndex = 0;

				MsgIndex_Offset = 5; //数据头是"T1\rT1"

				rec_flags = true; //数据接收完毕
			}
			else if(count == 1 && g.msg[g.msgIndex - 2] == 'E') //返回的数据是由硬件触发
			{
				count = 0;
				g.msg[g.msgIndex] = '\0';

				g.msgIndex = 0;

				MsgIndex_Offset = 2; //数据头是"T1"

				rec_flags = true; //数据接收完毕
			}
		}
#endif
		if ('\r' == ch)
		{
			g.msg[g.msgIndex - 1] = '\0';
			g.isReciveOver = true;

			//已经收到结束符，可以开始执行记录
#ifdef MYDEBUG
			//cout << g.msg << endl;
#endif
			memcpy(g.msg1, g.msg, g.msgIndex);
			showStr();
			
			//if (g.isRecord)
			//{
			//	g.isRecord = false;
			//	startRecord();	
			//}
		}
#ifdef _GT

#endif


#ifdef MYDEBUG
		//cout << (int)ch << endl;
#endif
	}
	catch (...)
	{
		GetDlgItem(IDC_EDIT1)->SetWindowText(L"未知错误");
	}

	return 0;
}

void CCommunicationReaderDlg::showStr()
{
	
	CStringDeal Cd;
	vector<CString> str;
	vector<double> value_check;
	CString str1;
	Cd.cutString(g.msg, str);
	size_t sensor_idx = 0;//
	size_t sensor_start = 2;
	int i = 0;
	for (; (sensor_idx + sensor_start) <= str.size(); sensor_idx += 2,i++)
	{//add here
		CString s = str[sensor_idx + sensor_start];
		double value = atof((LPCSTR)(CStringA)(s));
		value -= (adjustValue1[i]);
		value_check.push_back(value);
		s.Format(_T("第%d个感应器数值：%f\r\n"), 1 + i, value);
		str1 += s;
	}
	GetDlgItem(IDC_EDIT4)->SetWindowText(str1);
	//I wanna add my result record here
	if (WAIT_OBJECT_0 == WaitForSingleObject(g.evtStartRecord, 1))
	{
		CString tmp;
		xls.SelectSheet(1);
		bool _result = true;
		/**已经不用下面的代码了
		if ((compvalue1.size() / 3) != value_check.size())
		{
			AfxMessageBox(L"检测的参数和读取的数据数量不一致！");
		}

		vector<double> UpExamData;
		vector<double> DownExamData;
		vector<double> ExamData;
		for (size_t i = 0; i<(compvalue1.size() / 3); i++)
		{
			UpExamData.push_back(compvalue1[3 * i] + compvalue1[3 * i + 1]);
			DownExamData.push_back(compvalue1[3 * i] - compvalue1[3 * i + 2]);
		}
		*/
		tmp.Format(_T("%d"), g.row - 2);
		CString strRes;
		xls.SetCellValue(g.row, 1, tmp);
		xls.SetCellValue(g.row, 3, xls.GetTime());
		for (size_t i = 0; i < value_check.size(); i++)
		{
			tmp.Format(_T("%f"), value_check[i]);
			xls.SetCellValue(g.row, 4 + i * 2, tmp);
			CString tt;
			tt.Format(_T("第%d个感应器的数值为:%f \r\n "), 1+i, value_check[i]);
			strRes += tt;
			/*
			if (value_check[i] < DownExamData[i] || value_check[i]>UpExamData[i])
			{
				xls.SetCellValue(g.row, 5 + i * 2, L"NG");
				CString tt;
				tt.Format(_T("第%d个感应器的数值为:%f 不合格\r\n "), i, value_check[i]);
				strRes += tt;
				_result = false;
			}
			else
			{
				xls.SetCellValue(g.row, 5 + i * 2, L"OK");
				CString tt;
				tt.Format(_T("第%d个感应器的数值为:%f 合格\r\n "), i, value_check[i]);
				strRes += tt;
			}
			*/
		}
		double max = GetMaxValue(value_check);
		double min = GetMinValue(value_check);
		double mmm = max - min;
		if ((mmm) > ini.m_value_threshold)
			showNG();
		else
			showOK();
		CString tt;
		tt.Format(_T("最大值为:%f，最小值为:%f,差值为:%f,允许公差:%f \r\n "), max, min, mmm,ini.m_value_threshold);
		strRes += tt;
		g.row++;
		GetDlgItem(IDC_EDIT1)->SetWindowText(strRes);
		g.evtStartRecord.ResetEvent();
	}
}

double CCommunicationReaderDlg::GetMaxValue(vector<double>& value_check)
{
	if (0 == value_check.size()) return  -1.0;
	double max = value_check[0];
	for (size_t i = 1; i < value_check.size(); i++)
	{
		if (max < value_check[i])
			max = value_check[i];
	}
	return max;
}

double CCommunicationReaderDlg::GetMinValue(vector<double>& value_check)
{
	if (0 == value_check.size()) return  -1.0;
	double min = value_check[0];
	for (size_t i = 1; i < value_check.size(); i++)
	{
		if (min > value_check[i])
			min = value_check[i];
	}
	return min;
}


void CCommunicationReaderDlg::startRecord()
{
	bool flag = true;
	vector<CString> str;
	vector<CString> strdata;
	CStringDeal Cd;
	Cd.cutString(g.msg, str);
	if (str[0].Compare(L"MS"))
	{
		AfxMessageBox(L"没有找到 MS 标记头，是否感应器出现错误？");
	}

	const int max_sensor = (str.size() - 1) / 2;//MS,data1state,data1,data2state2,data2...
	size_t sensor_idx = 0;//
	size_t sensor_start = 2;
	for (; (sensor_idx + sensor_start) <= str.size(); sensor_idx += 2)
	{
		strdata.push_back(str[sensor_idx + sensor_start]);
	}
	//这个是存储了四个数据的
	//if((compvalue1.size()/3)!=strdata.size())
	//{
//		AfxMessageBox(L"检测的参数和读取的数据数量不一致！");
//	}

	vector<double> UpExamData;
	vector<double> DownExamData;
	vector<double> ExamData;
//	for (size_t i = 0; i<(compvalue1.size() / 3); i++)
//	{
//		UpExamData.push_back(compvalue1[3 * i] + compvalue1[3 * i + 1]);
//		DownExamData.push_back(compvalue1[3 * i] - compvalue1[3 * i + 2]);
//	}

	for (size_t i = 0; i < strdata.size(); i++)
	{
		double examdata = atof((LPSTR)(LPCSTR)(CStringA)strdata[i]);
		if (examdata < DownExamData[i] || examdata>UpExamData[i])
		{
			flag = false;
		}
	}

	if(flag) showOK();
	else showNG();

	xls.SelectSheet(1);


	return;
}





int CCommunicationReaderDlg::Procedure()
{
	//重要，我把CW()定义为下降，因为其他地方用上，所以要改CW里的函数，千万不要弄错
	if (!read_input_state(IN_MOTOR_ALM)) return MOTOR_ALM; //g.RunDisable
	if (NOT_ALREADY == (NOT_ALREADY&g.RunDisable)) return NOT_ALREADY;
	for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTOVER, OFF);
	for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTING, ON);
	g.act.FirstMotorStepRunCW();
	if (!g.act.WaitMotorTimeout(FIRST_MOTOR, 10000))	return MOTOR_TIME_OUT;
	Sleep(1500);//等待感应器稳定
	g.evtStartRecord.SetEvent();
   //表示可以开始记录数据，数据记录方式是从OnComm里开始的，因为这个接收了字符串
	Sleep(200);
	//g.act.FirstMotorStepRunCCW();
	int res = g.act.FisrtMotorBackToOrigin();
	res = g.act.FisrtMotorBackToOrigin();
	res = g.act.FisrtMotorBackToOrigin();
	res = g.act.FisrtMotorBackToOrigin();
	res = g.act.FisrtMotorBackToOrigin();
	if (res) return res;//每次回到原点
	
	for (size_t i = 0; i < 32; i++)
	{
		if (!read_input_state(MOTOR1_ORG)) return MOTOR_TIME_OUT; //检查是否回到原点。
		Sleep(1);
	}

	if (!g.act.WaitMotorTimeout(FIRST_MOTOR, 10000))	return MOTOR_TIME_OUT;
	DWORD tick = GetTickCount();
	while (WAIT_OBJECT_0 == WaitForSingleObject(g.evtStartRecord, 1)){
		if (((GetTickCount())-tick)>1000)
		{
			return REC_TIME_OUT;
		}
	}
	for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTOVER, ON);
	for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTING, OFF);
	return NoError;
}


bool  CCommunicationReaderDlg::BeginProcedureMonitor()
{
	tProcedureMonitor = AfxBeginThread(ProcedureMonitor, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT CCommunicationReaderDlg::ProcedureMonitor(LPVOID param)
{
	CCommunicationReaderDlg* ComDlg = (CCommunicationReaderDlg*)param;
	for (;;) //windows编译器有bug while(1)可能会造成死锁
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g.evtMotorProcedure, 1))
		{
			ComDlg->ButtonDisable();
			for (int i = 0; i < MAX_COUNTER; i++) write_output(OUT_NG, OFF);
			for (int i = 0; i < MAX_COUNTER; i++) write_output(OUT_OK, OFF);
			int res = ComDlg->Procedure();
			ComDlg->ButtonEnable();
#if 1
			switch (res)
			{
			case NoError:break;
			case EMERGENGCY:ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"急停按钮按下！"); break;
			case MOTOR_TIME_OUT:ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"伺服电机运行超时！请加快速度");;break;
			case MOTOR_ALM:ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"伺服电机报警"); break;
			case NOT_ALREADY:ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"没有经过复位"); break;
			case REC_TIME_OUT:ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"excel记录超时"); break;
			case BACKTIMEOUT:ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"上升过程超时"); break;
			default:
				ComDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(L"多个错误,电机报警？伺服没复位？"); break;
				break;
			}
#endif
			g.evtMotorProcedure.ResetEvent();
		}
		Sleep(20);
	}
}




LRESULT CCommunicationReaderDlg::OnCommunication1(WPARAM ch, LPARAM port)
{
	static int count = 0;

	if (g.msgIndex1 >= BUFFSIZE)
		g.msgIndex1 = 0; //缓存区满以后从头覆盖

	g.msg1[g.msgIndex1] = ch;
	++g.msgIndex1;

	if (ch == '\r')
	{
		++count;
		if (count >= 2) //接收完毕
		{
			count = 0;
			g.msg1[g.msgIndex1] = '\0';

			if (g.msg1[1] == '1')
			{
				/*将T1转为T3*/
				g.msg1[1] = '3'; //"T1\rT1"->"T3\rT1"
			}

			else if (g.msg1[1] == '2')
			{
				/*将T2转为T4*/
				g.msg1[1] = '4'; //"T2\rT2"->"T4\rT4"
			}

			g.msgIndex1 = 0;

			MsgIndex_Offset = 5;

			rec_flags1 = true;

		}
		else if (count == 1 && g.msg1[g.msgIndex1 - 2] == 'E')
		{
			count = 0;
			g.msg1[g.msgIndex1] = '\0';

			if (g.msg1[1] == '1')
			{
				/*将T1转为T3*/
				g.msg1[1] = '3'; //"T1"->"T3"
			}

			else if (g.msg1[1] == '2')
			{
				/*将T2转为T4*/
				g.msg1[1] = '4'; //"T2"->"T4"
			}

			g.msgIndex1 = 0;

			MsgIndex_Offset = 2;

			rec_flags1 = true;
		}
	}
	return 0;
}
void CCommunicationReaderDlg::decodeMsg(char *msg)
{
	return;
}
void CCommunicationReaderDlg::ClearData()
{
	memset(g.msg, 0x0, BUFFSIZE*sizeof(char));
}


void CCommunicationReaderDlg::ClearData1()
{
	memset(g.msg1, 0x0, BUFFSIZE*sizeof(char));
}

void CCommunicationReaderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!(MessageBox(L"确认退出?", L"", MB_YESNO | MB_ICONQUESTION) == IDYES)) return;
	//关闭串口
	KillTimer(GTMONITOR);
	KillTimer(DATAMONITOR);
	KillTimer(IOSCANNER);

	Port1.Close();
	xls.End();
	//Port2.Close();
	

	/*写入时间*/
	//WriteTime();//////


	
//	CString s1, s2, s3, s4;
//	s1.Format(L"%d\r\n",Com1T1Cnt);
	//s2.Format(L"%d\r\n", Com1T2Cnt);
	//s3.Format(L"%d\r\n", Com2T1Cnt);
	//s4.Format(L"%d\r\n", Com2T2Cnt);
	//xls.SetStateFile(CurXlsFileName + L"\r\n",s1,s2,s3,s4,xls.statefile_path);
	//结束excel，并杀死进程，保证软件执行完毕后能够正常打开excel
	
	g.act.CloseCard();

#ifdef MYDEBUG
	FreeConsole();
#endif
	
	//等待500ms是为了保证上面的程序执行完毕
	Sleep(300);


	CDialogEx::OnClose();
}


void CCommunicationReaderDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

}


//void CCommunicationReaderDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CCommunicationReaderDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


BOOL CCommunicationReaderDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void CCommunicationReaderDlg::SetListItemText(void)
{
	UpdateData(TRUE);
	CString str;
	vector<CString> str1;
	CStringDeal Cd;
	switch (listSelFlag[0])
	{
	case 1:m_list1.Invalidate(FALSE);m_list1.SetItemText(listSelFlag[1], listSelFlag[2], cstrItemTextEdit);
		for (int i = 0;i < m_list1.GetItemCount();i++)
			for (int j = 0;j < list_col;j++)
			{
				CString s = m_list1.GetItemText(i,j);
				str += s;
				str += L",";
			}
		ini.CompareValue1 = str;
		Cd.cutString((LPSTR)(LPCSTR)(CStringA)str, str1);
		compvalue1.clear();
		for (size_t i = 0; i < str1.size(); compvalue1.push_back(atof((LPSTR)(LPCSTR)(CStringA)str1[i])), i++);
		ini.SaveParaFile(PARA_IMAGE);
		break;
#if 0
	case 2:m_list2.Invalidate(FALSE);m_list2.SetItemText(listSelFlag[1], listSelFlag[2], cstrItemTextEdit);
		for (int i = 0;i < m_list2.GetItemCount();i++)
			for (int j = 0;j < list_col;j++)
			{
				CString s = m_list2.GetItemText(i, j);
				str += s;
				str += L",";
			}
		ini.CompareValue2 = str;
		Cd.cutString((LPSTR)(LPCSTR)(CStringA)str, str1);
		compvalue2.clear();
		for (int i = 0;i < str1.size();compvalue2.push_back(atof((LPSTR)(LPCSTR)(CStringA)str1[i])), i++);
		ini.SaveParaFile(PARA_IMAGE);
		break;
	case 3:m_list3.Invalidate(FALSE);m_list3.SetItemText(listSelFlag[1], listSelFlag[2], cstrItemTextEdit);
		for (int i = 0;i < m_list3.GetItemCount();i++)
			for (int j = 0;j < list_col;j++)
			{
				CString s = m_list3.GetItemText(i, j);
				str += s;
				str += L",";
			}
		ini.CompareValue3 = str;
		Cd.cutString((LPSTR)(LPCSTR)(CStringA)str, str1);
		compvalue3.clear();
		for (int i = 0;i < str1.size();compvalue3.push_back(atof((LPSTR)(LPCSTR)(CStringA)str1[i])), i++);
		ini.SaveParaFile(PARA_IMAGE);
		break;
	case 4:m_list4.Invalidate(FALSE);m_list4.SetItemText(listSelFlag[1], listSelFlag[2], cstrItemTextEdit);
		for (int i = 0;i < m_list4.GetItemCount();i++)
			for (int j = 0;j < list_col;j++)
			{
				CString s = m_list4.GetItemText(i, j);
				str += s;
				str += L",";
			}
		ini.CompareValue4 = str;
		Cd.cutString((LPSTR)(LPCSTR)(CStringA)str, str1);
		compvalue4.clear();
		for (int i = 0;i < str1.size();compvalue4.push_back(atof((LPSTR)(LPCSTR)(CStringA)str1[i])), i++);
		ini.SaveParaFile(PARA_IMAGE);
		break;
#endif
	default:
		break;
	}
	//
	m_SubItemEdit.ShowWindow(SW_HIDE);
	return;

}

BOOL CCommunicationReaderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			start_tv_move(FIRST_MOTOR, 500, ini.m_Motor1Speed, 3);
		}

		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			start_tv_move(FIRST_MOTOR, -500, -1*ini.m_Motor1Speed, 3);
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_UP)->m_hWnd)
		{
			// 按钮弹起
			d1000_decel_stop(FIRST_MOTOR);
		}

		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_DOWN)->m_hWnd)
		{
			d1000_decel_stop(FIRST_MOTOR);
		}
	}
	
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->GetDlgCtrlID() == IDC_EDIT_SUB)
		{
	//		if (listSelFlag[0] == 1)
	//			m_list1.SetFocus();//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息  
			switch (listSelFlag[0])
			{
			case 1:m_list1.SetFocus(); break;//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息  break;
//			case 2:m_list2.SetFocus();break;//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息  break;
//			case 3:m_list3.SetFocus();break;//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息  break;
//			case 4:m_list4.SetFocus();break;//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息  break;
			default:
				break;
			}
		}
		return FALSE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CCommunicationReaderDlg::OnKillfocusEditSub()
{
	// TODO: 在此添加控件通知处理程序代码
	SetListItemText();
}

void CCommunicationReaderDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pNMListCtrl->iItem != -1)
	{
		cstrItemTextEdit = m_list1.GetItemText(pNMListCtrl->iItem, pNMListCtrl->iSubItem);
		UpdateData(FALSE);
		CRect rect, dlgRect;
		//获得当前列的宽度以适应如果用户调整宽度  
		//此处不用获得的子项rect矩形框来设置宽度  
		//是因第一列时返回的宽度是整行的宽度，我也不知道为啥  
		int width = m_list1.GetColumnWidth(pNMListCtrl->iSubItem);
		m_list1.GetSubItemRect(pNMListCtrl->iItem, pNMListCtrl->iSubItem, LVIR_BOUNDS, rect);
		//保存选择的列表项索引  
		//这个因为我是用了两个列表控件一个CEdit  
		//所以需要保存列表的索引  
		//以及子项相对应的行列号索引  
		listSelFlag[0] = 1;//列表1  
		listSelFlag[1] = pNMListCtrl->iItem;
		listSelFlag[2] = pNMListCtrl->iSubItem;
		//获得listctrl矩形框  
		//获得父对话框的位置以调整CEdit的显示位置，具体见下面代码  
		m_list1.GetWindowRect(&dlgRect);
		//调整与父窗口对应  
		ScreenToClient(&dlgRect);
		int height = rect.Height();
		rect.top += (dlgRect.top + 1);
		rect.left += (dlgRect.left + 1);
		rect.bottom = rect.top + height + 2;
		rect.right = rect.left + width + 2;
		m_SubItemEdit.MoveWindow(&rect);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}
	else
	{
		int MaxRow = m_list1.GetItemCount();
		m_list1.InsertItem(MaxRow, _T("0.0"), LVCFMT_LEFT);
		m_list1.SetItemText(MaxRow, 1, _T("0.0"));
		m_list1.SetItemText(MaxRow, 2, _T("0.0"));
	}

	*pResult = 0;
}


void CCommunicationReaderDlg::OnRdblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	int MaxRow = m_list1.GetItemCount();
	m_list1.DeleteItem(MaxRow-1);
	*pResult = 0;
}

#if 0
void CCommunicationReaderDlg::OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{

	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
		NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pNMListCtrl->iItem != -1)
	{
		
		cstrItemTextEdit = m_list2.GetItemText(pNMListCtrl->iItem, pNMListCtrl->iSubItem);
		UpdateData(FALSE);
		CRect rect, dlgRect;
		//获得当前列的宽度以适应如果用户调整宽度  
		//此处不用获得的子项rect矩形框来设置宽度  
		//是因第一列时返回的宽度是整行的宽度，我也不知道为啥  
		int width = m_list2.GetColumnWidth(pNMListCtrl->iSubItem);
		m_list2.GetSubItemRect(pNMListCtrl->iItem, pNMListCtrl->iSubItem, LVIR_BOUNDS, rect);
		//保存选择的列表项索引  
		//这个因为我是用了两个列表控件一个CEdit  
		//所以需要保存列表的索引  
		//以及子项相对应的行列号索引  
		listSelFlag[0] = 2;//列表1  
		listSelFlag[1] = pNMListCtrl->iItem;
		listSelFlag[2] = pNMListCtrl->iSubItem;
		//获得listctrl矩形框  
		//获得父对话框的位置以调整CEdit的显示位置，具体见下面代码  
		m_list2.GetWindowRect(&dlgRect);
		//调整与父窗口对应  
		ScreenToClient(&dlgRect);
		int height = rect.Height();
		rect.top += (dlgRect.top + 1);
		rect.left += (dlgRect.left + 1);
		rect.bottom = rect.top + height + 2;
		rect.right = rect.left + width + 2;
		m_SubItemEdit.MoveWindow(&rect);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}
	else
	{
		int MaxRow = m_list2.GetItemCount();
		m_list2.InsertItem(MaxRow, _T("0.0"), LVCFMT_LEFT);
		m_list2.SetItemText(MaxRow, 1, _T("0.0"));
		m_list2.SetItemText(MaxRow, 2, _T("0.0"));
	}
	*pResult = 0;
}


void CCommunicationReaderDlg::OnRdblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	int MaxRow = m_list2.GetItemCount();
	m_list2.DeleteItem(MaxRow - 1);
	*pResult = 0;
}


void CCommunicationReaderDlg::OnDblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pNMListCtrl->iItem != -1)
	{

		cstrItemTextEdit = m_list3.GetItemText(pNMListCtrl->iItem, pNMListCtrl->iSubItem);
		UpdateData(FALSE);
		CRect rect, dlgRect;
		//获得当前列的宽度以适应如果用户调整宽度  
		//此处不用获得的子项rect矩形框来设置宽度  
		//是因第一列时返回的宽度是整行的宽度，我也不知道为啥  
		int width = m_list3.GetColumnWidth(pNMListCtrl->iSubItem);
		m_list3.GetSubItemRect(pNMListCtrl->iItem, pNMListCtrl->iSubItem, LVIR_BOUNDS, rect);
		//保存选择的列表项索引  
		//这个因为我是用了两个列表控件一个CEdit  
		//所以需要保存列表的索引  
		//以及子项相对应的行列号索引  
		listSelFlag[0] = 3;//列表1  
		listSelFlag[1] = pNMListCtrl->iItem;
		listSelFlag[2] = pNMListCtrl->iSubItem;
		//获得listctrl矩形框  
		//获得父对话框的位置以调整CEdit的显示位置，具体见下面代码  
		m_list3.GetWindowRect(&dlgRect);
		//调整与父窗口对应  
		ScreenToClient(&dlgRect);
		int height = rect.Height();
		rect.top += (dlgRect.top + 1);
		rect.left += (dlgRect.left + 1);
		rect.bottom = rect.top + height + 2;
		rect.right = rect.left + width + 2;
		m_SubItemEdit.MoveWindow(&rect);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}
	else
	{
		int MaxRow = m_list3.GetItemCount();
		m_list3.InsertItem(MaxRow, _T("0.0"), LVCFMT_LEFT);
		m_list3.SetItemText(MaxRow, 1, _T("0.0"));
		m_list3.SetItemText(MaxRow, 2, _T("0.0"));
	}
	*pResult = 0;
}


void CCommunicationReaderDlg::OnRdblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	int MaxRow = m_list3.GetItemCount();
	m_list3.DeleteItem(MaxRow - 1);
	*pResult = 0;
}


void CCommunicationReaderDlg::OnDblclkList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	if (pNMListCtrl->iItem != -1)
	{

		cstrItemTextEdit = m_list4.GetItemText(pNMListCtrl->iItem, pNMListCtrl->iSubItem);
		UpdateData(FALSE);
		CRect rect, dlgRect;
		//获得当前列的宽度以适应如果用户调整宽度  
		//此处不用获得的子项rect矩形框来设置宽度  
		//是因第一列时返回的宽度是整行的宽度，我也不知道为啥  
		int width = m_list4.GetColumnWidth(pNMListCtrl->iSubItem);
		m_list4.GetSubItemRect(pNMListCtrl->iItem, pNMListCtrl->iSubItem, LVIR_BOUNDS, rect);
		//保存选择的列表项索引  
		//这个因为我是用了两个列表控件一个CEdit  
		//所以需要保存列表的索引  
		//以及子项相对应的行列号索引  
		listSelFlag[0] = 4;//列表1  
		listSelFlag[1] = pNMListCtrl->iItem;
		listSelFlag[2] = pNMListCtrl->iSubItem;
		//获得listctrl矩形框  
		//获得父对话框的位置以调整CEdit的显示位置，具体见下面代码  
		m_list4.GetWindowRect(&dlgRect);
		//调整与父窗口对应  
		ScreenToClient(&dlgRect);
		int height = rect.Height();
		rect.top += (dlgRect.top + 1);
		rect.left += (dlgRect.left + 1);
		rect.bottom = rect.top + height + 2;
		rect.right = rect.left + width + 2;
		m_SubItemEdit.MoveWindow(&rect);
		m_SubItemEdit.ShowWindow(SW_SHOW);
		m_SubItemEdit.SetFocus();
	}
	else
	{
		int MaxRow = m_list4.GetItemCount();
		m_list4.InsertItem(MaxRow, _T("0.0"), LVCFMT_LEFT);
		m_list4.SetItemText(MaxRow, 1, _T("0.0"));
		m_list4.SetItemText(MaxRow, 2, _T("0.0"));
	}
	*pResult = 0;
}


void CCommunicationReaderDlg::OnRdblclkList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	int MaxRow = m_list4.GetItemCount();
	m_list4.DeleteItem(MaxRow - 1);
	*pResult = 0;
}
#endif
void CCommunicationReaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
	    ReSize();
	}
}



void CCommunicationReaderDlg::ReSize()
{
	return;
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);     //取客户区大小  
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角
	CPoint OldBRPoint, BRPoint; //右下角
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件  
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);

		OldTLPoint = Rect.TopLeft();

		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);

		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);

		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;

	m_font_height = m_font_height * fsp[0];
	m_font_width = m_font_width * fsp[1];

	font.CreateFont(m_font_height, m_font_width, 0, 0, FW_THIN, false, false, false,
		CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
		FF_MODERN, L"宋体");

	/*加上启动不了程序*/
	//GetDlgItem(IDC_EDIT1)->SetFont(&font); 
	//GetDlgItem(IDC_EDIT2)->SetFont(&font);
	//GetDlgItem(IDC_EDIT3)->SetFont(&font);
	//GetDlgItem(IDC_EDIT4)->SetFont(&font);
}
void CCommunicationReaderDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	if (nHitTest == HTCAPTION) 
		return;
	CDialogEx::OnNcLButtonDblClk(nHitTest, point);
}
void CCommunicationReaderDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHitTest == HTCAPTION)
		return;
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}

//void CCommunicationReaderDlg::OnSize(UINT nType, int cx, int cy)
//{
//	CDialogEx::OnSize(nType, cx, cy);
//
//	// TODO: 在此处添加消息处理程序代码
//
//	/*设置控件大小*/
//	double sx = cx / m_rect.Width();
//	double sy = cy / m_rect.Height();
//#ifdef MYDEBUG
//	printf("sx = %lf,sy = %lf\n",sx,sy);
//#endif
//	//CWnd *pWnd;
//	//pWnd = GetDlgItem(IDC_EDIT1);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_EDIT2);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_EDIT3);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_EDIT4);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//
//	//pWnd = GetDlgItem(IDC_CAMERA1);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_CAMERA2);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_CAMERA3);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_CAMERA4);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	//pWnd = GetDlgItem(IDC_SAVE);     //获取ID为i的空间的句柄，因为“确认”ID为1，“取消”ID为2
//	//if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//	//{
//	//	CRect rect;   //获取控件变化前的大小 
//
//	//	pWnd->GetWindowRect(&rect);
//	//	ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
//	//						  //cx/m_rect.Width()为对话框在横向的变化比例
//	//	rect.left = rect.left*sx;//调整控件大小
//	//	rect.right = rect.right*sx;
//	//	rect.top = rect.top*sy;
//	//	rect.bottom = rect.bottom*sy;
//	//	pWnd->MoveWindow(rect);//设置控件大小
//	//}
//	/*设置空间大小结束*/
//
//	//m_font_height = m_font_height * sy;
//	//m_font_width = m_font_width * sx;
//
//	//font.CreateFont(m_font_height, m_font_width, 0, 0, FW_THIN, false, false, false,
//	//	CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS,
//	//	CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
//	//	FF_MODERN, L"宋体");
//
//	GetClientRect(&m_rect);
//}


void CCommunicationReaderDlg::OnBnClickedStartDetect()
{
	// TODO:  在此添加控件通知处理程序代码
	/*
	if (g.test)
	{ 
		ButtonDisable();
		g.test = false;
	}
	else
	{
		ButtonEnable();
		g.test = true;
	}
	*/

	g.evtMotorProcedure.SetEvent();
}

void CCommunicationReaderDlg::showOK()
{
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC);
	HBITMAP hBitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAPOK));
	pStatic->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	pStatic->SetBitmap(hBitmap);
	g.act.isOK();
}

void CCommunicationReaderDlg::showNG()
{
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC);
	HBITMAP hBitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAPNG));
	pStatic->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	pStatic->SetBitmap(hBitmap);
	g.act.isNotOK();
}

void CCommunicationReaderDlg::showError()
{
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_WARNING);
	HBITMAP hBitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAPWARNING));
	pStatic->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	pStatic->SetBitmap(hBitmap);
}

void CCommunicationReaderDlg::showClear()
{
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_WARNING);
	HBITMAP hBitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAPCLEAR));
	pStatic->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	pStatic->SetBitmap(hBitmap);
}

void CCommunicationReaderDlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CCommunicationReaderDlg::OnBnClickedButtonResetdlg()
{
	// TODO:  在此添加控件通知处理程序代码
	ResetDlg dlg;
	dlg.DoModal();
}


void CCommunicationReaderDlg::OnBnClickedStartTozero()
{
	// TODO:  在此添加控件通知处理程序代码
	ini.m_Motor1Step = d1000_get_command_pos(FIRST_MOTOR);
	if (!tAdjustZero)
		BeginAdjustZeroMonitor();

	
}

bool CCommunicationReaderDlg::BeginAdjustZeroMonitor()
{
	tAdjustZero = AfxBeginThread(AdjustZeroMonitor, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}


UINT CCommunicationReaderDlg::AdjustZeroMonitor(LPVOID param)
{
	try{
		CCommunicationReaderDlg* ComDlg = (CCommunicationReaderDlg*)param;
		CStringDeal Cd;
		vector<CString> str;
		CString str1;
		DWORD tick = GetTickCount();
		for (; !g.isReciveOver;)
		{
			if ((GetTickCount() - tick) > 3000)
			{
				ComDlg->m_camerares2 = L"等待发送结束时间超时";
				ComDlg->UpdateData(FALSE);
				return 1;
			}
		}


		Cd.cutString(g.msg1, str);
		//	std::lock_guard<std::mutex> lck(g.mtx);
		if (str[0].Compare(L"MS"))
		{
			ComDlg->m_camerares2 = L"文件标头不正确，是否感应器出问题？";
			ComDlg->UpdateData(FALSE);
			return 1;
		}
		size_t sensor_idx = 0;//
		size_t sensor_start = 2;
		ComDlg->adjustValue1.clear();
		for (; (sensor_idx + sensor_start) <= str.size(); sensor_idx += 2)
		{
			CString s = str[sensor_idx + sensor_start];
			str1 = str1 + s + L',';
			//	adjustValue1.push_back(wcstof((LPWSTR)(LPCSTR)(CStringA)s,NULL));
			ComDlg->adjustValue1.push_back(atof((LPCSTR)(CStringA)s));
#ifdef MYDEBUG
			cout << atof((LPCSTR)(CStringA)s) << endl;
#endif
		}
		ini.adjustValue1 = str1;
		ini.SaveParaFile(PARA_ALL);
		Sleep(300);
		ComDlg->GetDlgItem(IDC_EDIT4)->SetWindowText(str1);
		delete ComDlg->tAdjustZero;
		ComDlg->tAdjustZero = NULL;
		return 0;
	}
	catch (...)
	{
		AfxMessageBox(L"can not find");
	}
	return 0;
}

void CCommunicationReaderDlg::copy_data(char** dst)
{
	std::lock_guard<std::mutex> lck(g.mtx);
	size_t len =1 + strlen(g.msg);
	memcpy(&dst, g.msg, len);
}

void CCommunicationReaderDlg::ButtonDisable() //禁止按键，目的是在检测中防止乱按造成机械碰撞
{
	GetDlgItem(IDC_BUTTON_DOWNTOCELIANG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESETDLG)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_DETECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_TOZERO)->EnableWindow(FALSE);

}


void CCommunicationReaderDlg::ButtonEnable() //禁止按键，目的是在检测中防止乱按造成机械碰撞
{
	GetDlgItem(IDC_BUTTON_DOWNTOCELIANG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESETDLG)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_DETECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_TOZERO)->EnableWindow(TRUE);
}


void CCommunicationReaderDlg::OnBnClickedButtonDowntoceliang()
{
	// TODO:  在此添加控件通知处理程序代码
	if (g.RunDisable)
	{
		AfxMessageBox(L"没有复位过或者报警");
		return;
	}

	int tmp = ini.m_Motor1Wise * ini.m_Motor1Step - d1000_get_command_pos(FIRST_MOTOR);
	start_t_move(FIRST_MOTOR, tmp, 500, ini.m_Motor1Speed, 0.05);
	GetDlgItem(IDC_BUTTON_DOWNTOCELIANG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(FALSE);
	DWORD Tick = GetTickCount();
	for (; !d1000_check_done(FIRST_MOTOR);)
	{
		if ((GetTickCount() - Tick) > 12000)
			break;
	}
	GetDlgItem(IDC_BUTTON_DOWNTOCELIANG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(TRUE);
}


void CCommunicationReaderDlg::OnBnClickedButtonBack()
{
	// TODO:  在此添加控件通知处理程序代码
#ifdef MYDEBUG
	if (g.test)
	{
		ButtonDisable();
		cout << "disable" << endl;
		g.test = false;

	}
	else
	{
		ButtonEnable();
		cout << "enable" << endl;
		g.test = true;
	}
#endif
	if (g.RunDisable)
	{
		AfxMessageBox(L"没有复位过或者报警");
		return;
	}	
	GetDlgItem(IDC_BUTTON_DOWNTOCELIANG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(FALSE);
	long tmp = -1 * d1000_get_command_pos(FIRST_MOTOR);
	start_t_move(FIRST_MOTOR, tmp, 500, ini.m_Motor1Speed, 0.05);
	DWORD Tick = GetTickCount();
	for (; !d1000_check_done(FIRST_MOTOR);)
	{
		if ((GetTickCount() - Tick) > 12000)
			break;
	}
	GetDlgItem(IDC_BUTTON_DOWNTOCELIANG)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(TRUE);
}


void CCommunicationReaderDlg::OnClickedGetthreshold()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_THRES)->GetWindowText(str);
	ini.m_value_threshold = atof((LPSTR)(LPCSTR)(CStringA)str);
	ini.SaveParaFile(PARA_PRJ);
}
