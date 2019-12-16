#pragma once
class CAbooutMe : public CDialog
{
public:
	CAbooutMe(CWnd* pParent = NULL);
	enum { IDD = IDD_DIALOG_ABOUT };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};
