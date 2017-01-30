
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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
		if (!msg || !msg1) { AfxMessageBox(L"内存不足！"); exit(0); }
		Motor1IsUpside = false;
		Motor1IsDownSide = false;
		In_Old_bits = false;//外界触发信号上升沿
		In_New_bits = false;
		In_Old_bits_emergency = false;
		In_New_bits_emergency = false;
		RunDisable = 0;//运行不能情况
		FirstSrvReady = 0;
		//true的时候表示已经接收完毕一次信号了
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
	//感应器最大数量
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
	std::mutex mtx;	//用于串口1的锁
	int msgIndex1;
	bool isReciveOver1;
	std::mutex mtx1;	//用于串口2的锁

	HANDLE hCamHand1; //等待第一个相机发回数据
	HANDLE hCamHand2; //等待第一个相机发回数据
	HANDLE hCamHand3; //等待第一个相机发回数据
	HANDLE hCamHand4; //等待第一个相机发回数据

	CString model_path;

	CEvent evtMotorProcedure;
	CEvent evtEmgencyStop;
	CEvent evtStartRecord;//利用电机感应到下降后，利用上升沿类似的模式进行record，来保证电机下来只记录一次!




	//伺服电机
	Action act;

	//以下变量是全局工作变量，涉及到很多地方使用
	bool Motor1IsUpside;
	bool Motor1IsDownSide;
	bool isRecord;
	bool In_Old_bits;//外界触发信号上升沿
	bool In_New_bits;

	bool In_Old_bits_emergency;
	bool In_New_bits_emergency;

	int RunDisable;
	int FirstSrvReady;//伺服1准备好


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








//#define _GX //GX系列
#define _GT//GT感应器

//#define MYDEBUG

#define STEPVERSION
//#define DOWNLIMITVERSION


#endif // !1
