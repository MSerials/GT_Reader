
// CommunicationReader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCommunicationReaderApp: 
// �йش����ʵ�֣������ CommunicationReader.cpp
//

class CCommunicationReaderApp : public CWinApp
{
public:
	CCommunicationReaderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCommunicationReaderApp theApp;