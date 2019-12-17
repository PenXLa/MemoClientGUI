#pragma once
#include "resource.h"
#include "Essentials/Schedule.h"


class CMemApp : public CWinApp
{
public:
	CMemApp();
public:
	CString CMemApp::TimetoCString(const CTime& date);
public:
	POSITION m_pos;
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
