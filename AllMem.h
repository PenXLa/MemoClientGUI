#pragma once

class CAllMem : public CDialog
{
public:
	CAllMem(CWnd* pParent = NULL);
	void refresh();
public:
	int FindMyMem(int& nSel, POSITION& pos);
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_list;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDel();
	virtual void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnButtonRemoveall();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButNew();
};
