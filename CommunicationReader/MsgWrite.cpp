#include "stdafx.h"
#include "MsgWrite.h"

//��Ϣ���ݿ�ʼ��������
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
	//cstr.push_back(CString(s));//����E����
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
	for (int i = 0; i < 8; i++) {  //8Ҫ��str �� enum��ͬ��
		tag_MSG_INDEX idx;
		idx.index = i;
		idx.str = str[i];
		msgindex.push_back(idx);
	}
}


int MsgIndex_Offset;
/*
���������ȫ�ֱ���
*/
//�洢Tx����ַ���//���㹻��
char tmpMsg[200];	

//��¼����//��ʼ��Ϊ0
int Com1T1Cnt = 0;
int Com1T2Cnt = 0;
int Com2T1Cnt = 0;
int Com2T2Cnt = 0;

//��ǰExcel�ļ���
CString CurXlsFileName;


//�����뷵�صĲ���ֵ�û��ı�ǱȽ�
const CString OK(L"0");
const CString NG(L"1");

//�������û�д���ļ���ֵ
const CString FOK(L"OK");
const CString FNG(L"NG");

//��ComxTxCnt��ƫ��ֵ������Ԥ��Excelǰ��offset��
const int offset = 2;
//�������Ӧ�����꣬�������ɼ�¼����ComTxTxCnt��offsetȷ��
#define COL 1
//����offset��COL�����ɾ���������Ͻǵ�λ��


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

//���д��ʱ���������¼��������ǰ��Ԥ������֮�͵�ƫ��
const int TIME_ROWOFFSET(5 + offset);
const int TIME_COLOFFSET(2);

void println(const std::vector<CString> &vcchar)
{
	typedef std::vector<CString> MyType;
	for (MyType::const_iterator it = vcchar.begin(); it != vcchar.end(); ++it)
		printf("%s\n", (LPCSTR)CStringA(*it));
} 
/*
���ܣ����ŷָ������ָ��ַ���
������
str��������ַ�����vCStr������洢��sign���ָ���
����ֵ����
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
	//cstr.push_back(CString(s));//����E����
}

/*
���ܣ�������Ϣ�ĵڶ����ַ���ȷ����Ϣ�����ͣ���ĳ��COMx���ܴ���Ӧ��T1תΪT3��T2תΪT4
������
p���������Ϣָ�룻
����ֵ����Ϣ����
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

//��������
int SaveInFile(const std::vector<CString> &vCStr, int count);

/*
���ܣ�����Ϣ����Excel�����ļ�
������
p����Ϣָ��
����ֵ����ԱCamera��ָʾ�ĸ��������COMxTx��Ӧ��
��Աflagsָʾ�Ƿ�������true-������false-�쳣��
��ԱvCStr������Ϣ�ַ����ָ���Ľ��
*/

ExpData MsgStoreInFile(const char *p)
{
	int normal;
	ExpData expdata;
	//ȡ����ͷ����ַ�������tmpMsg������ԭʼ��Ϣ
	strcpy_s(tmpMsg, p + MsgIndex_Offset);/*�������Tx������ֵ��Ӳ���ϳ�����ֵ��һ��*/

#ifdef MYDEBUG
	printf("tmpMsg = %s\n",tmpMsg);
#endif

	std::vector<CString> vCStr;//���ڱ���ָ����ַ���
	//���ݶ��ŷָ��ַ������������vCStr
	cutString(tmpMsg, vCStr, ',');//vCStr�����ô���

	expdata.vCStr = vCStr; //����expdata���������غ��������

	//ȷ����Ϣ����
	int cls = ChkMsgSource(p);
	//CString time = xls.GetDate() + xls.GetTime();//��ȡʱ��
	CString time = xls.GetTime();//��ȡʱ��
	switch (cls)
	{
	case COM1T1:
		//��vCStr�еĽ������Excel
		++Com1T1Cnt;//��¼������1
		xls.SelectSheet(1);
		xls.SetCellValue(Com1T1Cnt + offset, COL, Com1T1Cnt);
		xls.SetCellValue(Com1T1Cnt + offset, COL + 1, CString(L"��Ʒ1"));
		normal = SaveInFile(vCStr, Com1T1Cnt);

		/*д�뵥�β���ʱ��*/
		xls.SetCellValue(Com1T1Cnt + offset, TIMECol, time);

		if (normal > 0) //���������
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
		xls.SetCellValue(Com1T2Cnt + offset, COL + 1, CString(L"��Ʒ2"));
		normal = SaveInFile(vCStr, Com1T2Cnt);

		/*д�뵥�β���ʱ��*/
		xls.SetCellValue(Com1T2Cnt + offset, TIMECol, time);

		if (normal > 0) //���������
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
		xls.SetCellValue(Com2T1Cnt + offset, COL + 1, CString(L"��Ʒ3"));
		normal = SaveInFile(vCStr, Com2T1Cnt);

		/*д�뵥�β���ʱ��*/
		xls.SetCellValue(Com2T1Cnt + offset, TIMECol, time);

		if (normal > 0) //���������
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
		xls.SetCellValue(Com2T2Cnt + offset, COL + 1, CString(L"��Ʒ4"));
		normal = SaveInFile(vCStr, Com2T2Cnt);

		/*д�뵥�β���ʱ��*/
		xls.SetCellValue(Com2T2Cnt + offset, TIMECol, time);

		if (normal > 0) //���������
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
	default: //�쳣���
		expdata.Camera = COMT;
		expdata.flags = false;
#ifdef MYDEBUG
		printf("�쳣���\n");
#endif
		break;
	}
	return expdata;
}

int SaveInFile(const std::vector<CString> &vCStr, int count)
{
	int total = 0;//�����ж��������ֵ�ĺû� //�������㣬����������⡣
	CString cstr;
	int index;
#ifdef TEST
	println(vCStr);
#endif
	for (std::vector<CString>::const_iterator it = vCStr.begin(); it != vCStr.end(); it = it + 2)//�뱣֤cCStr�Ĵ�СΪż��
	{

		index = it->Find(LD);
		if (index >= 0)
		{
			cstr = it->Mid(index + LD.GetLength());
			xls.SetCellValue(count + offset, LDCol, cstr);
			/*���ݷ��ش��������ݣ�д��״̬*/
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
*���ܣ�����ָ������Ϣ�ַ������ϳ�һ����������ʾ�ڿؼ���
*������

*1��vCStr�ָ�����ַ�������
*����ֵ���ϳɵ��ַ���
*/
CString DealvCstr(const std::vector<CString> &vCStr)
{
	int index;
	CString cstr;
	CString rescstr;

	for (std::vector<CString>::const_iterator it = vCStr.begin(); it != vCStr.end(); it = it + 2)//�뱣֤cCStr�Ĵ�СΪż��
	{
		index = it->Find(LD);
		if (index >= 0) //�ҵ�ͷ
		{
			cstr = it->Mid(index + LD.GetLength());//��ȡ�ַ�����ȥ��ͷ��
			if (*(it + 1) == OK)
			{
				rescstr += L"Բ��001 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"Բ��001 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(RD);
		if (index >= 0)
		{
			cstr = it->Mid(index + RD.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"Բ��002 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"Բ��002 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(CD);
		if (index >= 0)
		{
			cstr = it->Mid(index + CD.GetLength());

			if (*(it + 1) == OK)
			{
				rescstr += L"�׾�[����] = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"�׾�[����] = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(CLL);
		if (index >= 0)
		{
			cstr = it->Mid(index + CLL.GetLength());

			if (*(it + 1) == OK)
			{
				rescstr += L"Բ-�߾���001[����] = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"Բ-�߾���001[����] = " + cstr + L", " + FNG + L"\r\n";
			}
		}
		index = it->Find(CLR);
		if (index >= 0)
		{
			cstr = it->Mid(index + CLR.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"Բ-�߾���002[����] = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"Բ-�߾���002[����] = " + cstr + L", " + FNG + L"\r\n";
			}
		}
		//add
		index = it->Find(YGJ);
		if (index >= 0)
		{
			cstr = it->Mid(index + YGJ.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"ӥ���� = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"ӥ���� = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(QXL);
		if (index >= 0)
		{
			cstr = it->Mid(index + QXL.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"������ = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"������ = " + cstr + L", " + FNG + L"\r\n";
			}
		}

		index = it->Find(YHBJ);
		if (index >= 0)
		{
			cstr = it->Mid(index + YHBJ.GetLength());
			if (*(it + 1) == OK)
			{
				rescstr += L"Բ���뾶 = " + cstr + L", " + FOK + L"\r\n";
			}
			else if (*(it + 1) == NG)
			{
				rescstr += L"Բ���뾶 = " + cstr + L", " + FNG + L"\r\n";
			}
		}

	}//end for
	return rescstr;
}

void ExcelFormat();
void ExcelFormat1();

//����Xls::InIt()�ж�Ӧ��λ��
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
	xls.SetCellValue(filerow, filecol + 1, L"��Ʒ");
	xls.SetCellValue(filerow, filecol + 2, L"����ʱ��");
	xls.SetCellValue(filerow, filecol + 3, L"��һ��Ӧ��");
	xls.SetCellValue(filerow, filecol + 4, L"�ж�");
	xls.SetCellValue(filerow, filecol + 5, L"�ڶ���Ӧ��");
	xls.SetCellValue(filerow, filecol + 6, L"�ж�");
	xls.SetCellValue(filerow, filecol + 7, L"������Ӧ��");
	xls.SetCellValue(filerow, filecol + 8, L"�ж�");
	xls.SetCellValue(filerow, filecol + 9, L"���ĸ�Ӧ��");
	xls.SetCellValue(filerow, filecol + 10, L"�ж�");
	return;
	xls.SetCellValue(filerow, filecol + 11, L"Բ��002");
	xls.SetCellValue(filerow, filecol + 12, L"�ж�");
	xls.SetCellValue(filerow, filecol + 13, L"�׾�[����]");
	xls.SetCellValue(filerow, filecol + 14, L"�ж�");
	xls.SetCellValue(filerow, filecol + 15, L"Բ-�߾���003[����]");
	xls.SetCellValue(filerow, filecol + 16, L"�ж�");
	xls.SetCellValue(filerow, filecol + 17, L"Բ-�߾���004[����]");
	xls.SetCellValue(filerow, filecol + 18, L"�ж�");
	return;



	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"��Ʒ");
	xls.SetCellValue(filerow, filecol + 2, L"Բ��001");
	xls.SetCellValue(filerow, filecol + 3, L"�ж�");
	xls.SetCellValue(filerow, filecol + 4, L"Բ��002");
	xls.SetCellValue(filerow, filecol + 5, L"�ж�");
	xls.SetCellValue(filerow, filecol + 6, L"�׾�[����]");
	xls.SetCellValue(filerow, filecol + 7, L"�ж�");
	xls.SetCellValue(filerow, filecol + 8, L"���β���ʱ��");
}
void ExcelFormat1()
{
	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"��Ʒ");
	xls.SetCellValue(filerow, filecol + 2, L"����ʱ��");

	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"��Ʒ");
	xls.SetCellValue(filerow, filecol + 2, L"����ʱ��");
	xls.SetCellValue(filerow, filecol + 3, L"��һ��Ӧ��");
	xls.SetCellValue(filerow, filecol + 4, L"�ж�");
	xls.SetCellValue(filerow, filecol + 5, L"�ڶ���Ӧ��");
	xls.SetCellValue(filerow, filecol + 6, L"�ж�");
	xls.SetCellValue(filerow, filecol + 7, L"������Ӧ��");
	xls.SetCellValue(filerow, filecol + 8, L"�ж�");
	xls.SetCellValue(filerow, filecol + 9, L"���ĸ�Ӧ��");
	xls.SetCellValue(filerow, filecol + 10, L"�ж�");
	return;






	xls.SetCellValue(filerow, filecol + 3, L"ӥ����");
	xls.SetCellValue(filerow, filecol + 4, L"�ж�");
	xls.SetCellValue(filerow, filecol + 5, L"������");
	xls.SetCellValue(filerow, filecol + 6, L"�ж�");
	xls.SetCellValue(filerow, filecol + 7, L"Բ���뾶");
	xls.SetCellValue(filerow, filecol + 8, L"�ж�");
	xls.SetCellValue(filerow, filecol + 9, L"Բ��001");
	xls.SetCellValue(filerow, filecol + 10, L"�ж�");
	xls.SetCellValue(filerow, filecol + 11, L"Բ��002");
	xls.SetCellValue(filerow, filecol + 12, L"�ж�");
	xls.SetCellValue(filerow, filecol + 13, L"�׾�[����]");
	xls.SetCellValue(filerow, filecol + 14, L"�ж�");
	xls.SetCellValue(filerow, filecol + 15, L"Բ-�߾���003[����]");
	xls.SetCellValue(filerow, filecol + 16, L"�ж�");
	xls.SetCellValue(filerow, filecol + 17, L"Բ-�߾���004[����]");
	xls.SetCellValue(filerow, filecol + 18, L"�ж�");

	return;
	xls.SetCellValue(filerow, filecol, L"No");
	xls.SetCellValue(filerow, filecol + 1, L"��Ʒ");
	xls.SetCellValue(filerow, filecol + 2, L"Բ��001");
	xls.SetCellValue(filerow, filecol + 3, L"�ж�");
	xls.SetCellValue(filerow, filecol + 4, L"Բ��002");
	xls.SetCellValue(filerow, filecol + 5, L"�ж�");
	xls.SetCellValue(filerow, filecol + 6, L"�׾�[����]");
	xls.SetCellValue(filerow, filecol + 7, L"�ж�");
	
	xls.SetCellValue(filerow, filecol + 8, L"Բ-�߾���003[����]");
	xls.SetCellValue(filerow, filecol + 9, L"�ж�");
	xls.SetCellValue(filerow, filecol + 10, L"Բ-�߾���004[����]");
	xls.SetCellValue(filerow, filecol + 11, L"�ж�");

	xls.SetCellValue(filerow, filecol + 12, L"���β���ʱ��");
}

/*д��ʱ��*/ //����CCommunicationReaderDlg::OnClose()��
void WriteTime()
{

	CString time = xls.GetDate() + xls.GetTime();
	xls.SelectSheet(1);
	
#ifdef _GT
	xls.SetCellValue(g.row + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"�ĵ��ر�ʱ�䣺") + time);
		return;
#endif
		xls.SetCellValue(Com1T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"�ĵ��ر�ʱ�䣺") + time);
	xls.SelectSheet(2);
	xls.SetCellValue(Com1T2Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"�ĵ��ر�ʱ�䣺") + time);

	xls.SelectSheet(3);
	xls.SetCellValue(Com2T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"�ĵ��ر�ʱ�䣺") + time);

	xls.SelectSheet(4);
	xls.SetCellValue(Com2T2Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L"�ĵ��ر�ʱ�䣺") + time);
}