// ResetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CommunicationReader.h"
#include "ResetDlg.h"
#include "afxdialogex.h"


// ResetDlg �Ի���

IMPLEMENT_DYNAMIC(ResetDlg, CDialogEx)

ResetDlg::ResetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ResetDlg::IDD, pParent)
{

}

ResetDlg::~ResetDlg()
{
}

void ResetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ResetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &ResetDlg::OnBnClickedButtonReset)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// ResetDlg ��Ϣ�������


void ResetDlg::OnBnClickedButtonReset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	StartFirstBackThread();
}



bool ResetDlg::StartFirstBackThread()
{
	if (RunFirstBackThread) return false;
	RunFirstBackThread = AfxBeginThread(FirstBackThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return true;
}

UINT ResetDlg::FirstBackThread(LPVOID lParam)
{
	ResetDlg* pAction = (ResetDlg*)lParam;
	pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"��ԭ����...");
	int res = g.act.FisrtMotorBackToOrigin();

	switch (res)
	{
	case BACKTIMEOUT:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"��λʱ�䳬ʱ��"); break;
	case NoError:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"��ԭ��ɹ���");
		write_output(OUT_ALM, OFF);
		for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTOVER, ON);
		for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTING, OFF);
		break;
	case EMERGENGCY:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"�����˼�ͣ��ť��"); break;
	default:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"δ֪����"); break;
	}
	delete pAction->RunFirstBackThread;
	pAction->RunFirstBackThread = NULL;
	return res;
}


void ResetDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (RunFirstBackThread) return;
	CDialogEx::OnClose();
}
