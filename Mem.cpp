#include "stdafx.h"
#include "Mem.h"
#include "AllMem.h"
#include "MemDlg.h"

#include <iostream>
#include "JSONLib/json.hpp"
#include "Net/ClientNetUtils.h"
#include "Utils/Terminal.h"
#include "Essentials/Timer.h"
#include "Essentials/ClientDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CMemApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CMemApp::CMemApp()
{
	m_pos = NULL;
}

CMemApp theApp;

BOOL CMemApp::InitInstance()
{
	if (!initSocket()) MessageBox(0,"Socket初始化失败，网络功能将无法使用。","计划安排提醒",0);
	DataBase::loadSchedules();
	startTimer();


	AfxEnableControlContainer();

	CAllMem dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	return FALSE;
}

void CMemApp::Serialize(CArchive& ar)
{
	POSITION pos;
	WORD nCount;

	if (ar.IsStoring())
	{
		nCount = (WORD)m_memList.GetCount();
		ar << nCount;
		pos = m_memList.GetHeadPosition();
		while (pos != NULL)
		{
			CMyMem* pMem = m_memList.GetNext(pos);
			ar << pMem->m_TDate;
			ar << pMem->m_Time;
			ar << pMem->m_strBody;
			nCount--;
		}
		ASSERT(nCount == 0);
	}
	else
	{
		m_memList.RemoveAll();
		ar >> nCount;
		while (nCount-- > 0)
		{
			CMyMem* pMem = new CMyMem;
			ar >> pMem->m_TDate;
			ar >> pMem->m_Time;
			ar >> pMem->m_strBody;
			m_memList.AddTail(pMem);
		}
	}
}

void CMemApp::onSave()
{
	CFile file;
	if (file.Open("mem.mem", CFile::modeCreate | CFile::modeWrite))
	{
		CArchive ar(&file, CArchive::store);
		Serialize(ar);
		ar.Close();
		file.Close();
	}
}

void CMemApp::onRead()
{
	CFile file;
	if (file.Open("mem.mem", CFile::modeRead))
	{
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
		ar.Close();
		file.Close();
	}
}

CString CMemApp::TimetoCString(CTime& date, CTime& time)
{
	char str[30];
	sprintf(str, "%d年%d月%d日 %d:%02d", date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute());
	return (CString)str;
}