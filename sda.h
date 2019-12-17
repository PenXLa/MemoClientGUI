#pragma once


// sda 对话框

class sda : public CDialogEx
{
	DECLARE_DYNAMIC(sda)

public:
	sda(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~sda();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
