#pragma once

class CMemDlg : public CDialog
{
public:
	CMemDlg(CWnd* pParent = NULL);
	CToolBar m_wndToolBar;
	enum { IDD = IDD_MEM_DIALOG };
	CEdit	m_body;
	CDateTimeCtrl	m_date;
	CDateTimeCtrl	m_time;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuNew();
	afx_msg void OnOK();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonManger();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTest();
};
