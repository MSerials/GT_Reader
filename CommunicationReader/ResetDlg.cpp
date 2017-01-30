// ResetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommunicationReader.h"
#include "ResetDlg.h"
#include "afxdialogex.h"


// ResetDlg 对话框

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


// ResetDlg 消息处理程序


void ResetDlg::OnBnClickedButtonReset()
{
	// TODO:  在此添加控件通知处理程序代码
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
	pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"回原点中...");
	int res = g.act.FisrtMotorBackToOrigin();

	switch (res)
	{
	case BACKTIMEOUT:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"复位时间超时！"); break;
	case NoError:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"回原点成功！");
		write_output(OUT_ALM, OFF);
		for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTOVER, ON);
		for (int i = 0; i<MAX_COUNTER; i++) write_output(DETECTING, OFF);
		break;
	case EMERGENGCY:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"按下了急停按钮！"); break;
	default:pAction->GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(L"未知错误"); break;
	}
	delete pAction->RunFirstBackThread;
	pAction->RunFirstBackThread = NULL;
	return res;
}


void ResetDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (RunFirstBackThread) return;
	CDialogEx::OnClose();
}
