#pragma once

class CMemDlg : public CDialog
{
public:
	CMemDlg(Schedule* mem = NULL);
	CToolBar m_wndToolBar;
	enum { IDD = IDD_MEM_DIALOG };
	CEdit	m_body;
	CDateTimeCtrl	m_date;
	CDateTimeCtrl	m_time;
	Schedule* mem = NULL;//���Ϊ�գ����½�����Ϊ���Ǳ༭
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
