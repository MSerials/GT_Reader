
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif









#ifndef ___GLOBAL____
#define ___GLOBAL____
//#include "ExString.h"
#include <mutex>          // std::mutex, std::lock_guard
#include <stdexcept>  
#include "include/Preferences.h"
#include "include/SerialPort.h"
#include "include/SerialPort1.h"
#include "xls.h"
#include "CommunicationReaderDlg.h"
#include "ActionCaculation.h"


//errrcode
#define THREADRUNNING -2

#define NoError 0
#define EMERGENGCY (1<<0)
#define MOTOR_TIME_OUT (1<<1)
#define MOTOR_ALM	   (1<<2)
#define SENOR_TIME_OUT (1<<3)
#define NOT_ALREADY	   (1<<4)
#define REC_TIME_OUT   (1<<5)
#define BACKTIMEOUT		(1<<6)

#define BUFFSIZE 8192

//#define TEST
//#define MYDEBUG

class _global__
{
	
public:
	_global__()
		: com_state1(false)
		, com_state2(false)
		, msg(NULL)
		, msg1(NULL)
		, row(1)
		, col(0)
		, evtMotorProcedure(FALSE, TRUE)
		, evtEmgencyStop(FALSE,TRUE)
		, evtStartRecord(FALSE,TRUE)
	{
		msg = (char*)malloc(BUFFSIZE);
		msg1 = (char*)malloc(BUFFSIZE);
		msgIndex = 0;
		msgIndex1 = 0;
		if (!msg || !msg1) { AfxMessageBox(L"�ڴ治�㣡"); exit(0); }
		Motor1IsUpside = false;
		Motor1IsDownSide = false;
		In_Old_bits = false;//��紥���ź�������
		In_New_bits = false;
		In_Old_bits_emergency = false;
		In_New_bits_emergency = false;
		RunDisable = 0;//���в������
		FirstSrvReady = 0;
		//true��ʱ���ʾ�Ѿ��������һ���ź���
		isReciveOver = false;
		isReciveOver1 = false;
		isRecord = false;
		//hCamHand1 = CreateEvent(NULL, FALSE, TRUE, NULL);
		//hCamHand2 = CreateEvent(NULL, FALSE, TRUE, NULL);
		//hCamHand3 = CreateEvent(NULL, FALSE, TRUE, NULL);
		//hCamHand4 = CreateEvent(NULL, FALSE, TRUE, NULL);

		//ResetEvent(hCamHand1);
		//ResetEvent(hCamHand2);
		//ResetEvent(hCamHand3);
		//ResetEvent(hCamHand4);
	}
	~_global__()
	{
		if (msg != NULL)
			free(msg);
		if (msg1 != NULL)
			free(msg1);
	}
	//��Ӧ���������
#define SENSOR_MAX 4
#define SENSOR_DATA_STATRT 2

	int row;
	int col;

	int row1;
	int col1;

	bool com_state1;
	bool com_state2;

	char*   msg;
	char*	msg1;

	int msgIndex;
	bool isReciveOver;
	std::mutex mtx;	//���ڴ���1����
	int msgIndex1;
	bool isReciveOver1;
	std::mutex mtx1;	//���ڴ���2����

	HANDLE hCamHand1; //�ȴ���һ�������������
	HANDLE hCamHand2; //�ȴ���һ�������������
	HANDLE hCamHand3; //�ȴ���һ�������������
	HANDLE hCamHand4; //�ȴ���һ�������������

	CString model_path;

	CEvent evtMotorProcedure;
	CEvent evtEmgencyStop;
	CEvent evtStartRecord;//���õ����Ӧ���½����������������Ƶ�ģʽ����record������֤�������ֻ��¼һ��!




	//�ŷ����
	Action act;

	//���±�����ȫ�ֹ����������漰���ܶ�ط�ʹ��
	bool Motor1IsUpside;
	bool Motor1IsDownSide;
	bool isRecord;
	bool In_Old_bits;//��紥���ź�������
	bool In_New_bits;

	bool In_Old_bits_emergency;
	bool In_New_bits_emergency;

	int RunDisable;
	int FirstSrvReady;//�ŷ�1׼����


	bool test;


};
extern _global__ g;
extern CSerialPort Port1;
extern CSerialPort1 Port2;
extern CPreferences ini;
extern Xls xls;

extern CCommunicationReaderDlg *pComReaderDlg;


#define COM1T1 0
#define COM1T2 1
#define COM2T1 2
#define COM2T2 3
#define COMT  -1

#define CR "\r"
#define LF "\n"
#define CRLF "\r\n"








//#define _GX //GXϵ��
#define _GT//GT��Ӧ��

//#define MYDEBUG

#define STEPVERSION
//#define DOWNLIMITVERSION


#endif // !1
