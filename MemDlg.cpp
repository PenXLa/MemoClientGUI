#include "stdafx.h"
#include "Mem.h"
#include "MemDlg.h"
#include "AllMem.h"
#include "AbooutMe.h"

#include <iostream>
#include <string>
#include "Essentials/Schedule.h"
#include "Essentials/ClientDataBase.h"
#include "Net/ClientNetUtils.h"
#include "JSONLib/json.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMemDlg::CMemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BODY, m_body);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_TIME, m_time);
}

BEGIN_MESSAGE_MAP(CMemDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_NEW, OnMenuNew)
	ON_COMMAND(ID_MENU_SAVE, OnButtonSave)
	ON_COMMAND(ID_MENU_SHOWALL, OnButtonManger)
	ON_COMMAND(ID_MENU_EXIT, OnOK)
	ON_COMMAND(ID_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_NEW, OnMenuNew)
	ON_COMMAND(ID_SAVE, OnButtonSave)
	ON_COMMAND(ID_SHOW, OnButtonManger)
	ON_COMMAND(ID_MENU_TEST, &CMemDlg::OnTest)
END_MESSAGE_MAP()

BOOL CMemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	m_date.GetTime(&sysTime);
	

	pApp->onRead();
	if (!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
	{
		TRACE0("Failed to Create Memor Toolbar\n");
		EndDialog(IDCANCEL);
	}

	CRect rcClientOld;
	CRect rcClientNew;
	GetClientRect(rcClientOld);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNew);

	CPoint ptOffset(rcClientNew.left - rcClientOld.left,
		rcClientNew.top - rcClientOld.top);

	CRect rcChild;
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientOld.Width() - rcClientNew.Width();
	rcWindow.bottom += rcClientOld.Height() - rcClientNew.Height();
	MoveWindow(rcWindow, FALSE);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	return TRUE;
}

void CMemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMemDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CMemDlg::OnButtonSave()
{
	CTime mm_date, mm_time;
	memset(&mm_date, 0, sizeof(mm_date));
	CString mm_important, mm_body;
	CMemApp* pApp = (CMemApp*)AfxGetApp();

	m_date.GetTime(mm_date);
	m_time.GetTime(mm_time);
	m_body.GetWindowText(mm_body);
	


	tm ddl; Schedule *sch = new Schedule;//内存中创建新事项
	strcpy(sch->name, ((std::string)mm_body.GetBuffer()).c_str());
	ddl.tm_year = mm_date.GetYear();
	ddl.tm_mon = mm_date.GetMonth();
	ddl.tm_mday = mm_date.GetDay();
	ddl.tm_hour = mm_time.GetHour();
	ddl.tm_min = mm_time.GetMinute();
	ddl.tm_sec = mm_time.GetSecond();
	ddl.tm_year -= 1900; --ddl.tm_mon;
	sch->endTime = mktime(&ddl);
	sch->lastEdit = time(0);
	schedules.push_back(sch);
	DataBase::addSchedule(*sch);//放到文件中
	if (loggedin) DataBase::sync_add(*sch);//服务器同步


	CMyMem* pMem = new CMyMem;

	pMem->m_TDate = mm_date;
	pMem->m_strBody = mm_body;
	pMem->m_Time = mm_time;
	pMem->sch = sch;
	if (pApp->is_old)
	{
		POSITION pos;
		pos = pApp->m_pos;
		pApp->m_memList.SetAt(pos, pMem);
	}
	else
	{
		pApp->m_memList.AddTail(pMem);
		pApp->m_pos = pApp->m_memList.GetTailPosition();
		pApp->is_old = TRUE;
	}

	pApp->onSave();

	m_body.SetModify(FALSE);
}

void CMemDlg::OnButtonManger()
{
	POSITION pos;
	CAllMem alldlg;
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	if (IDOK == alldlg.DoModal())
	{
		pos = pApp->m_pos;
		CMyMem* pMem = pApp->m_memList.GetAt(pos);
		m_date.SetTime(&pMem->m_TDate);
		m_time.SetTime(&pMem->m_Time);
		m_body.SetWindowText(pMem->m_strBody);
	}
}

void CMemDlg::OnMenuAbout()
{
	CAbooutMe aboutdlg;
	aboutdlg.DoModal();
}

void CMemDlg::OnOK()
{
	if (m_body.GetModify())
	{
		int flag = MessageBox("是否保存？", "提示", MB_ICONQUESTION | MB_YESNOCANCEL);
		if (IDYES == flag)
		{
			OnButtonSave();
			CDialog::OnOK();
		}
		else if (IDNO == flag)
		{
			CDialog::OnOK();
		}
	}
	else
		CDialog::OnOK();
}

void CMemDlg::OnMenuNew()
{
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	pApp->is_old = FALSE;
	pApp->m_pos = NULL;
	m_body.SetWindowText("");
}


void CMemDlg::OnTest()
{

}
