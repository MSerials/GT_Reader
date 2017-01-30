#include "stdafx.h"
#include "Xls.h"
#include "odbcinst.h"
#include"tlhelp32.h"

#include "MsgWrite.h"


BOOL Xls::Init()
{
	/*
	1.�Ȳ����Ƿ����ini�����¼��excel��ַ��������д򿪣�����򲻿����������½���һ��excel��
	Ȼ��Լ�¼���к��н��г�ʼ�������м�¼��update��Ϊ��Ҫ��������dlg�ͼ�¼�ļ�����Ӧ�����ݽ��и��£��������״���,�ڽ����ϸ������ݺ�Ҫ�������Update����������

	2.��ָ����·��ȥ�����ļ������ļ�Ӧ�������ȿͻ��Ѿ�д�þ߱��ض���ʽ��excel�ļ���������ļ�����¼��ʽ

	3.��һ������¼�����Ժ�Ӧ������һ����񣬽��и��Ƹ�ʽ�����ܹ���֤�´μ�¼����Ӧ�ĸ�ʽ��
	*/

	Update();
	KillProcessFromName(L"EXCEL.exe");
	Sleep(100);
	program_path = GetProgramPath();
	CString ExcelPath = program_path + L"\\Excel";
	CreateDirectory(ExcelPath, NULL);
	ExcelPath = program_path + L"\\�������\\";
	CreateDirectory(ExcelPath, NULL);
	res_path = ExcelPath;

	file_path = ini.model_path;

	CString InitPath = program_path + L"\\Excel\\Example.xlsx";

	if (!AfxOleInit())
	{
		AfxMessageBox(L"����û�а�װoffice2013");
		return FALSE;
	}

	if (!ExcelApp.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(L"�޷�����ExcelӦ�ã�");
		return FALSE;
	}	
	//һ��һ���ĵ�������رպ�ͬһ��򿪣������洢
	//״̬�ļ�����-------------------------------------------------------
	CString tmppath = program_path + L"\\statefile";
	CreateDirectory(tmppath, NULL);

	tmppath = tmppath + L"\\" + L"statefile.txt";

	statefile_path = tmppath;

	CFileFind find;
	if (!find.FindFile(tmppath))//�ļ���һ�δ��� //���ⶪʧ������ݲ�����
	{
	   CString date = GetDate();
	   SetStateFile(date + L"\r\n",L"0\r\n", L"0\r\n", L"0\r\n", L"0\r\n",tmppath);
	   ReadStateFile(tmppath);
	}
	else
	{
		ReadStateFile(tmppath);//��ȡ��ǰ�ļ���CurXlsFileName��������¼����ComxTxCnt��
	}
	//ģ�壬Excel�ĵ�����-----------------------------------
	if (!OpenExcelBook(file_path))//��ģ��ʧ�ܣ�ģ�嶪ʧ����file_pathģ��·��
	{
		/*��������ģ��*/
		file_path = InitPath;
		ini.model_path = InitPath;
		ini.SaveParaFile(PARA_PRJ);
		FirstWriteExcel();
		SaveAsExcel(ini.model_path);
	}
	End(); //����Ӧ��

	/*���³�ʼ��Excel App*/
	if (!AfxOleInit())
	{
		AfxMessageBox(L"����û�а�װoffice2013_2nd");
		return FALSE;
	}

	if (!ExcelApp.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(L"�޷�����ExcelӦ��_2nd��");
		return FALSE;
	}

	//------------------------------------------------------------
	CString curdate = GetDate(); //��ȡ��ǰ����
	if (CurXlsFileName == curdate) //���ļ���¼��������curdate������һ��,����Ϊ����
	{
		CString search_path = res_path + curdate + L".xlsx";
		//���ļ�������¼
		CFileFind find;
		if (find.FindFile(search_path)) //�ҵ��ļ�
		{

				Workbooks = ExcelApp.get_Workbooks();
				lpDisp = Workbooks.Open(search_path,
						covOptional, covOptional, covOptional, covOptional,
						covOptional, covOptional, covOptional, covOptional,
						covOptional, covOptional, covOptional, covOptional,
						covOptional, covOptional
						);				//��office 2000�Ĳ�ͬ���Ǹ������ģ�ֱ���ں����������covOptional�ɹ���
				Workbook.AttachDispatch(lpDisp);
				Worksheets = Workbook.get_Sheets();
				Worksheet = Worksheets.get_Item(COleVariant((short)1));	

				/*�ϴ�ʱ������*/
				SelectSheet(1);
				SetCellValue(Com1T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L""));

				//д�Ĳ��Ǻã�
				//������һ�е�����
			//	g.row = LastLineIndex();

#ifdef MYDEBUG
				printf("find excel file.\n");
#endif

				//SelectSheet(2);
				//SetCellValue(Com1T2Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L""));

				//SelectSheet(3);
				//SetCellValue(Com2T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L""));

				//SelectSheet(4);
				//SetCellValue(Com2T2Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L""));
		}
		else //δ�ҵ��ļ� //��һ��ʹ��ʱ
		{

				/*����һ��ģ��Ŀ������洢����*/
				OpenExcelBook(file_path);

				SaveAsExcel(res_path + curdate);
				//SelectSheet(1);
				InitSheet();

				g.row = 0;
#ifdef MYDEBUG
				printf("excel not found. \n");
#endif
		}
	}
	else //��Ϊ����
	{
		CString date = GetDate();
		SetStateFile(date + L"\r\n", L"0\r\n", L"0\r\n", L"0\r\n", L"0\r\n", tmppath);
		ReadStateFile(tmppath);
		/*����һ��ģ��Ŀ������洢����*/
		OpenExcelBook(file_path);

		SaveAsExcel(res_path + GetDate());
		//SelectSheet(1);
		InitSheet();
	}

#ifdef MYDEBUG
	ExcelApp.put_Visible(TRUE);
	ExcelApp.put_UserControl(TRUE);
#endif
	return TRUE;
}

void Xls::SetStateFile(LPCTSTR s0, LPCTSTR s1, LPCTSTR s2, LPCTSTR s3, LPCTSTR s4, const CString &path)
{
	CStdioFile cfile;

	cfile.Open(path, CFile::modeCreate | CFile::modeWrite);
	//д���ļ���
	cfile.WriteString(s0);
	//д���¼��
	cfile.WriteString(s1);
	cfile.WriteString(s2);
	cfile.WriteString(s3);
	cfile.WriteString(s4);

	cfile.Close();
}
void Xls::ReadStateFile(const CString &path)
{
	CStdioFile cfile;

	cfile.Open(path, CFile::modeRead);

	//read param
	//read current excel filename
	cfile.ReadString(CurXlsFileName);
	CurXlsFileName.Remove(L'\r');

	CString tmp;
	//Com1T1Cnt
	cfile.ReadString(tmp);
	tmp.Remove(L'\r');//param repair
	Com1T1Cnt = _ttoi(tmp);

	//Com1T2Cnt
	cfile.ReadString(tmp);
	tmp.Remove(L'\r');//param repair
	Com1T2Cnt = _ttoi(tmp);

	//Com2T1Cnt
	cfile.ReadString(tmp);
	tmp.Remove(L'\r');//param repair
	Com2T1Cnt = _ttoi(tmp);

	//Com2T2Cnt
	cfile.ReadString(tmp);
	tmp.Remove(L'\r');//param repair
	Com2T2Cnt = _ttoi(tmp);

	//close file
	cfile.Close();
}


Xls::Xls()
	:covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
	, m_row(0)
	, m_col(0)
	, m_showxls(TRUE)
	, m_errorcode(0)
{
	 recordrow1 = 0; //reseverd
	 recordcol1 = 0;
	 recordrow2 = 0;
	 recordcol2 = 0;
	 recordrow3 = 0;
	 recordcol3 = 0;
	 recordrow4 = 0;
	 recordcol4 = 0;
}


CString Xls::GetTime()
{
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%d:%d:%d"), st.wHour, st.wMinute, st.wSecond);
	return strTime;

}

CString Xls::GetDate()
{
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%d-%d_"), st.wYear, st.wMonth, st.wDay);
	return strDate;
}


void Xls::Update()
{
	m_row = ini.m_row;
	m_col = ini.m_col;
}

void Xls::FirstWriteExcel()
{
	Workbooks = ExcelApp.get_Workbooks();
	//��Excel������pathnameΪExcel���·����  
	Workbook = Workbooks.Add(covOptional);
	Worksheets = Workbook.get_Worksheets();
	Worksheet = Worksheets.get_Item(COleVariant((short)1)); 
	//�������Ϊ��A��1���ͣ�B��1����������Ԫ�� 
	range = Worksheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("B1")));  //���õ�Ԫ������ΪHello Excel
	range.Merge(_variant_t(0));
	range.put_Value2(COleVariant(_T("����ʿ���ݼ�¼")));  //ѡ�����У������ÿ��Ϊ����Ӧ 
	cols = range.get_EntireColumn();
	cols.AutoFit();
	//��������Ϊ���� 
	font = range.get_Font();
	font.put_Bold(COleVariant((short)TRUE));
	//�������Ϊ��C��2����Ԫ�� 
	range = Worksheet.get_Range(COleVariant(_T("C2")), COleVariant(_T("C2")));
	//���ù�ʽ��=RAND()*100000��
	//range.put_Formula(COleVariant(_T("=RAND()*100000")));
	//�������ָ�ʽΪ������  
	//range.put_NumberFormat(COleVariant(_T("$0.00")));
	//ѡ�����У������ÿ��Ϊ����Ӧ  

	cols = range.get_EntireColumn();
	cols.AutoFit();
	rows = range.get_EntireRow();
	rows.AutoFit();
	
	SetCellValue(2, 2, L"Exp");
}

void Xls::AddWorkSheet()
{
	CString SheetName = Worksheet.get_Name(), str;
	str.Format(_T("Sheet%d"), 1+Worksheets.get_Count());;
	CWorksheet DestSheet;
	DestSheet = Worksheets.get_Item(_variant_t(SheetName));
	DestSheet.Copy(_variant_t(Worksheet), covOptional);
	DestSheet = Worksheets.get_Item(_variant_t(SheetName + L" (2)"));
	Worksheet.put_Name(str);
	DestSheet.put_Name(SheetName);
	Worksheet.Activate();
	return;
}

bool Xls::SelectSheet(int SheetNo)
{
	CString SheetName;
	SheetName.Format(_T("Sheet%d"), SheetNo);
	try{
		Worksheet = Worksheets.get_Item(_variant_t(SheetName));
		Worksheet.Activate();
		return true;
	}
	catch (...)
	{
		Worksheet = Worksheets.Add(covOptional,_variant_t(Worksheet), covOptional, covOptional);
		Worksheet.put_Name(SheetName);
		Worksheet.Activate();
		return false;
	}
}


UINT Xls::Version()
{
	CString strExcelVersion = ExcelApp.get_Version();
	int iStart = 0;
	strExcelVersion = strExcelVersion.Tokenize(_T("."), iStart);
	if (_T("11") == strExcelVersion)
	{
		return 11;
		AfxMessageBox(_T("��ǰExcel�İ汾��2003��"));
	}
	else if (_T("12") == strExcelVersion)
	{
		return 12;
		AfxMessageBox(_T("��ǰExcel�İ汾��2007��"));
	}
	else
	{
		return 1;
		AfxMessageBox(_T("��ǰExcel�İ汾�������汾��"));
	}


}

void Xls::FirstCopy(CString model)
{

}


CString Xls::GetProgramPath()
{
	CString sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return sPath;
}

BOOL Xls::IsInstalled()
{
#if 0
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	// ��ȡ�Ѱ�װ����������(������odbcinst.h��)
	if (!SQLGetInstalledDrivers((LPWSTR)szBuf, cbBufMax, &cbBufOut))
	{
		sDriver = L"";
		return FALSE;
	}
		
	// �����Ѱ�װ�������Ƿ���Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//���� !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	} while (pszBuf[1] != '\0');
	sDriver = (CString)pszBuf;
#endif
	return TRUE;
}



#if 0

COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


////////////////////////////////////////////////////////////////////////
///Function:	COptExcel
///Description: ��ʼ����������ʼ���и���excelӦ�ó���
///Call:		app.CreateDispatch(_T("Excel.Application")
////////////////////////////////////////////////////////////////////////
COptExcel::COptExcel(void)
{
	if (::CoInitialize(NULL) == E_INVALIDARG)
	{
		AfxMessageBox(_T("��ʼ��Comʧ��!"));
		return;
	}

	//��֤office�ļ��Ƿ������ȷ����

	if (!app.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("�޷�����ExcelӦ�ã�"));
		return;
	}
	//�ڳ���ִ���ļ�·�����У�����ִ���ļ������õ�����·����׷��ģ���ļ������õ�ģ������·��


}
#endif

////////////////////////////////////////////////////////////////////////
///Function:	~COptExcel
///Description:	�����������ͷŶ��󣬷ǳ���Ҫ����ȫ���ͷţ�ռ���ڴ棬��
///				һ��ʹ�ô���ʱ���ж�
///Call:		ReleaseDispatch()
////////////////////////////////////////////////////////////////////////
Xls::~Xls()
{
	Workbook.Save();//////

	Workbooks.ReleaseDispatch();
	Workbook.ReleaseDispatch();
	Worksheets.ReleaseDispatch();
	Worksheet.ReleaseDispatch();
	range.ReleaseDispatch();
	font.ReleaseDispatch();
	cell.ReleaseDispatch();
	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();
	::CoUninitialize();
	KillProcessFromName(L"EXCEL.exe");
}

void Xls::End()
{
	Workbook.Save();//////

	Workbooks.ReleaseDispatch();
	Workbook.ReleaseDispatch();
	Worksheets.ReleaseDispatch();
	Worksheet.ReleaseDispatch();
	range.ReleaseDispatch();
	font.ReleaseDispatch();
	cell.ReleaseDispatch();
	ExcelApp.Quit();
	ExcelApp.ReleaseDispatch();
	::CoUninitialize();
	Sleep(100);
	KillProcessFromName(L"EXCEL.exe");

}

////////////////////////////////////////////////////////////////////////
///Function:	OpenExcelBook
///Description:	�򿪱���Ϊfilename���ļ���ע�⣬�ļ�·�����Զ����ɣ��Ժ�
///				���Ǵ��´�����
///Call:		GetAppPath()
///Input:		CString filename �ļ���
////////////////////////////////////////////////////////////////////////
bool Xls::OpenExcelBook(CString filename)
{

	CFileFind filefind;
	if (!filefind.FindFile(filename))
	{
		return false;
	}
	LPDISPATCH lpDisp; //�ӿ�ָ��
	Workbooks = ExcelApp.get_Workbooks();
	lpDisp = Workbooks.Open(filename,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional
		);										//��office 2000�Ĳ�ͬ���Ǹ������ģ�ֱ���ں����������covOptional�ɹ���
	Workbook.AttachDispatch(lpDisp);
	Worksheets = Workbook.get_Sheets();
	Worksheet = Worksheets.get_Item(COleVariant((short)1));		//��Ĳ�ͬ���Ǹ������ģ�ֱ���ں����������covOptional�ɹ���
	return true;
}

void Xls::NewExcelBook()
{
	Workbooks = ExcelApp.get_Workbooks();
	Workbook = Workbooks.Add(covOptional);
	Worksheets = Workbook.get_Sheets();
	Worksheet = Worksheets.get_Item(COleVariant((short)1));		//��Ĳ�ͬ���Ǹ������ģ�ֱ���ں����������covOptional�ɹ���
}

////////////////////////////////////////////////////////////////////////
///Function:	OpenExcelApp
///Description:	��Ӧ�ó���Ҫע���Ժ����ʶ���û�Ҫ�򿪵����ĸ��ļ���
////////////////////////////////////////////////////////////////////////
void Xls::OpenExcelApp(BOOL style)
{
	if (TRUE == style)
	{ 
		ExcelApp.put_Visible(TRUE);
		ExcelApp.put_UserControl(TRUE);
	}
	else
	{
		ExcelApp.put_Visible(FALSE);
		ExcelApp.put_UserControl(FALSE);
	}
}

BOOL Xls::showXls()
{
	if (m_showxls)
	{
		ExcelApp.put_Visible(FALSE);
		ExcelApp.put_UserControl(FALSE);
		m_showxls = FALSE;
		return !m_showxls;
	}
	else
	{
		ExcelApp.put_Visible(TRUE);
		ExcelApp.put_UserControl(TRUE);
		m_showxls = TRUE;
		return !m_showxls;
	}


}

////////////////////////////////////////////////////////////////////////
///Function:	SaveExcel
///Description:	���ڴ������ļ����������ݺ�ֱ�ӱ���
////////////////////////////////////////////////////////////////////////
void Xls::SaveExcel(void)
{
	//Workbook.put_Saved(TRUE);//////
	Workbook.Save();//////
}

////////////////////////////////////////////////////////////////////////
///Function:	SaveAsExcel
///Description:	����excel�ļ�
////////////////////////////////////////////////////////////////////////
void Xls::SaveAsExcel(CString filename)
{
	Workbook.SaveAs(COleVariant(filename), covOptional,
		covOptional, covOptional,
		covOptional, covOptional, (long)0, covOptional, covOptional, covOptional,
		covOptional, covOptional);
}


////////////////////////////////////////////////////////////////////////
///Function:	SetCellValue
///Description:	�޸ĵ�Ԫ���ڵ�ֵ
///Call:		IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���
///Input:		int row ��Ԫ��������
///				int col ��Ԫ��������
///				int Align		���뷽ʽĬ��Ϊ����
////////////////////////////////////////////////////////////////////////
void Xls::SetCellValue(int row, int col, int Align) //��Ϊд����ֵ
{
	try {
		range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
		range.put_Value2(COleVariant((short)Align));
	}
	catch (...)
	{
		AfxMessageBox(L"��ȡExcel��Ԫ�����");
	}

}

void Xls::SetCellValue(int row, int col, CString str)
{
	try{
		range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
		range.put_Value2(COleVariant(str));
	}
	catch (...)
	{
		AfxMessageBox(L"��ȡExcel��Ԫ�����");
	}
}

////////////////////////////////////////////////////////////////////////
///Function:	GetCellValue
///Description:	�õ��ĵ�Ԫ���е�ֵ
///Call:		IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���
///Input:		int row ��Ԫ��������
///				int col ��Ԫ��������
///Return:		CString ��Ԫ���е�ֵ
////////////////////////////////////////////////////////////////////////
CString Xls::GetCellValue(int row, int col)
{
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	COleVariant rValue;
	rValue = COleVariant(range.get_Value2());
	rValue.ChangeType(VT_BSTR);
	return CString(rValue.bstrVal);
}
////////////////////////////////////////////////////////////////////////
///Function:	SetRowHeight
///Description:	�����и�
///Call:		IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���
///Input:		int row ��Ԫ��������
////////////////////////////////////////////////////////////////////////
void Xls::SetRowHeight(int row, CString height)
{
	int col = 1;
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	range.put_RowHeight(COleVariant(height));
}
////////////////////////////////////////////////////////////////////////
///Function:	SetColumnWidth
///Description:	�����п�
///Call:		IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���
///Input:		int col Ҫ�����п����
///				CString ��ֵ
////////////////////////////////////////////////////////////////////////
void Xls::SetColumnWidth(int col, CString width)
{
	int row = 1;
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	range.put_ColumnWidth(COleVariant(width));
}

////////////////////////////////////////////////////////////////////////
///Function:	SetRowHeight
///Description:	�����и�
///Call:		IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���
///Input:		int row ��Ԫ��������
////////////////////////////////////////////////////////////////////////
CString Xls::GetColumnWidth(int col)
{
	int row = 1;
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	VARIANT width = range.get_ColumnWidth();
	CString strwidth;
	strwidth.Format(CString((LPCSTR)(_bstr_t)(_variant_t)width));
	return strwidth;
}

////////////////////////////////////////////////////////////////////////
///Function:	GetRowHeight
///Description:	�����и�
///Call:		IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���
///Input:		int row Ҫ�����иߵ���
///				CString ��ֵ
////////////////////////////////////////////////////////////////////////
CString Xls::GetRowHeight(int row)
{
	int col = 1;
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	VARIANT height = range.get_RowHeight();
	CString strheight;
	strheight.Format(CString((LPCSTR)(_bstr_t)(_variant_t)height));
	return strheight;
}


////////////////////////////////////////////////////////////////////////
///Function:	IndexToString
///Description:	�õ��ĵ�Ԫ����EXCEL�еĶ�λ�����ַ���
///Input:		int row ��Ԫ��������
///				int col ��Ԫ��������
///Return:		CString ��Ԫ����EXCEL�еĶ�λ�����ַ���
////////////////////////////////////////////////////////////////////////
CString Xls::IndexToString(int row, int col)
{
	CString strResult;
	if (col > 26)
	{
		strResult.Format(_T("%c%c%d"), 'A' + (col - 1) / 26 - 1, 'A' + (col - 1) % 26, row);
	}
	else
	{
		strResult.Format(_T("%c%d"), 'A' + (col - 1) % 26, row);
	}
	return strResult;
}

////////////////////////////////////////////////////////////////////////
///Function:	LastLineIndex
///Description:	�õ�����ܵ�һ�����е�����
///Return:		int ���е�������
////////////////////////////////////////////////////////////////////////
int Xls::LastLineIndex()
{
	int i, j, flag = 0;
	CString str;
	for (i = 1;; i++)
	{
		flag = 0;
		//����ͳ�ƣ���Ϊǰ�ж�û�����ݼ�Ϊ����
		for (j = 1; j <= 5; j++)
		{
			str.Format(_T("%s"), this->GetCellValue(i, j).Trim());
			if (str.Compare(_T("")) != 0)
			{
				flag = 1;
				break;
			}

		}
		if (flag == 0)
			return i;

	}
}


BOOL Xls::KillProcessFromName(CString strProcessName)
{
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32���̿��յĽṹ��  
	PROCESSENTRY32 pe;

	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������  
	pe.dwSize = sizeof(PROCESSENTRY32);


	//�����IFЧ��ͬ:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��  
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//���ַ���ת��ΪСд  
	strProcessName.MakeLower();

	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ  
	while (Process32Next(hSnapShot, &pe))
	{

		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����  
		CString scTmp = pe.szExeFile;


		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд  
		scTmp.MakeLower();

		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ  
		//��ͬ�Ļ�Compare����0  
		if (!scTmp.Compare(strProcessName))
		{

			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)  
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			::TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
			return TRUE;
		}
		scTmp.ReleaseBuffer();
	}
	strProcessName.ReleaseBuffer();
	return FALSE;
}