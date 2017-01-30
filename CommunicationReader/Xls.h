#pragma once
#include "stdafx.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "MsgWrite.h"

enum{
	NoError,EXCELCLOSED

};

class Xls
{
public:
	Xls();
	~Xls();
	BOOL Init();
	void FirstWriteExcel();
	void Update();
	void SetCellValue();
	CString GetProgramPath();
	BOOL IsInstalled();
	bool OpenExcelBook(CString filename);
	void NewExcelBook();
	void OpenExcelApp(BOOL style = FALSE);
	void SaveExcel(void);
	void SaveAsExcel(CString filename);
	void SetCellValue(int row, int col, int Align = 0);
	void SetCellValue(int row, int col, CString str);
	CString GetCellValue(int row, int col);
	void  SetRowHeight(int row, CString height);
	void SetColumnWidth(int col, CString width);
	CString GetColumnWidth(int col);
	CString GetRowHeight(int row);
	CString IndexToString(int row, int col);
	int LastLineIndex();
	BOOL KillProcessFromName(CString strProcessName);
	void FirstCopy(CString model);
	void End();
	BOOL showXls();
	void AddWorkSheet();
	bool SelectSheet(int SheetNo);
	UINT Version();
	//获取日期
	CString GetTime();
	//获取时分秒
	CString GetDate();
	int m_row;
	int m_col;

	int recordrow1; //reseverd
	int recordcol1;
	int recordrow2;
	int recordcol2;
	int recordrow3;
	int recordcol3;
	int recordrow4;
	int recordcol4;
	CString value;
	int m_errorcode;

private:
	CApplication ExcelApp;
	CWorkbook Workbook;
	CWorkbooks Workbooks;
	CWorksheet Worksheet;
	CWorksheets Worksheets;
	CWorksheet TmpWorksheet;
	CWorksheets TmpWorksheets;

	CRange range;
	CFont0 font;
	//Font m_font;
	CRange cols;
	CRange rows;
	CRange cell;
	LPDISPATCH lpDisp;
	COleVariant covOptional;
	CString model_path;
	CString program_path;
	CString file_path;
	CString res_path;
	BOOL m_showxls;

	//-----------------------------------------
public:
	void SetStateFile(LPCTSTR s0, LPCTSTR s1, LPCTSTR s2, LPCTSTR s3, LPCTSTR s4, const CString &path);
	void ReadStateFile(const CString &path);
//private:
	CString statefile_path;
};

