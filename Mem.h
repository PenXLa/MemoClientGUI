#pragma once
#include "resource.h"
#include "Essentials/Schedule.h"
class CMyMem
{
public:
	CTime m_TDate;
	CTime m_Time;
	CString m_strBody;
	Schedule* sch = NULL;
	CMyMem()
	{
		memset(&m_TDate, 0, sizeof(m_TDate));
		m_strBody = "";
	}
};

typedef CTypedPtrList<CPtrList, CMyMem*> CMyMemList;
class CMemApp : public CWinApp
{
public:
	CMemApp();
public:
	CString CMemApp::TimetoCString(CTime& date, CTime& time);
public:
	CMyMemList m_memList;
	POSITION m_pos;
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
