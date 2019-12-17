#include "stdafx.h"
#include "Mem.h"
#include "AllMem.h"
#include "MemDlg.h"
#include "LoginDlg.h"


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

CAllMem::CAllMem(CWnd* pParent /*=NULL*/)
	: CDialog(CAllMem::IDD, pParent)
{

}

void CAllMem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEM, m_list);
}

BEGIN_MESSAGE_MAP(CAllMem, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEALL, OnButtonRemoveall)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	ON_BN_CLICKED(ID_BUT_NEW, &CAllMem::OnBnClickedButNew)
END_MESSAGE_MAP()



void requestLogin() {
	LoginDlg dlg;
	dlg.DoModal();
}

BOOL CAllMem::OnInitDialog()
{
	CDialog::OnInitDialog();
	requestLogin();

	m_list.SetBkColor(RGB(255, 255, 255));
	m_list.SetTextBkColor(RGB(200, 200, 0));
	m_list.SetTextColor(RGB(0, 0, 0));

	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	TCHAR szBuf[20];
	LV_COLUMN lvc;

	int i;
	for (i = 0; i < 2; i++)
	{
		switch (i)
		{
		case 0:
			wsprintf(szBuf, _T("日期"));
			lvc.cx = 180;
			break;
		case 1:
			wsprintf(szBuf, _T("内容"));
			lvc.cx = 250;
			break;
		}
		lvc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = szBuf;
		lvc.iSubItem = i;
		m_list.InsertColumn(i, &lvc);
	}

	refresh();
	return TRUE;
}

void CAllMem::refresh() {
	CString my_date;
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	m_list.DeleteAllItems();
	int k = 0;
	for (Schedule* sch:schedules)
	{
		m_list.InsertItem(k, LPSTR_TEXTCALLBACK);
		my_date = pApp->TimetoCString(CTime(sch->endTime));
		m_list.SetItemText(k, 0, my_date);
		m_list.SetItemText(k, 1, CString(sch->name));
		++k;
	}
}

void CAllMem::OnButtonDel()
{
	int nSel;
	POSITION pos;
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	Schedule *sch;
	if (sch =FindMyMem(nSel))
	{
		DataBase::removeSchedule(sch->sid);//数据库清除
		delete(sch);//内存释放
		schedules.remove(sch);//链表中除名
		if (loggedin)DataBase::sync_remove(sch->sid);//同步

		m_list.DeleteItem(nSel);
	}
	else
	{
		MessageBox("错误", "错误", MB_OK);
	}
}

Schedule* CAllMem::FindMyMem(int &nSel)
{
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	POSITION pos1 = m_list.GetFirstSelectedItemPosition();
	if (pos1 == NULL)
	{
		MessageBox("请先选择所要删除的备忘！", "提示", MB_OK);
		return NULL;
	}
	else
	{
		nSel = m_list.GetNextSelectedItem(pos1);
		auto p = schedules.begin();
		for (int i = 0; i < nSel; ++i) ++p;
		return *p;
	}
}

void CAllMem::OnOK()
{
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	int nSel;
	POSITION pos;
	POSITION pos1 = m_list.GetFirstSelectedItemPosition();
	if (pos1 == NULL)
	{
		MessageBox("请先选择所要查看的备忘！", "提示", MB_OK);
	}
	else
	{
		int nSel;
		CMemDlg dlg(FindMyMem(nSel));
		if (IDOK == dlg.DoModal())
			refresh();
	}
}

void CAllMem::OnCancel()
{
	CDialog::OnCancel();
}

void CAllMem::OnButtonRemoveall()
{
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	m_list.DeleteAllItems();

	DataBase::clearSchedule();
	schedules.clear();
	if (loggedin) DataBase::sync_clear();
}


void CAllMem::OnBnClickedButNew()
{
	CMemDlg dlg;
	if (IDOK == dlg.DoModal())
		refresh();
}
