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
		bool flag = sign == str[strlen(str) - 1] || 0==*d ? false : true;//如果最后一个是，
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
			CString n = oriStr.SpanExcluding(L",");//获取.之前的字符串  
			strVec.push_back(n);
			oriStr = oriStr.Right(oriStr.GetLength() - n.GetLength() - 1);//从右边1开始获取从右向左前 nCount 个字符  
			if (oriStr.IsEmpty())
			{
				break;
			}
		}
		return strVec;
	}
};

