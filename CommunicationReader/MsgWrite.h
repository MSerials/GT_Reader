#pragma once

#include "stdafx.h"
#include <vector>

#if !defined(__MSG__INDEX__)
#define __MSG__INDEX__



enum //索引用
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
	int index; //索引序列
	CString str;//字符
};

class MsgData
{
public:
	MsgData(){ initdata(); };
	virtual ~MsgData(){};
public:
	int Camera;//哪个摄像机

	bool flags;//是否正常,true-正常；false-异常

	std::vector<CString> vCStr; //保存分隔后的字符串

	//包含了索引，从里面查到相关字符所需要的
	vector<tag_MSG_INDEX> msgindex;

	//分割字符串
	void cutString(char *str, std::vector<CString> &cstr, char sign = ',');

	//打印错误
	void println(const std::vector<CString> &vcchar);

	//初始化msgindex表
	void initdata();

	//解码信息
	void DecodeMsg()
	{
		return;
	}
};


//消息队列
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


//变量声明
extern int MsgIndex_Offset;

//记录条数//初始化为0
extern int Com1T1Cnt;
extern int Com1T2Cnt;
extern int Com2T1Cnt;
extern int Com2T2Cnt;

extern const int TIME_ROWOFFSET;
extern const int TIME_COLOFFSET;

//当前Excel文件名
extern CString CurXlsFileName;

class ExpData
{
public:
	int Camera;//哪个摄像机
	bool flags;//是否正常,true-正常；false-异常
	std::vector<CString> vCStr; //保存分隔后的字符串
};

ExpData MsgStoreInFile(const char *p);

/*
*功能：处理分隔后的消息字符串，合成一串，用于显示在控件上
*参数：
*1、vCStr分隔后的字符串向量
*返回值：合成的字符串
*/
CString DealvCstr(const std::vector<CString> &vCStr);

void ExcelFormat();
void ExcelFormat1();

/*增加Sheet并写入No，测量项目等*/ //放入Xls::InIt()中对应的位置
void InitSheet();

/*写入时间*/ //放入CCommunicationReaderDlg::OnClose()中
void WriteTime();

/*工具函数*/
/*
功能：逗号分隔符，分割字符串
参数：
str，输入的字符串；vCStr，结果存储；sign，分割标记
返回值：无  
*/
void cutString(char *str, std::vector<CString> &cstr, char sign = ',');
/*
功能：根据消息的第二个字符，确认消息的类型，在某个COMx接受处，应将T1转为T3，T2转为T4
参数：
p，输入的消息指针；
返回值：消息类型
*/
int ChkMsgSource(const char *p);

//注意将SaveExcel()中的Workbook.put_Saved(true)，改为Workbook.Save();
//注意将另一个COM口的T1改为T3，T2改为T4;
#endif 