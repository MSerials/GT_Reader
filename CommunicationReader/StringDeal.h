#pragma once
#include "stdafx.h"
class CStringDeal
{
public:

	CStringDeal()
	{
	}

	virtual ~CStringDeal()
	{
	}

	void cutString(char *str, vector<CString>& cstr, char sign = ',')
	{
		cstr.clear();
		char *s = str, *d = str;
		bool flag = sign == str[strlen(str) - 1] || 0==*d ? false : true;//������һ���ǣ�
		while ('\0' != *d)
		{
			if (sign == *d)
			{
				*d = '\0';
				cstr.push_back((CString)s);
				s = ++d;
				continue;
			}
			d++;
		}
		if(flag) cstr.push_back((CString)s);
	}

	bool checkCharater(char ch)
	{
		if (ch > 13)
			return true;
		else
			return false;
	}

	void DecodeMsg(char *msg)
	{
		return;
	}

	vector<CString> Split(CString string)
	{
		CString oriStr = string;
		vector<CString> strVec;
		while (true)
		{
			CString n = oriStr.SpanExcluding(L",");//��ȡ.֮ǰ���ַ���  
			strVec.push_back(n);
			oriStr = oriStr.Right(oriStr.GetLength() - n.GetLength() - 1);//���ұ�1��ʼ��ȡ��������ǰ nCount ���ַ�  
			if (oriStr.IsEmpty())
			{
				break;
			}
		}
		return strVec;
	}
};

