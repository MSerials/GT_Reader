
// stdafx.cpp : 只包括标准包含文件的源文件
// CommunicationReader.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

struct Global
{
	int row;
	int col;

	bool com_state1;
	bool com_state2;
};


_global__ g;
CSerialPort Port1;
CSerialPort1 Port2;
CPreferences ini;
Xls xls;

CCommunicationReaderDlg *pComReaderDlg = NULL;