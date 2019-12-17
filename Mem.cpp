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
	if (!initSocket()) MessageBox(0,"Socket��ʼ��ʧ�ܣ����繦�ܽ��޷�ʹ�á�","�ƻ���������",0);
	DataBase::loadSchedules();
	startTimer();


	AfxEnableControlContainer();

	CAllMem dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}


CString CMemApp::TimetoCString(const CTime& date)
{
	char str[30];
	sprintf_s(str, "%d��%d��%d�� %d:%02d", date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), date.GetMinute());
	return (CString)str;
}