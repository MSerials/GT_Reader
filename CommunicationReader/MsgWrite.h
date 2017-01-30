#pragma once

#include "stdafx.h"
#include <vector>

#if !defined(__MSG__INDEX__)
#define __MSG__INDEX__



enum //������
{
	eLD, eRD, eCD, eCLL, eCLR, eYGJ, eQXL, eMAXINDEX
};

const CString LD(L"LD");
const CString RD(L"RD");
const CString CD(L"CD");
const CString CLL(L"CLL");
const CString CLR(L"CLR");
const CString YGJ(L"YGJ");//
const CString QXL(L"QXL");//
const CString YHBJ(L"YHBJ");

struct tag_MSG_INDEX
{
	int index; //��������
	CString str;//�ַ�
};

class MsgData
{
public:
	MsgData(){ initdata(); };
	virtual ~MsgData(){};
public:
	int Camera;//�ĸ������

	bool flags;//�Ƿ�����,true-������false-�쳣

	std::vector<CString> vCStr; //����ָ�����ַ���

	//������������������鵽����ַ�����Ҫ��
	vector<tag_MSG_INDEX> msgindex;

	//�ָ��ַ���
	void cutString(char *str, std::vector<CString> &cstr, char sign = ',');

	//��ӡ����
	void println(const std::vector<CString> &vcchar);

	//��ʼ��msgindex��
	void initdata();

	//������Ϣ
	void DecodeMsg()
	{
		return;
	}
};


//��Ϣ����
class CMsgQueue
{
public:
	CMsgQueue(const char *pName = NULL);
	~CMsgQueue();
	//revice data from message queue  
	virtual bool recvMsg(unsigned int &m_msg_code, void *&p_msg) = 0;
	//send data to message queue  
	virtual bool sendMsg(unsigned int m_msg_code, void *p_msg) = 0;
	const char * getName(void) const {
		return msg_queue_name;
	}
private:
	char *msg_queue_name;
}; 


//��������
extern int MsgIndex_Offset;

//��¼����//��ʼ��Ϊ0
extern int Com1T1Cnt;
extern int Com1T2Cnt;
extern int Com2T1Cnt;
extern int Com2T2Cnt;

extern const int TIME_ROWOFFSET;
extern const int TIME_COLOFFSET;

//��ǰExcel�ļ���
extern CString CurXlsFileName;

class ExpData
{
public:
	int Camera;//�ĸ������
	bool flags;//�Ƿ�����,true-������false-�쳣
	std::vector<CString> vCStr; //����ָ�����ַ���
};

ExpData MsgStoreInFile(const char *p);

/*
*���ܣ�����ָ������Ϣ�ַ������ϳ�һ����������ʾ�ڿؼ���
*������
*1��vCStr�ָ�����ַ�������
*����ֵ���ϳɵ��ַ���
*/
CString DealvCstr(const std::vector<CString> &vCStr);

void ExcelFormat();
void ExcelFormat1();

/*����Sheet��д��No��������Ŀ��*/ //����Xls::InIt()�ж�Ӧ��λ��
void InitSheet();

/*д��ʱ��*/ //����CCommunicationReaderDlg::OnClose()��
void WriteTime();

/*���ߺ���*/
/*
���ܣ����ŷָ������ָ��ַ���
������
str��������ַ�����vCStr������洢��sign���ָ���
����ֵ����  
*/
void cutString(char *str, std::vector<CString> &cstr, char sign = ',');
/*
���ܣ�������Ϣ�ĵڶ����ַ���ȷ����Ϣ�����ͣ���ĳ��COMx���ܴ���Ӧ��T1תΪT3��T2תΪT4
������
p���������Ϣָ�룻
����ֵ����Ϣ����
*/
int ChkMsgSource(const char *p);

//ע�⽫SaveExcel()�е�Workbook.put_Saved(true)����ΪWorkbook.Save();
//ע�⽫��һ��COM�ڵ�T1��ΪT3��T2��ΪT4;
#endif 