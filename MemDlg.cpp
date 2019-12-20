#include "stdafx.h"
#include "Mem.h"
#include "MemDlg.h"
#include "AllMem.h"

#include <iostream>
#include <string>
#include "Essentials/Schedule.h"
#include "Essentials/ClientDataBase.h"
#include "Net/ClientNetUtils.h"
#include "JSONLib/json.hpp"
#include "Essentials/Timer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMemDlg::CMemDlg(Schedule* mem)
	: CDialog(CMemDlg::IDD, NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->mem = mem;
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
	ON_BN_CLICKED(IDC_EDIT_OK, &CMemDlg::OnBnClickedEditOk)
	ON_BN_CLICKED(IDC_CANCEL_EDIT, &CMemDlg::OnBnClickedCancelEdit)
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
	

	if (mem) {
		m_date.SetTime(&CTime(mem->endTime));
		m_time.SetTime(&CTime(mem->endTime));
		m_body.SetWindowText(CString(mem->name));
	}
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








void CMemDlg::OnBnClickedEditOk()
{
	CTime mm_date, mm_time;
	memset(&mm_date, 0, sizeof(mm_date));
	CString mm_important, mm_body;
	CMemApp* pApp = (CMemApp*)AfxGetApp();

	m_date.GetTime(mm_date);
	m_time.GetTime(mm_time);
	m_body.GetWindowText(mm_body);



	tm ddl; Schedule *sch = mem ? mem : new Schedule;

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
	sch->alerted = false;
	if (mem) {
		DataBase::editSchedule(sch->sid, *sch);//放到文件中
		if (loggedin) DataBase::sync_edit(sch->sid, *sch);//服务器同步
	}
	else {
		schedules.push_back(sch);
		DataBase::addSchedule(*sch);//放到文件中
		if (loggedin) DataBase::sync_add(*sch);//服务器同步
	}
	selectEarliest();


	m_body.SetModify(FALSE);
	CDialog::OnOK();
}


void CMemDlg::OnBnClickedCancelEdit()
{
	CDialog::OnCancel();
}