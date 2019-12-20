// RegisterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Mem.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "Net/ClientNetUtils.h"
#include <string>
#include "JSONLib/json.hpp"


// RegisterDlg 对话框

IMPLEMENT_DYNAMIC(RegisterDlg, CDialogEx)

RegisterDlg::RegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER, pParent)
{

}

RegisterDlg::~RegisterDlg()
{
}

void RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REG_PWD, txtPWD);
}


BEGIN_MESSAGE_MAP(RegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RegisterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RegisterDlg 消息处理程序


void RegisterDlg::OnBnClickedOk()
{
	CString pwd;
	txtPWD.GetWindowText(pwd);
	nlohmann::json req;
	req["name"] = "register";
	req["pwd"] = (std::string)(pwd.GetBuffer(0));
	try {
		auto res = request(req);

		char msg[50];
		sprintf_s(msg, "您的账号是：%d", res["uid"].get<int>());
		MessageBox(msg, 0);
		CDialogEx::OnOK();
	}
	catch (...) {
		MessageBox("无法连接到服务器", "错误", 0);
	}
	
}
