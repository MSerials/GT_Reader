/*
**	�ļ���			CSerialPort.h
**  created  by lipeng 2010-9-12  in GW Corperation
**  functions :������Ҫʹ�� windows�ĵײ�API�������Դ��ڽ����˷�װ�����ڴ���ͨ�ţ���������
**  ��ѡ��ʹ����Ϣ���ƻ�ֱ�ӷ��ʵķ�ʽ��Ӧ�ó��򽻻��������û�ֻ��Ҫʵ���û��Զ������Ϣ�͵��ô������
**  public �������Դ��ڲ���
*/

#ifndef __SERIALPORT_H__1
#define __SERIALPORT_H__1
#include "../stdafx.h"
#pragma  warning(disable: 4996)
/*����Ϊ�û��Զ������Ϣ���� */
#define WM_COMM_BREAK_DETECTED1		WM_USER+11	// 
#define WM_COMM_CTS_DETECTED1		WM_USER+12	// 
#define WM_COMM_DSR_DETECTED1		WM_USER+13	// 
#define WM_COMM_ERR_DETECTED1		WM_USER+14	// 
#define WM_COMM_RING_DETECTED1		WM_USER+15	//
#define WM_COMM_RLSD_DETECTED1		WM_USER+16	// 
#define WM_COMM_RXCHAR1				WM_USER+17	//
#define WM_COMM_RXFLAG_DETECTED1	WM_USER+18	// 
#define WM_COMM_TXEMPTY_DETECTED1	WM_USER+19	// 


class CSerialPort1
{														 
public:
	CSerialPort1();
	virtual		~CSerialPort1();

	/*���ڳ�ʼ��*/										
	BOOL		InitPort(CWnd* pPortOwner,               //the owner (CWnd) of the port (receives message)
						 const char* portn, //const char* portnr="COM1",      //portnumber (1..4)
						 UINT baud=9600,                 //baudrate
					  	 const char* parity="NONE",      //parity
						 UINT databits=8,                //databits
						 const char* stopsbits="1",      //stopbits
						 DWORD dwCommEvents=EV_RXCHAR,  //EV_RXCHAR, EV_CTS etc
						 UINT nWriteBufferSize=512,      //size to the writebuffer
						 UINT nReadBufferSize=512);      //size to the readbuffer
    
	
	BOOL		StartMonitoring();   /*��ʼ���ڼ��� */
	BOOL		RestartMonitoring(); /*���¿�ʼ���ڼ���*/
	BOOL		StopMonitoring();    /*ֹͣ���ڼ���*/
	
	DWORD		GetWriteBufferSize();/*���д����������*/
	DWORD       GetReadBufferSize(); /*����û��Զ���Ľ������ݻ������Ĵ�С*/
	BOOL        GetReadBufferData(BYTE *ch,DWORD length);  /*��ý��ջ�����������*/
	DWORD		GetMonitorEvents();	 /*����¼�*/
	DWORD		GetCommPort();		 /*��ö˿ں�*/
	DCB			GetDCB();			 /*���dcb�ṹ��*/
	HANDLE		GetCommHandle();	 /*��ô��ڲ������*/
    CWnd*       GetWindowOwner();    /*��ô��ڵĴ���ӵ����*/

	BOOL        SetBaudRate(DWORD rate);  /*���ò�����*/
	BOOL        SetParity(const char* ch);      /*������żУ��*/
	BOOL        SetStopbit(const char* ch);     /*����ֹͣλ*/
	BOOL        SetDatabit(BYTE bits);    /*��������λ*/
	BOOL        SetWriteBufSize(DWORD len);   /*���û������Ĵ�С*/
	BOOL		SetReadBufSize(DWORD length);
	BOOL		SetMonitorEvent(DWORD);   /*���ô��ڼ����¼�*/
	BOOL		ClearReadBuffer();		//�����������
	void        EnableSerialMessage(BOOL EnRxchar=FALSE,BOOL EnCts=FALSE,BOOL EnRxFlag=FALSE,\
									BOOL EnBreak=FALSE,BOOL EnErr=FALSE,BOOL EnRing=FALSE);
	void        Close();				  /*�رն˿� �������߳�*/
  
	static UINT	CommThreadProc(LPVOID pParam); /*�̴߳�������*/
	BOOL		WriteToPort(char* string,DWORD length);/*�򴮿�д��Ϣ*/

	static BYTE*  msgdata;

protected:
	// protected memberfunctions
	void			ProcessErrorMessage(char* ErrorText);
	static void	    ReceiveChar(CSerialPort1* port);
    
	HANDLE				m_Thread;                     //�߳̾��

	CRITICAL_SECTION	m_csCommunicationSync;		  //�������ٽ���
	BOOL				m_bThreadAlive;

	HANDLE				m_hComm;            //����Comm�ڵľ��
	COMMTIMEOUTS		m_CommTimeouts;		//��ʱ��Ϣ����
	DCB					m_dcb;
	CWnd*				m_pOwner;          // owner window
    OVERLAPPED			m_ovWrite;		   //����д�ص��ṹ��
	OVERLAPPED          m_ovRead;          //���¼�
	OVERLAPPED			m_ovCommEv;        //���ڵȴ��¼�
private:
	UINT				m_nPortNr;          /*ͨ�ŵĶ˿ں�*/
	char*				m_szWriteBuffer;    /*д������*/
	DWORD				m_dwCommEvents;     /*��ͨ���豸���ӵ��¼�����*/
	DWORD				m_nWriteBufferSize; /*д����������*/

	DWORD				m_dwReadCount;       /*��ָ��*/
	BYTE*				m_szReadBuffer;     /*��������*/
	DWORD				m_nReadBufferSize;  //����������С

	BOOL				m_bWriteIsAlive;    //����д��־��
	BOOL				m_bReadIsAlive;     //���ڶ���־ 
/*************************************************************
** һ��Ϊ�û�����Ĵ�����Ϣ������ΪTRUEʱ��ʾ�������߳̽������Ĵ���ӵ���ŷ�����Щ������Ϣ��
** ��ʱ�û����ڴ�����ʵ���Զ���Ĵ�����Ϣӳ�䣬���򵱳�ʼ�����ڼ�����Щ��Ϣʱ����������
** ��Ĭ��״���£���Щ��Ϣ������ֹ����ͨ������EnableSerialMessage()������ʹ�ܻ��ֹ��Щ��Ϣ
***************************************************************/
private:
	BOOL m_evRxchar;
	BOOL m_evCts;
	BOOL m_evRxFlag;
	BOOL m_evBreak;
	BOOL m_evErr;
	BOOL m_evRing;

private:
	UINT parityAnalyze(const char* ch);
	UINT stopbitAnalyze(const char* ch);
	BOOL userBuildCommDCB(DCB* dcb,UINT baud,UINT parity,UINT stopbit,UINT databit);/*�û��Զ��彨��DCB��*/




};

#endif __SERIALPORT_H__

