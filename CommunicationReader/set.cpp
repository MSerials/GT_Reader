// set.cpp : 实现文件
//

#include "stdafx.h"
#include "CommunicationReader.h"
#include "set.h"
#include "afxdialogex.h"
#include "winspool.h"
#include "ExString.h"

// set 对话框

IMPLEMENT_DYNAMIC(set, CDialogEx)

set::set(CWnd* pParent /*=NULL*/)
	: CDialogEx(set::IDD, pParent)
	, m_SelCom1(_T(""))
//	, m_SelCom2(_T(""))
	, m_SelBit1(_T(""))
//	, m_SelBit2(_T(""))
	, m_SelDataLen1(_T(""))
//	, m_SelDataLen2(_T(""))
	, m_SelStop1(_T(""))
//	, m_SelStop2(_T(""))
	, m_SelPar1(_T(""))
//	, m_SelPar2(_T(""))
	, m_rev1(_T(""))
//	, m_rev2(_T(""))
	, m_send1(_T(""))
//	, m_send2(_T(""))
	, m_s1(_T(""))
//	, m_s2(_T(""))
	, TimeLimit(0)
	, m_row(0)
	, m_col(0)
	, m_row1(0)
	, m_col1(0)
	, model_path(_T(""))
	, m_speed(0)
	, m_acc(0)
	, m_autopuls(0)
	, m_backspeed(0)
	, m_manpuls(0)
	, m_compensate(0)
{

}

set::~set()
{
}

void set::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBOSELCOM1, m_SelCom1);
	//	DDX_CBString(pDX, IDC_COMBOSELCOM2, m_SelCom2);
	DDX_CBString(pDX, IDC_COMBOSELBIT1, m_SelBit1);
	//	DDX_CBString(pDX, IDC_COMBOSELBIT2, m_SelBit2);
	DDX_CBString(pDX, IDC_COMBOSELDATALEN1, m_SelDataLen1);
	//	DDX_CBString(pDX, IDC_COMBOSELDATALEN2, m_SelDataLen2);
	DDX_CBString(pDX, IDC_COMBOSELSTOP1, m_SelStop1);
	//	DDX_CBString(pDX, IDC_COMBOSELSTOP2, m_SelStop2);
	DDX_CBString(pDX, IDC_COMBOSELPAR1, m_SelPar1);
	//	DDX_CBString(pDX, IDC_COMBOSELPAR2, m_SelPar2);
	DDX_Control(pDX, IDC_COMBOSELCOM1, m_cSelCom1);
	//	DDX_Control(pDX, IDC_COMBOSELCOM2, m_cSelCom2);
	DDX_Control(pDX, IDC_COMBOSELBIT1, m_cSelBit1);
	//	DDX_Control(pDX, IDC_COMBOSELBIT2, m_cSelBit2);
	DDX_Control(pDX, IDC_COMBOSELDATALEN1, m_cSelDataLen1);
	//	DDX_Control(pDX, IDC_COMBOSELDATALEN2, m_cSelDataLen2);
	DDX_Control(pDX, IDC_COMBOSELSTOP1, m_cSelStop1);
	//	DDX_Control(pDX, IDC_COMBOSELSTOP2, m_cSelStop2);
	DDX_Control(pDX, IDC_COMBOSELPAR1, m_cSelPar1);
	//	DDX_Control(pDX, IDC_COMBOSELPAR2, m_cSelPar2);
	DDX_Text(pDX, IDC_EDIT_REV1, m_rev1);
	//	DDX_Text(pDX, IDC_EDITREV2, m_rev2);
	DDX_Text(pDX, IDC_EDIT_SEND1, m_send1);
	//	DDX_Text(pDX, IDC_EDIT_SEND2, m_send2);
	DDX_Text(pDX, IDC_EDIT_S1, m_s1);
	//	DDX_Text(pDX, IDC_EDIT_S2, m_s2);
	DDX_Text(pDX, IDC_EDIT8, TimeLimit);
	DDX_Text(pDX, IDC_EDIT2, m_row);
	DDX_Text(pDX, IDC_EDIT5, m_col);
	DDX_Text(pDX, IDC_EDIT6, m_row1);
	DDX_Text(pDX, IDC_EDIT7, m_col1);
	DDX_Text(pDX, IDC_EDIT4, model_path);
	DDX_Control(pDX, IDC_EDIT_REV1, m_recv1);
	DDX_Control(pDX, IDC_EDITREV2, m_recv2);
	DDX_Control(pDX, IDC_COMBOSE_WISE, m_spinwise);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_speed);
	DDX_Text(pDX, IDC_EDIT_ACC, m_acc);
	DDV_MinMaxInt(pDX, m_acc, 0, 300);
	DDX_Text(pDX, IDC_EDIT_PULSAUTO, m_autopuls);
	DDV_MinMaxInt(pDX, m_autopuls, -2100000000, 2100000000);
	DDX_Text(pDX, IDC_EDIT_BACKSPEED, m_backspeed);
	DDX_Text(pDX, IDC_EDIT_MANPULS, m_manpuls);
	DDX_Text(pDX, IDC_EDIT_COMPASENTE, m_compensate);
}


BEGIN_MESSAGE_MAP(set, CDialogEx)
	ON_MESSAGE(WM_COMM_RXCHAR, &set::OnCommunication)
//	ON_MESSAGE(WM_COMM_RXCHAR1, &set::OnCommunication1)
	ON_CBN_SELCHANGE(IDC_COMBOSELCOM1, &set::OnSelchangeComboselcom1)
	ON_CBN_SELCHANGE(IDC_COMBOSELBIT1, &set::OnSelchangeComboselbit1)
	ON_CBN_SELCHANGE(IDC_COMBOSELDATALEN1, &set::OnSelchangeComboseldatalen1)
	ON_CBN_SELCHANGE(IDC_COMBOSELSTOP1, &set::OnSelchangeComboselstop1)
	ON_CBN_SELCHANGE(IDC_COMBOSELPAR1, &set::OnSelchangeComboselpar1)
//	ON_CBN_SELCHANGE(IDC_COMBOSELCOM2, &set::OnSelchangeComboselcom2)
//	ON_CBN_SELCHANGE(IDC_COMBOSELBIT2, &set::OnSelchangeComboselbit2)
//	ON_CBN_SELCHANGE(IDC_COMBOSELDATALEN2, &set::OnSelchangeComboseldatalen2)
//	ON_CBN_SELCHANGE(IDC_COMBOSELPAR2, &set::OnSelchangeComboselpar2)
//	ON_CBN_SELCHANGE(IDC_COMBOSELSTOP2, &set::OnSelchangeComboselstop2)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE1, &set::OnBnClickedButtonClose1)
//	ON_BN_CLICKED(IDC_BUTTON_CLOSE2, &set::OnBnClickedButtonClose2)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &set::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SELMODEL, &set::OnBnClickedButtonSelmodel)
	ON_BN_CLICKED(IDC_BUTTON_SEND1, &set::OnBnClickedButtonSend1)
//	ON_BN_CLICKED(IDC_BUTTON_SEND2, &set::OnBnClickedButtonSend2)
	ON_EN_CHANGE(IDC_EDIT_REV1, &set::OnEnChangeEditRev1)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, &set::OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, &set::OnBnClickedButtonMoveup)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &set::OnBnClickedButtonBack)
	ON_CBN_SELCHANGE(IDC_COMBOSE_WISE, &set::OnSelchangeComboseWise)
	ON_BN_CLICKED(IDC_BUTTON_COMF, &set::OnBnClickedButtonComf)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// set 消息处理程序


void set::OnSelchangeComboselcom1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelCom1.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelCom1.GetLBText(nSel, str);
	int Num = GetNumFromComStr(str);
	ini.portnum = Num;
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboselbit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelBit1.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelBit1.GetLBText(nSel, str);
	ini.baud = atoi((LPSTR)(LPCSTR)(CStringA)str);
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboseldatalen1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelDataLen1.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelDataLen1.GetLBText(nSel, str);
	ini.databits = atoi((LPSTR)(LPCSTR)(CStringA)str);
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboselstop1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelStop1.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelStop1.GetLBText(nSel, str);
	ini.stopbits = str;
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboselpar1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelPar1.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelPar1.GetLBText(nSel, str);
	ini.parity = str;
	ini.SaveParaFile(PARA_PRJ);
}

#if 0
void set::OnSelchangeComboselcom2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelCom2.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelCom2.GetLBText(nSel, str);
	int Num = GetNumFromComStr(str);
	ini.portnum1 = Num;
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboselbit2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelBit2.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelBit2.GetLBText(nSel, str);
	ini.baud1 = atoi((LPSTR)(LPCSTR)(CStringA)str);
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboseldatalen2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelDataLen2.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelDataLen2.GetLBText(nSel, str);
	ini.databits1 = atoi((LPSTR)(LPCSTR)(CStringA)str);
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboselpar2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelPar2.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelPar2.GetLBText(nSel, str);
	ini.parity1 = str;
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnSelchangeComboselstop2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_cSelStop2.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_cSelStop2.GetLBText(nSel, str);
	ini.stopbits1 = str;
	ini.SaveParaFile(PARA_PRJ);
}
#endif

void set::OnBnClickedButtonClose1()
{
	// TODO:  在此添加控件通知处理程序代码
	//char Number[64];	
	//itoa(ini.portnum, Number, 10);
	CString portnum;// = (CString)CStringA(Number);
	//portnum = L"COM" + portnum;

	portnum.Format(_T("COM%d"),ini.portnum);
	if (!g.com_state1)
	{
		//if (!Port1.InitPort(this, (LPSTR)(LPCSTR)(CStringA)portnum, (UINT)ini.baud, (LPSTR)(LPCSTR)(CStringA)ini.parity, (UINT)ini.databits, (LPSTR)(LPCSTR)(CStringA)ini.stopbits, EV_RXCHAR, 512, 512))
		if (!Port1.InitPort(pComReaderDlg, (LPSTR)(LPCSTR)(CStringA)portnum, (UINT)ini.baud, (LPSTR)(LPCSTR)(CStringA)ini.parity, (UINT)ini.databits, (LPSTR)(LPCSTR)(CStringA)ini.stopbits, EV_RXCHAR, 512, 512))
		{
			g.com_state1 = false;
			GetDlgItem(IDC_BUTTON_CLOSE1)->SetWindowText(L"未能打开串口");
		}
		else
		{
			Port1.StartMonitoring();
			GetDlgItem(IDC_BUTTON_CLOSE1)->SetWindowText(L"关闭串口");
			g.com_state1 = true;

		}
		return;
	}
	else
	{
		Port1.Close();
		GetDlgItem(IDC_BUTTON_CLOSE1)->SetWindowText(L"未打开串口");
		g.com_state1 = false;
		return;
	}
}

#if 0
void set::OnBnClickedButtonClose2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString portnum;
	portnum.Format(_T("COM%d"), ini.portnum1);

	if (!g.com_state2)
	{
		//if (!Port2.InitPort(this, (LPSTR)(LPCSTR)(CStringA)portnum, (UINT)ini.baud1, (LPSTR)(LPCSTR)(CStringA)ini.parity1, (UINT)ini.databits1, (LPSTR)(LPCSTR)(CStringA)ini.stopbits1, EV_RXCHAR, 512, 512))
		if (!Port2.InitPort(pComReaderDlg, (LPSTR)(LPCSTR)(CStringA)portnum, (UINT)ini.baud1, (LPSTR)(LPCSTR)(CStringA)ini.parity1, (UINT)ini.databits1, (LPSTR)(LPCSTR)(CStringA)ini.stopbits1, EV_RXCHAR, 512, 512))
		{
			g.com_state2 = false;
			GetDlgItem(IDC_BUTTON_CLOSE2)->SetWindowText(L"未能打开串口");
		}
		else
		{
			Port2.StartMonitoring();
			GetDlgItem(IDC_BUTTON_CLOSE2)->SetWindowText(L"关闭串口");
			g.com_state2 = true;
		}
		return;
	}
	else
	{
		Port2.Close();
		GetDlgItem(IDC_BUTTON_CLOSE2)->SetWindowText(L"未打开串口");
		g.com_state2 = false;
		return;
	}
}
#endif

int set::GetNumFromComStr(CString str)
{
	char *s =(LPSTR)(LPCSTR)(CStringA)(str);
	if (strlen(s) < 3) return 0;
	s[strlen(s) - 1] = '\0';
	char s1[64] = { 0 };
	for (int i = 3; *(s + i) != '\0'; i++)
	{
		s1[i - 3] = *(s + i);
	}
	return atoi(s1);
}

BOOL set::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//最先更新EDIT控件，否则setcur失效
	InitData();

	//枚举串口
	CString str0[256];
	int i = 0;
	int m_nSerialPortNum(0);// 串口计数 
	CString          strSerialList[256];  // 临时定义 256 个字符串组 
	LPBYTE pBite = NULL;
	DWORD pcbNeeded = 0;  // bytes received or required 
	DWORD pcReturned = 0;  // number of ports received 
	m_nSerialPortNum = 0;
	// 获取端口信息，能得到端口信息的大小 pcbNeeded 
	EnumPorts(NULL, 2, pBite, 0, &pcbNeeded, &pcReturned);
	pBite = new BYTE[pcbNeeded];
	// 枚举端口，能得到端口的具体信息 pBite 以及端口的的个数 pcReturned
	EnumPorts(NULL, 2, pBite, pcbNeeded, &pcbNeeded, &pcReturned);
	PORT_INFO_2 *pPort;
	pPort = (PORT_INFO_2*)pBite;
	for (i = 0; i < pcReturned; i++)
	{
		CString str = pPort[i].pPortName;
		if (str.Left(3) == "COM")
		{
			str0[m_nSerialPortNum] = str;
			m_nSerialPortNum++;
		}
	}



	if(!g.com_state1)
	{
		GetDlgItem(IDC_BUTTON_CLOSE1)->SetWindowText(L"未打开串口");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CLOSE1)->SetWindowText(L"关闭串口");
	}
#ifdef _GX
	if(!g.com_state2)
	{
		GetDlgItem(IDC_BUTTON_CLOSE2)->SetWindowText(L"未打开串口");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CLOSE2)->SetWindowText(L"关闭串口");
	}
#endif

	CComboBox *com1 = (CComboBox*)GetDlgItem(IDC_COMBOSELCOM1);
	CComboBox *bit1 = (CComboBox*)GetDlgItem(IDC_COMBOSELBIT1);
	CComboBox *datalen1 = (CComboBox*)GetDlgItem(IDC_COMBOSELDATALEN1);
	CComboBox *stop1 = (CComboBox*)GetDlgItem(IDC_COMBOSELSTOP1);
	CComboBox *par1 = (CComboBox*)GetDlgItem(IDC_COMBOSELPAR1);
#ifdef _GX
	CComboBox *com2 = (CComboBox*)GetDlgItem(IDC_COMBOSELCOM2);
	CComboBox *bit2 = (CComboBox*)GetDlgItem(IDC_COMBOSELBIT2);
	CComboBox *datalen2 = (CComboBox*)GetDlgItem(IDC_COMBOSELDATALEN2);
	CComboBox *stop2 = (CComboBox*)GetDlgItem(IDC_COMBOSELSTOP2);
	CComboBox *par2 = (CComboBox*)GetDlgItem(IDC_COMBOSELPAR2);
#endif

#ifdef _GT
	CComboBox *spinwise = (CComboBox*)GetDlgItem(IDC_COMBOSE_WISE);
#endif

	for (int i = 0; i < m_nSerialPortNum; i++)
	{
		m_cSelCom1.AddString(str0[i]);
		int number = GetNumFromComStr(str0[i]);
		if (ini.portnum == number)
			com1->SetCurSel(i);
	}
	CString str1[] = { _T("300"), _T("600"), _T("1200"), _T("2400"), _T("4800"), _T("9600"), _T("19200"), _T("38400"), _T("43000"), _T("56000"), _T("57600"), _T("115200") };
	for (int j = 0; j < 12; j++)
	{

		m_cSelBit1.AddString(str1[j]);
		int baud = atoi((LPSTR)(LPCSTR)(CStringA)str1[j]);
		if (ini.baud == baud)
			bit1->SetCurSel(j);
	}

	CString str2[] = { _T("0"), _T("1") };
	for (int j = 0; j < 2; j++)
	{
		m_cSelStop1.AddString(str2[j]);
		if (ini.stopbits == str2[j])
			stop1->SetCurSel(j);
	}

	CString str3[] = { _T("NONE"), _T("ODD"), _T("EVEN")};
	for (int j = 0; j < 3; j++)
	{
		m_cSelPar1.AddString(str3[j]);
		if (ini.parity== str3[j])
			par1->SetCurSel(j);
	}

	CString str4[] = { _T("7"), _T("8")};
	for (int j = 0; j < 2; j++)
	{
		m_cSelDataLen1.AddString(str4[j]);
		int databits = atoi((LPSTR)(LPCSTR)(CStringA)str4[j]);
		if (ini.databits == databits)
			datalen1->SetCurSel(j);
	}
#ifdef _GX
	for (int i = 0; i < m_nSerialPortNum; i++)
	{
		m_cSelCom2.AddString(str0[i]);
		int number = GetNumFromComStr(str0[i]);
		if (ini.portnum1 == number)
			com2->SetCurSel(i);
	}
	for (int j = 0; j < 12; j++)
	{

		m_cSelBit2.AddString(str1[j]);
		int baud = atoi((LPSTR)(LPCSTR)(CStringA)str1[j]);
		if (ini.baud1 == baud)
			bit2->SetCurSel(j);
	}
	for (int j = 0; j < 2; j++)
	{
		m_cSelStop2.AddString(str2[j]);
		if (ini.stopbits1 == str2[j])
			stop2->SetCurSel(j);
	}
	for (int j = 0; j < 3; j++)
	{
		m_cSelPar2.AddString(str3[j]);
		if (ini.parity1 == str3[j])
			par2->SetCurSel(j);
	}

	for (int j = 0; j < 2; j++)
	{
		m_cSelDataLen2.AddString(str4[j]);
		int databits = atoi((LPSTR)(LPCSTR)(CStringA)str4[j]);
		if (ini.databits1 == databits)
			datalen2->SetCurSel(j);
	}
#endif
	
#ifdef _GT
	spinwise->AddString(L"正转"); 
	spinwise->AddString(L"反转");
	if (ini.m_Motor1Wise == -1)
	{
		spinwise->SetCurSel(1);
	}
	else
	{
		spinwise->SetCurSel(0);
		if (1 != ini.m_Motor1Wise) {
			ini.m_Motor1Wise = 1; ini.SaveParaFile(PARA_PRJ);
		}
	}
#endif
	
	SetTimer(DATAMONITOR, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void set::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgData();
	ini.SaveParaFile(PARA_ALL);
	reFreshData();
}


void set::GetDlgData()
{
	UpdateData(TRUE);
	//ini.m_row = m_row;
	ini.m_row = m_row;
	ini.m_row1 = m_row1;

	ini.m_col = m_col;
	ini.m_col1 = m_col1;

	ini.TimeLimit = TimeLimit;

//	ini.m_Motor1Wise = m_spinwise;
	ini.m_Motor1Speed = m_speed;
	ini.m_Motor1BackSpeed = m_backspeed;
	ini.m_Motor1Acc = m_acc;
	ini.m_Motor1Compensate = m_compensate; //回原点补偿值
	ini.m_Motor1Step = m_autopuls;
	ini.m_Motor1StepByContol = m_manpuls;//手动控制用的步进
}

void set::reFreshData()
{
	g.row = ini.m_row;
	g.row1 = ini.m_row1;
	g.col = ini.m_col;
	g.col1 = ini.m_col1;
}


void set::InitData()
{
	m_row			=	ini.m_row;
	m_col			=	ini.m_col;

	m_row1			=	ini.m_row1;
	m_col1			=	ini.m_col1;

	TimeLimit		=	ini.TimeLimit;

	model_path		=	ini.model_path;

	m_backspeed		=	ini.m_Motor1BackSpeed;
	m_speed			=	ini.m_Motor1Speed;
	m_acc			=	ini.m_Motor1Acc;
	m_compensate	=	ini.m_Motor1Compensate;
	m_autopuls		=	ini.m_Motor1Step;
	m_manpuls		=	ini.m_Motor1StepByContol;
	UpdateData(FALSE);
}


void set::OnBnClickedButtonSelmodel()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE, (LPCTSTR)"xlsx", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"表格文件 (*.xlsx)|*.xlsx|All Files (*.*)|*.*||");	//打开读取文件对话框
	if (fileDlg.DoModal() != IDOK) return;
	ini.model_path = fileDlg.GetPathName();
	model_path = ini.model_path;
	ini.SaveParaFile(PARA_PRJ);
	UpdateData(FALSE);
}


void set::OnBnClickedButtonSend1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!g.com_state1) { AfxMessageBox(L"没有打开串口！"); return; }
	for (int i = 0; i < 32 && m_s1.IsEmpty(); i++) UpdateData(TRUE);
	if (m_s1.IsEmpty()) {AfxMessageBox(L"没有输入字符！" + m_s1); return;
}
	char* data = (LPSTR)(LPCSTR)(CStringA)(m_s1);
	regexp(data);
	Port1.WriteToPort(data, strlen(data));
	m_send1 = (CString)(CStringA)(LPCSTR)data;
	UpdateData(FALSE);
}

#if 0
void set::OnBnClickedButtonSend2()
{
	// TODO:  在此添加控件通知处理程序代码
	//GetDlgItemText(IDC_EDIT_S2, m_s2);
	if (!g.com_state2) { AfxMessageBox(L"没有打开串口！"); return; }
	for (int i = 0; i < 32 && m_s2.IsEmpty(); i++) UpdateData(TRUE);
	if (m_s2.IsEmpty()) {AfxMessageBox(L"没有输入字符！" + m_s2); return;
}
	char* data = (LPSTR)(LPCSTR)(CStringA)(m_s2);
	regexp(data);
	Port2.WriteToPort(data, strlen(data));
	m_send2 = (CString)(CStringA)(LPCSTR)data;
	UpdateData(FALSE);
}
#endif

BOOL set::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	return FALSE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void set::OnEnChangeEditRev1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

LRESULT set::OnCommunication(WPARAM ch, LPARAM port)
{
	char s[2] = { ch, 0 }; //s是edit里追加的字符串，是字符串！
	((CEdit*)GetDlgItem(IDC_EDIT_REV1))->SetSel(GetDlgItem(IDC_EDIT_REV1)->GetWindowTextLength(), GetDlgItem(IDC_EDIT_REV1)->GetWindowTextLength());//EDIT1是edit控件，首先要在属性里multiline设置为true,并把Horizen Scroll 和veritcal Scroll设置为true
	((CEdit*)GetDlgItem(IDC_EDIT_REV1))->ReplaceSel((LPCTSTR)(CString)s);
	m_recv1.LineScroll(m_recv1.GetLineCount());//m_caeraedit1是edit变量
	return 0;
}


#if 0
LRESULT set::OnCommunication1(WPARAM ch, LPARAM port)
{
	char s[2] = { ch, 0 }; //s是edit里追加的字符串，是字符串！
	((CEdit*)GetDlgItem(IDC_EDITREV2))->SetSel(GetDlgItem(IDC_EDITREV2)->GetWindowTextLength(), GetDlgItem(IDC_EDITREV2)->GetWindowTextLength());//EDIT1是edit控件，首先要在属性里multiline设置为true,并把Horizen Scroll 和veritcal Scroll设置为true
	((CEdit*)GetDlgItem(IDC_EDITREV2))->ReplaceSel((LPCTSTR)(CString)s);
	m_recv2.LineScroll(m_recv2.GetLineCount());//m_caeraedit1是edit变量
	return 0;
}
#endif

void set::OnBnClickedButtonMovedown()
{
	// TODO:  在此添加控件通知处理程序代码
	g.act.FirstMotorStepRunCCWByCtrl();
}


void set::OnBnClickedButtonMoveup()
{
	// TODO:  在此添加控件通知处理程序代码
	
	g.act.FirstMotorStepRunCWByCtrl();
}


void set::OnBnClickedButtonBack()
{
	// TODO:  在此添加控件通知处理程序代码
	
	//if (d1000_get_outbit(FIRST_SRV))
	int res = read_input_state(MOTOR1_UP_LIMIT);
	cout << res << endl;
	return;
	if (g.test)
	{
		write_output(FIRST_SRV, ON);
		GetDlgItem(IDC_BUTTON_BACK)->SetWindowText(L"打开伺服");
		g.test = false;
	}
	else
	{
		write_output(FIRST_SRV, OFF);
		GetDlgItem(IDC_BUTTON_BACK)->SetWindowText(L"关闭伺服");
		g.test = true;
	}

		
	return;
	CString str = L"MS";
	if (str.Compare(L"MS"))
	{
		AfxMessageBox(L"ok");
	}
	else
	{
		AfxMessageBox(L"ng");
	}
}


void set::OnSelchangeComboseWise()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_spinwise.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_spinwise.GetLBText(nSel, str);

	if (L"正" == str)
		ini.m_Motor1Wise = 1;
	else if (L"反" == str)
		ini.m_Motor1Wise = -1;
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnBnClickedButtonComf()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ini.m_Motor1StepByContol = m_manpuls;
	ini.SaveParaFile(PARA_PRJ);
}


void set::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnTimer(nIDEvent);
}
