#include "stdafx.h"
#include "Mem.h"
#include "AllMem.h"
#include "MemDlg.h"


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


BOOL CAllMem::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	pApp->onRead();

	refresh();
	return TRUE;
}

void CAllMem::refresh() {
	CMemApp* pApp = (CMemApp*)AfxGetApp();

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
	WORD nCount;
	POSITION pos;
	CString my_date;

	nCount = (WORD)pApp->m_memList.GetCount();
	pos = pApp->m_memList.GetHeadPosition();
	m_list.DeleteAllItems();
	for (int k = 0; k < nCount && pos != NULL; k++)
	{
		CMyMem* pMem = pApp->m_memList.GetNext(pos);
		m_list.InsertItem(k, LPSTR_TEXTCALLBACK);
		my_date = pApp->TimetoCString(pMem->m_TDate, pMem->m_Time);
		m_list.SetItemText(k, 0, my_date);
		m_list.SetItemText(k, 1, pMem->m_strBody);
	}
}

void CAllMem::OnButtonDel()
{
	int nSel;
	POSITION pos;
	CMemApp* pApp = (CMemApp*)AfxGetApp();
	if (FindMyMem(nSel, pos))
	{
		CMyMem* pMem = pApp->m_memList.GetAt(pos);
		for (auto p = schedules.begin(); p != schedules.end(); ++p) {
			if ((*p)->sid == pMem->sch->sid) {
				DataBase::removeSchedule(pMem->sch->sid);//数据库清除
				delete(*p);//内存释放
				schedules.erase(p);//链表中除名
				break;
			}
		}
		if (loggedin)DataBase::sync_remove(pMem->sch->sid);//同步

		pApp->m_memList.RemoveAt(pos);
		m_list.DeleteItem(nSel);
		pApp->onSave();
	}
	else
	{
		MessageBox("错误", "错误", MB_OK);
	}
}

int CAllMem::FindMyMem(int& nSel, POSITION& pos)
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
	}
	pos = pApp->m_memList.FindIndex(nSel);
	ASSERT(pos != NULL);
	return 1;
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
		nSel = m_list.GetNextSelectedItem(pos1);
		pos = pApp->m_memList.FindIndex(nSel);
		pApp->m_pos = pos;
		
		CMemDlg dlg(pApp->m_memList.GetAt(pos));
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
	pApp->m_memList.RemoveAll();
	m_list.DeleteAllItems();
	pApp->onSave();

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
