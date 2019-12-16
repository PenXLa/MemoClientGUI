#pragma once

class CMemDlg : public CDialog
{
public:
	CMemDlg(CMyMem* mem = NULL);
	CToolBar m_wndToolBar;
	enum { IDD = IDD_MEM_DIALOG };
	CEdit	m_body;
	CDateTimeCtrl	m_date;
	CDateTimeCtrl	m_time;
	CMyMem* mem = NULL;//如果为空，是新建，不为空是编辑
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEditOk();
	afx_msg void OnBnClickedCancelEdit();
};
