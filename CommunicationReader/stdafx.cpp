
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// CommunicationReader.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

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