#include "stdafx.h"
#include "Xls.h"
#include "odbcinst.h"
#include"tlhelp32.h"

#include "MsgWrite.h"


BOOL Xls::Init()
{
	/*
	1.先查找是否存在ini里面记录的excel地址，对其进行打开，如果打不开，则重新新建立一个excel。
	然后对记录的行和列进行初始化，进行记录。update是为了要契合其他dlg和记录文件对相应的数据进行更新，否则容易错乱,在界面上更新数据后要调用这个Update来更新数据

	2.从指定的路径去拷贝文件，该文件应该是事先客户已经写好具备特定格式的excel文件，从这个文件来记录格式

	3.当一个表格记录完整以后，应该新增一个表格，进行复制格式，就能够保证下次记录有相应的格式。
	*/

	Update();
	KillProcessFromName(L"EXCEL.exe");
	Sleep(100);
	program_path = GetProgramPath();
	CString ExcelPath = program_path + L"\\Excel";
	CreateDirectory(ExcelPath, NULL);
	ExcelPath = program_path + L"\\测量结果\\";
	CreateDirectory(ExcelPath, NULL);
	res_path = ExcelPath;

	file_path = ini.model_path;

	CString InitPath = program_path + L"\\Excel\\Example.xlsx";

	if (!AfxOleInit())
	{
		AfxMessageBox(L"可能没有安装office2013");
		return FALSE;
	}

	if (!ExcelApp.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(L"无法创建Excel应用！");
		return FALSE;
	}	
	//一天一个文档，软件关闭后，同一天打开，继续存储
	//状态文件处理-------------------------------------------------------
	CString tmppath = program_path + L"\\statefile";
	CreateDirectory(tmppath, NULL);

	tmppath = tmppath + L"\\" + L"statefile.txt";

	statefile_path = tmppath;

	CFileFind find;
	if (!find.FindFile(tmppath))//文件第一次创建 //意外丢失的情况暂不处理
	{
	   CString date = GetDate();
	   SetStateFile(date + L"\r\n",L"0\r\n", L"0\r\n", L"0\r\n", L"0\r\n",tmppath);
	   ReadStateFile(tmppath);
	}
	else
	{
		ReadStateFile(tmppath);//读取当前文件名CurXlsFileName，各个记录数（ComxTxCnt）
	}
	//模板，Excel文档处理-----------------------------------
	if (!OpenExcelBook(file_path))//打开模板失败（模板丢失），file_path模板路径
	{
		/*重新制作模板*/
		file_path = InitPath;
		ini.model_path = InitPath;
		ini.SaveParaFile(PARA_PRJ);
		FirstWriteExcel();
		SaveAsExcel(ini.model_path);
	}
	End(); //结束应用

	/*重新初始化Excel App*/
	if (!AfxOleInit())
	{
		AfxMessageBox(L"可能没有安装office2013_2nd");
		return FALSE;
	}

	if (!ExcelApp.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(L"无法创建Excel应用_2nd！");
		return FALSE;
	}

	//------------------------------------------------------------
	CString curdate = GetDate(); //读取当前日期
	if (CurXlsFileName == curdate) //若文件记录的日期与curdate的日期一致,即仍为当天
	{
		CString search_path = res_path + curdate + L".xlsx";
		//打开文件继续记录
		CFileFind find;
		if (find.FindFile(search_path)) //找到文件
		{

				Workbooks = ExcelApp.get_Workbooks();
				lpDisp = Workbooks.Open(search_path,
						covOptional, covOptional, covOptional, covOptional,
						covOptional, covOptional, covOptional, covOptional,
						covOptional, covOptional, covOptional, covOptional,
						covOptional, covOptional
						);				//与office 2000的不同，是个参数的，直接在后面加了两个covOptional成功了
				Workbook.AttachDispatch(lpDisp);
				Worksheets = Workbook.get_Sheets();
				Worksheet = Worksheets.get_Item(COleVariant((short)1));	

				/*上次时间戳清除*/
				SelectSheet(1);
				SetCellValue(Com1T1Cnt + TIME_ROWOFFSET, TIME_COLOFFSET, CString(L""));

				//写的不是好，
				//获得最后一行的行数
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
		else //未找到文件 //第一次使用时
		{

				/*建立一份模板的拷贝，存储数据*/
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
	else //不为当天
	{
		CString date = GetDate();
		SetStateFile(date + L"\r\n", L"0\r\n", L"0\r\n", L"0\r\n", L"0\r\n", tmppath);
		ReadStateFile(tmppath);
		/*建立一份模板的拷贝，存储数据*/
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
	//写入文件名
	cfile.WriteString(s0);
	//写入记录数
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
	//打开Excel，其中pathname为Excel表的路径名  
	Workbook = Workbooks.Add(covOptional);
	Worksheets = Workbook.get_Worksheets();
	Worksheet = Worksheets.get_Item(COleVariant((short)1)); 
	//获得坐标为（A，1）和（B，1）的两个单元格 
	range = Worksheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("B1")));  //设置单元格类容为Hello Excel
	range.Merge(_variant_t(0));
	range.put_Value2(COleVariant(_T("基恩士数据记录")));  //选择整列，并设置宽度为自适应 
	cols = range.get_EntireColumn();
	cols.AutoFit();
	//设置字体为粗体 
	font = range.get_Font();
	font.put_Bold(COleVariant((short)TRUE));
	//获得坐标为（C，2）单元格 
	range = Worksheet.get_Range(COleVariant(_T("C2")), COleVariant(_T("C2")));
	//设置公式“=RAND()*100000”
	//range.put_Formula(COleVariant(_T("=RAND()*100000")));
	//设置数字格式为货币型  
	//range.put_NumberFormat(COleVariant(_T("$0.00")));
	//选择整列，并设置宽度为自适应  

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
		AfxMessageBox(_T("当前Excel的版本是2003。"));
	}
	else if (_T("12") == strExcelVersion)
	{
		return 12;
		AfxMessageBox(_T("当前Excel的版本是2007。"));
	}
	else
	{
		return 1;
		AfxMessageBox(_T("当前Excel的版本是其他版本。"));
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

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers((LPWSTR)szBuf, cbBufMax, &cbBufOut))
	{
		sDriver = L"";
		return FALSE;
	}
		
	// 检索已安装的驱动是否有Excel...
	do
	{
		if (strstr(pszBuf, "Excel") != 0)
		{
			//发现 !
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
///Description: 初始化函数，初始化中附加excel应用程序
///Call:		app.CreateDispatch(_T("Excel.Application")
////////////////////////////////////////////////////////////////////////
COptExcel::COptExcel(void)
{
	if (::CoInitialize(NULL) == E_INVALIDARG)
	{
		AfxMessageBox(_T("初始化Com失败!"));
		return;
	}

	//验证office文件是否可以正确运行

	if (!app.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("无法创建Excel应用！"));
		return;
	}
	//在程序执行文件路径名中，剪掉执行文件名，得到程序路径，追加模板文件名，得到模板完整路径


}
#endif

////////////////////////////////////////////////////////////////////////
///Function:	~COptExcel
///Description:	析构函数，释放对象，非常重要，不全部释放，占用内存，下
///				一次使用此类时会中断
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
///Description:	打开表名为filename的文件，注意，文件路径非自动生成，以后
///				考虑从下处理方法
///Call:		GetAppPath()
///Input:		CString filename 文件名
////////////////////////////////////////////////////////////////////////
bool Xls::OpenExcelBook(CString filename)
{

	CFileFind filefind;
	if (!filefind.FindFile(filename))
	{
		return false;
	}
	LPDISPATCH lpDisp; //接口指针
	Workbooks = ExcelApp.get_Workbooks();
	lpDisp = Workbooks.Open(filename,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional
		);										//与office 2000的不同，是个参数的，直接在后面加了两个covOptional成功了
	Workbook.AttachDispatch(lpDisp);
	Worksheets = Workbook.get_Sheets();
	Worksheet = Worksheets.get_Item(COleVariant((short)1));		//与的不同，是个参数的，直接在后面加了两个covOptional成功了
	return true;
}

void Xls::NewExcelBook()
{
	Workbooks = ExcelApp.get_Workbooks();
	Workbook = Workbooks.Add(covOptional);
	Worksheets = Workbook.get_Sheets();
	Worksheet = Worksheets.get_Item(COleVariant((short)1));		//与的不同，是个参数的，直接在后面加了两个covOptional成功了
}

////////////////////////////////////////////////////////////////////////
///Function:	OpenExcelApp
///Description:	打开应用程序（要注意以后如何识别用户要打开的是哪个文件）
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
///Description:	用于打开数据文件，续存数据后直接保存
////////////////////////////////////////////////////////////////////////
void Xls::SaveExcel(void)
{
	//Workbook.put_Saved(TRUE);//////
	Workbook.Save();//////
}

////////////////////////////////////////////////////////////////////////
///Function:	SaveAsExcel
///Description:	保存excel文件
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
///Description:	修改单元格内的值
///Call:		IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串
///Input:		int row 单元格所在行
///				int col 单元格所在列
///				int Align		对齐方式默认为居中
////////////////////////////////////////////////////////////////////////
void Xls::SetCellValue(int row, int col, int Align) //改为写入数值
{
	try {
		range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
		range.put_Value2(COleVariant((short)Align));
	}
	catch (...)
	{
		AfxMessageBox(L"获取Excel单元格出错！");
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
		AfxMessageBox(L"获取Excel单元格出错！");
	}
}

////////////////////////////////////////////////////////////////////////
///Function:	GetCellValue
///Description:	得到的单元格中的值
///Call:		IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串
///Input:		int row 单元格所在行
///				int col 单元格所在列
///Return:		CString 单元格中的值
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
///Description:	设置行高
///Call:		IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串
///Input:		int row 单元格所在行
////////////////////////////////////////////////////////////////////////
void Xls::SetRowHeight(int row, CString height)
{
	int col = 1;
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	range.put_RowHeight(COleVariant(height));
}
////////////////////////////////////////////////////////////////////////
///Function:	SetColumnWidth
///Description:	设置列宽
///Call:		IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串
///Input:		int col 要设置列宽的列
///				CString 宽值
////////////////////////////////////////////////////////////////////////
void Xls::SetColumnWidth(int col, CString width)
{
	int row = 1;
	range = Worksheet.get_Range(COleVariant(IndexToString(row, col)), COleVariant(IndexToString(row, col)));
	range.put_ColumnWidth(COleVariant(width));
}

////////////////////////////////////////////////////////////////////////
///Function:	SetRowHeight
///Description:	设置行高
///Call:		IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串
///Input:		int row 单元格所在行
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
///Description:	设置行高
///Call:		IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串
///Input:		int row 要设置行高的行
///				CString 宽值
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
///Description:	得到的单元格在EXCEL中的定位名称字符串
///Input:		int row 单元格所在行
///				int col 单元格所在列
///Return:		CString 单元格在EXCEL中的定位名称字符串
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
///Description:	得到表格总第一个空行的索引
///Return:		int 空行的索引号
////////////////////////////////////////////////////////////////////////
int Xls::LastLineIndex()
{
	int i, j, flag = 0;
	CString str;
	for (i = 1;; i++)
	{
		flag = 0;
		//粗略统计，认为前列都没有数据即为空行
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
	//创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32进程快照的结构体  
	PROCESSENTRY32 pe;

	//实例化后使用Process32First获取第一个快照的进程前必做的初始化操作  
	pe.dwSize = sizeof(PROCESSENTRY32);


	//下面的IF效果同:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄  
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//将字符串转换为小写  
	strProcessName.MakeLower();

	//如果句柄有效  则一直获取下一个句柄循环下去  
	while (Process32Next(hSnapShot, &pe))
	{

		//pe.szExeFile获取当前进程的可执行文件名称  
		CString scTmp = pe.szExeFile;


		//将可执行文件名称所有英文字母修改为小写  
		scTmp.MakeLower();

		//比较当前进程的可执行文件名称和传递进来的文件名称是否相同  
		//相同的话Compare返回0  
		if (!scTmp.Compare(strProcessName))
		{

			//从快照进程中获取该进程的PID(即任务管理器中的PID)  
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