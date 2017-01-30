#include "stdafx.h"
#include "MsgWrite.h"

//消息内容开始处的索引
//#define MSGSTARTINDEX 3




void MsgData::cutString(char *str, std::vector<CString> &cstr, char sign)
{
	char *s = str, *d = str;
	while ('\0' != *d)
	{
		if (sign == *d)
		{
			*d = '\0';
			cstr.push_back(CString(s));
			s = ++d;
			continue;
		}
		d++;
	}
	//cstr.push_back(CString(s));//最后的E舍弃
}


void MsgData::println(const std::vector<CString> &vcchar)
{
	typedef std::vector<CString> MyType;
	for (MyType::const_iterator it = vcchar.begin(); it != vcchar.end(); ++it)
		printf("%s\n", (LPCSTR)CStringA(*it));
}

void MsgData::initdata()
{
	CString str[] = { L"LD", L"RD", L"CD", L"CLL", L"CLR", L"YGJ", L"QXL", L"YHBJ" };
	for (int i = 0; i < 8; i++) {  //8要和str 和 enum里同步
		tag_MSG_INDEX idx;
		idx.index = i;
		idx.str = str[i];
		msgindex.push_back(idx);
	}
}


int MsgIndex_Offset;
/*
定义所需的全局变量
*/
//存储Tx后的字符串//需足够大
char tmpMsg[200];	

//记录条数//初始化为0
int Com1T1Cnt = 0;
int Com1T2Cnt = 0;
int Com2T1Cnt = 0;
int Com2T2Cnt = 0;

//当前Excel文件名
CString CurXlsFileName;


//用于与返回的测量值好坏的标记比较
const CString OK(L"0");
const CString NG(L"1");

//标记零件好坏写入文件的值
const CString FOK(L"OK");
const CString FNG(L"NG");

//与ComxTxCnt的偏差值，用于预留Excel前面offset行
const int offset = 2;
//测量项对应列坐标，行坐标由记录条数ComTxTxCnt与offset确定
#define COL 1
//设置offset和COL，即可决定表格左上角的位置


#define COLSTART (COL + 3)
//const int JUDGECol(COLSTART - 1);
const int TIMECol(COLSTART - 1);
const int YGJCol(COLSTART + 0);
const int QXLCol(COLSTART + 2);
const int YHBJCol(COLSTART + 4);
const int LDCol(COLSTART +6);
const int RDCol(COLSTART + 8);
const int CDCol(COLSTART + 10);
const int CLLCol(COLSTART + 12);
const int CLRCol(COLSTART + 14);


const int filerow = offset;
const int filecol = COL;

//最后写入时间行列与记录条数加上前面预留行数之和的偏差
const int TIME_ROWOFFSET(5 + offset);
const int TIME_COLOFFSET(2);

void println(const std::vector<CString> &vcchar)
{
	typedef std::vector<CString> MyType;
	for (MyType::const_iterator it = vcchar.begin(); it != vcchar.end(); ++it)
		printf("%s\n", (LPCSTR)CStringA(*it));
} 
/*
功能：逗号分隔符，分割字符串
参数：
str，输入的字符串；vCStr，结果存储；sign，分割标记
返回值：无
*/
void cutString(char *str, std::vector<CString> &cstr, char sign)
{
	char *s = str, *d = str;
	while ('\0' != *d)
	{
		if (sign == *d)
		{
			*d = '\0';
			cstr.push_back(CString(s));
			s = ++d;
			continue;
		}
		d++;
	}
	//cstr.push_back(CString(s));//最后的E舍弃
}

/*
功能：根据消息的第二个字符，确认消息的类型，在某个COMx接受处，应将T1转为T3，T2转为T4
参数：
p，输入的消息指针；
返回值：消息类型
*/

int ChkMsgSource(const char *p)
{
	int res;
	if (p[1] == '1')
		res = COM1T1;
	else if (p[1] == '2')
		res = COM1T2;
	else if (p[1] == '3')
		res = COM2T1;
	else if (p[1] == '4')
		res = COM2T2;
	else
		res = COMT;

	return res;
}

//函数声明
int SaveInFile(const std::vector<CString> &vCStr, int count);

/*
功能：将消息存入Excel存入文件
参数：
p，消息指针
返回值：成员Camera，指示哪个相机，与COMxTx对应；
成员flags指示是否正常，true-正常，false-异常；
成员vCStr保存消息字符串分隔后的结果
*/

ExpData MsgStoreInFile(const char *p)
{
	int normal;
	ExpData expdata;
	//取出标头后的字符串存于tmpMsg，保持原始信息
	strcpy_s(tmpMsg, p + MsgIndex_Offset);/*软件发送Tx出发的值和硬件上出发的值不一样*/

#ifdef MYDEBUG
	printf("tmpMsg = %s\n",tmpMsg);
#endif

	std::vector<CString> vCStr;//用于保存分割后的字符串
	//根据逗号分割字符串，结果存于vCStr
	cutString(tmpMsg, vCStr, ',');//vCStr以引用传递

	expdata.vCStr = vCStr; //存入expdata，函数返回后继续处理

	//确认消息类型
	int cls = ChkMsgSource(p);
	//CString time = xls.GetDate() + xls.GetTime();//获取时间
	CString time = xls.GetTime();//获取时间
	switch (cls)
	{
	case COM1T1:
		//将vCStr中的结果存入Excel
		++Com1T1Cnt;//记录条数加1
		xls.SelectSheet(1);
		xls.SetCellValue(Com1T1Cnt + offset, COL, Com1T1Cnt);
		xls.SetCellValue(Com1T1Cnt + offset, COL + 1, CString(L"样品1"));
		normal = SaveInFile(vCStr, Com1T1Cnt);

		/*写入单次测量时间*/
		xls.SetCellValue(Com1T1Cnt + offset, TIMECol, time);

		if (normal > 0) //零件有问题
		{
			expdata.Camera = COM1T1;
			expdata.flags = false;
#ifdef MYDEBUG
			printf("expdata.Camera = COM1T1;expdata.flags = false;\n");
#endif
		}
		else if (normal == 0)
		{
			expdata.Camera = COM1T1;
			expdata.flags = true;
		}
		break;
	case COM1T2:
		++Com1T2Cnt;
		xls.SelectSheet(2);
		xls.SetCellValue(Com1T2Cnt + offset, COL, Com1T2Cnt);
		xls.SetCellValue(Com1T2Cnt + offset, COL + 1, CString(L"样品2"));
		normal = SaveInFile(vCStr, Com1T2Cnt);

		/*写入单次测量时间*/
		xls.SetCellValue(Com1T2Cnt + offset, TIMECol, time);

		if (normal > 0) //零件有问题
		{
			expdata.Camera = COM1T2;
			expdata.flags = false;
#ifdef MYDEBUG
			printf("expdata.Camera = COM1T2;expdata.flags = false;\n");
#endif
		}
		else if (normal == 0)
		{
			expdata.Camera = COM1T2;
			expdata.flags = true;
		}
		break;
	case COM2T1:
		++Com2T1Cnt;
		xls.SelectSheet(3);
		xls.SetCellValue(Com2T1Cnt + offset, COL, Com2T1Cnt);
		xls.SetCellValue(Com2T1Cnt + offset, COL + 1, CString(L"样品3"));
		normal = SaveInFile(vCStr, Com2T1Cnt);

		/*写入单次测量时间*/
		xls.SetCellValue(Com2T1Cnt + offset, TIMECol, time);

		if (normal > 0) //零件有问题
		{
			expdata.Camera = COM2T1;
			expdata.flags = false;
#ifdef MYDEBUG
			printf("expdata.Camera = COM2T1;expdata.flags = false;\n");
#endif
		}
		else if (normal == 0)
		{
			expdata.Camera = COM2T1;
			expdata.flags = true;
		}
		break;
	case COM2T2:
		++Com2T2Cnt;
		xls.SelectSheet(4);
		xls.SetCellValue(Com2T2Cnt + offset, COL, Com2T2Cnt);
		xls.SetCellValue(Com2T2Cnt + offset, COL + 1, CString(L"样品4"));
		normal = SaveInFile(vCStr, Com2T2Cnt);

		/*写入单次测量时间*/
		xls.SetCellValue(Com2T2Cnt + offset, TIMECol, time);

		if (normal > 0) //零件有问题
		{
			expdata.Camera = COM2T2;
			expdata.flags = false;
#ifdef MYDEBUG
			printf("expdata.Camera = COM2T2;expdata.flags = false;\n");
#endif
		}
		else if (normal == 0)
		{
			expdata.Camera = COM2T2;
			expdata.flags = true;
		}
		break;
	default: //异常情况
		expdata.Camera = COMT;
		expdata.flags = false;
#ifdef MYDEBUG
		printf("异常情况\n");
#endif
		break;
	}
	return expdata;
}

int SaveInFile(const std::vector<CString> &vCStr, int count)
{
	int total = 0;//用于判断零件测量值的好坏 //若大于零，则零件有问题。
	CString cstr;
	int index;
#ifdef TEST
	println(vCStr);
#endif
	for (std::vector<CString>::const_iterator it = vCStr.begin(); it != vCStr.end(); it = it + 2)//须保证cCStr的大小为偶数
	{

		index = it->Find(LD);
		if (index >= 0)
		{
			cstr = it->Mid(index + LD.GetLength());
			xls.SetCellValue(count + offset, LDCol, cstr);
			/*根据发回处理后的数据，写入状态*/
			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset,  LDCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, LDCol + 1, FNG);
				++total;
			}
		}

		index = it->Find(RD);
		if (index >= 0)
		{
			cstr = it->Mid(index + RD.GetLength());
			xls.SetCellValue(count + offset, RDCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, RDCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, RDCol + 1, FNG);
				++total;
			}
		}

		index = it->Find(CD);
		if (index >= 0)
		{
			cstr = it->Mid(index + CD.GetLength());
			xls.SetCellValue(count + offset, CDCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, CDCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, CDCol + 1, FNG);
				++total;
			}
		}

		index = it->Find(CLL);
		if (index >= 0)
		{
			cstr = it->Mid(index + CLL.GetLength());
			xls.SetCellValue(count + offset, CLLCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, CLLCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, CLLCol + 1, FNG);
				++total;
			}
		}
		index = it->Find(CLR);
		if (index >= 0)
		{
			cstr = it->Mid(index + CLR.GetLength());
			xls.SetCellValue(count + offset, CLRCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, CLRCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, CLRCol + 1, FNG);
				++total;
			}
		}


		//
		index = it->Find(YGJ);
		if (index >= 0)
		{
			cstr = it->Mid(index + YGJ.GetLength());
			xls.SetCellValue(count + offset, YGJCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, YGJCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, YGJCol + 1, FNG);
				++total;
			}
		}


		index = it->Find(QXL);
		if (index >= 0)
		{
			cstr = it->Mid(index + QXL.GetLength());
			xls.SetCellValue(count + offset, QXLCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, QXLCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, QXLCol + 1, FNG);
				++total;
			}
		}

		index = it->Find(YHBJ);
		if (index >= 0)
		{
			cstr = it->Mid(index + YHBJ.GetLength());
			xls.SetCellValue(count + offset, YHBJCol, cstr);

			if (*(it + 1) == OK)
				xls.SetCellValue(count + offset, YHBJCol + 1, FOK);
			else if (*(it + 1) == NG)
			{
				xls.SetCellValue(count + offset, YHBJCol + 1, FNG);
				++total;
			}
		}
	}//end for

	return total;
}

/*
*功能：处理分隔后的消息字符串，合成一串，用于显示在控件上
*参数：

*1、vCStr分隔后的字符串向量
*返回值：合成的字符串
*/
CString DealvCstr(const std::vector<CString> &vCStr)
{
	int index;
	CString cstr;
	CString rescstr;

	for (std::vector<CString>::const_iterator it = vCStr.begin(); it != vCStr.end(); it = it + 2)//须保证cCStr的大小为偶数
	{
		index = it->Find(LD);
		if (index >= 0) //找到头
		{
			cstr = it->Mid(index + LD.GetLength());//截取字符串，去除头部
			if (*(it + 1) == OK)
			{
				rescstr += L"圆径001 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"圆径001 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(RD);
		if (index >= 0)
		{
			cstr = it->Mid(index + RD.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"圆径002 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"圆径002 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(CD);
		if (index >= 0)
		{
			cstr = it->Mid(index + CD.GetLength());

			if (*(it + 1) == OK)
			{
				rescstr += L"孔距[中心] = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"孔距[中心] = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(CLL);
		if (index >= 0)
		{
			cstr = it->Mid(index + CLL.GetLength());

			if (*(it + 1) == OK)
			{
				rescstr += L"圆-线距离001[中心] = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"圆-线距离001[中心] = " + cstr + L", " + FNG + L"\r\n";
			}
		}
		index = it->Find(CLR);
		if (index >= 0)
		{
			cstr = it->Mid(index + CLR.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"圆-线距离002[中心] = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"圆-线距离002[中心] = " + cstr + L", " + FNG + L"\r\n";
			}
		}
		//add
		index = it->Find(YGJ);
		if (index >= 0)
		{
			cstr = it->Mid(index + YGJ.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"鹰勾角 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"鹰勾角 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(QXL);
		if (index >= 0)
		{
			cstr = it->Mid(index + QXL.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"切削量 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"切削量 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(YHBJ);
		if (index >= 0)
		{
			cstr = it->Mid(index + YHBJ.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"圆弧半径 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"圆弧半径 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

	}//end for
	return rescstr;
}

void ExcelFormat();
void ExcelFormat1();

//放入Xls::InIt()中对应的位置
void InitSheet()
{
	xls.SelectSheet(1);
	ExcelFormat();
	return;
	xls.AddWorkSheet();
	xls.AddWorkSheet();
	xls.AddWorkSheet();


	xls.SelectSheet(2);
	ExcelFormat1();
	xls.SelectSheet(3);
	ExcelFormat();
	xls.SelectSheet(4);
	ExcelFormat1();
	xls.SelectSheet(1);
}
void ExcelFormat()
{
	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"样品");
	xls.SetCellValue(filerow, filecol + 2, L"测量时间");
	xls.SetCellValue(filerow, filecol + 3, L"第一感应器");
	xls.SetCellValue(filerow, filecol + 4, L"判断");
	xls.SetCellValue(filerow, filecol + 5, L"第二感应器");
	xls.SetCellValue(filerow, filecol + 6, L"判断");
	xls.SetCellValue(filerow, filecol + 7, L"第三感应器");
	xls.SetCellValue(filerow, filecol + 8, L"判断");
	xls.SetCellValue(filerow, filecol + 9, L"第四感应器");
	xls.SetCellValue(filerow, filecol + 10, L"判断");
	return;
	xls.SetCellValue(filerow, filecol + 11, L"圆径002");
	xls.SetCellValue(filerow, filecol + 12, L"判断");
	xls.SetCellValue(filerow, filecol + 13, L"孔距[中心]");
	xls.SetCellValue(filerow, filecol + 14, L"判断");
	xls.SetCellValue(filerow, filecol + 15, L"圆-线距离003[中心]");
	xls.SetCellValue(filerow, filecol + 16, L"判断");
	xls.SetCellValue(filerow, filecol + 17, L"圆-线距离004[中心]");
	xls.SetCellValue(filerow, filecol + 18, L"判断");
	return;



	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"样品");
	xls.SetCellValue(filerow, filecol + 2, L"圆径001");
	xls.SetCellValue(filerow, filecol + 3, L"判断");
	xls.SetCellValue(filerow, filecol + 4, L"圆径002");
	xls.SetCellValue(filerow, filecol + 5, L"判断");
	xls.SetCellValue(filerow, filecol + 6, L"孔距[中心]");
	xls.SetCellValue(filerow, filecol + 7, L"判断");
	xls.SetCellValue(filerow, filecol + 8, L"单次测量时间");
}
void ExcelFormat1()
{
	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"样品");
	xls.SetCellValue(filerow, filecol + 2, L"测量时间");

	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"样品");
	xls.SetCellValue(filerow, filecol + 2, L"测量时间");
	xls.SetCellValue(filerow, filecol + 3, L"第一感应器");
	xls.SetCellValue(filerow, filecol + 4, L"判断");
	xls.SetCellValue(filerow, filecol + 5, L"第二感应器");
	xls.SetCellValue(filerow, filecol + 6, L"判断");
	xls.SetCellValue(filerow, filecol + 7, L"第三感应器");
	xls.SetCellValue(filerow, filecol + 8, L"判断");
	xls.SetCellValue(filerow, filecol + 9, L"第四感应器");
	xls.SetCellValue(filerow, filecol + 10, L"判断");
	return;






	xls.SetCellValue(filerow, filecol + 3, L"鹰勾角");
	xls.SetCellValue(filerow, filecol + 4, L"判断");
	xls.SetCellValue(filerow, filecol + 5, L"切削量");
	xls.SetCellValue(filerow, filecol + 6, L"判断");
	xls.SetCellValue(filerow, filecol + 7, L"圆弧半径");
	xls.SetCellValue(filerow, filecol + 8, L"判断");
	xls.SetCellValue(filerow, filecol + 9, L"圆径001");
	xls.SetCellValue(filerow, filecol + 10, L"判断");
	xls.SetCellValue(filerow, filecol + 11, L"圆径002");
	xls.SetCellValue(filerow, filecol + 12, L"判断");
	xls.SetCellValue(filerow, filecol + 13, L"孔距[中心]");
	xls.SetCellValue(filerow, filecol + 14, L"判断");
	xls.SetCellValue(filerow, filecol + 15, L"圆-线距离003[中心]");
	xls.SetCellValue(filerow, filecol + 16, L"判断");
	xls.SetCellValue(filerow, filecol + 17, L"圆-线距离004[中心]");
	xls.SetCellValue(filerow, filecol + 18, L"判断");

	return;
	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"样品");
	xls.SetCellValue(filerow, filecol + 2, L"圆径001");
	xls.SetCellValue(filerow, filecol + 3, L"判断");
	xls.SetCellValue(filerow, filecol + 4, L"圆径002");
	xls.SetCellValue(filerow, filecol + 5, L"判断");
	xls.SetCellValue(filerow, filecol + 6, L"孔距[中心]");
	xls.SetCellValue(filerow, filecol + 7, L"判断");
	
	xls.SetCellValue(filerow, filecol + 8, L"圆-线距离003[中心]");
	xls.SetCellValue(filerow, filecol + 9, L"判断");
	xls.SetCellValue(filerow, filecol + 10, L"圆-线距离004[中心]");
	xls.SetCellValue(filerow, filecol + 11, L"判断");

	xls.SetCellValue(filerow, filecol + 12, L"单次测量时间");
}

/*写入时间*/ //放入CCommunicationReaderDlg::OnClose()中
void WriteTime()
{

	CString time = xls.GetDate() + xls.GetTime();
	xls.SelectSheet(1);
	
#ifdef _GT
	xls.SetCellValue(g.row + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"文档关闭时间：") + time);
		return;
#endif
		xls.SetCellValue(Com1T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"文档关闭时间：") + time);
	xls.SelectSheet(2);
	xls.SetCellValue(Com1T2Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"文档关闭时间：") + time);

	xls.SelectSheet(3);
	xls.SetCellValue(Com2T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"文档关闭时间：") + time);

	xls.SelectSheet(4);
	xls.SetCellValue(Com2T2Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"文档关闭时间：") + time);
}