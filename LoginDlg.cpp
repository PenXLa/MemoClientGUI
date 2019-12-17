// LoginDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "Mem.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "Net/ClientNetUtils.h"
#include "JSONLib/json.hpp"
#include <string>
#include "Essentials/ClientDataBase.h"
#include "AllMem.h"
#include "RegisterDlg.h"


// LoginDlg 对话框

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERID, txtUID);
	DDX_Control(pDX, IDC_PWD, txtPWD);
}



// LoginDlg 消息处理程序
BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDLOGIN, &LoginDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_REG, &LoginDlg::OnBnClickedReg)
END_MESSAGE_MAP()


void LoginDlg::OnBnClickedLogin()
{
	CString cuid, cpwd;
	txtPWD.GetWindowText(cpwd);
	txtUID.GetWindowText(cuid);
	passwd = (std::string)(cpwd.GetBuffer(0));
	uid = atoi(cuid);
	nlohmann::json req;
	req["name"] = "login";
	req["pwd"] = passwd;
	req["uid"] = uid;
	auto res = request(req);
	if (res["success"].get<bool>()) {
		loggedin = true;
		DataBase::sync();
		CDialog::OnOK();
	}
	else {
		MessageBox(res["reason"].get<std::string>().c_str(), "登录失败",0);
	}
}



void LoginDlg::OnBnClickedReg()
{
	RegisterDlg dlg;
	dlg.DoModal();
}
