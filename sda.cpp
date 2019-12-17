// sda.cpp: 实现文件
//

#include "pch.h"
#include "Mem.h"
#include "sda.h"
#include "afxdialogex.h"


// sda 对话框

IMPLEMENT_DYNAMIC(sda, CDialogEx)

sda::sda(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

sda::~sda()
{
}

void sda::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(sda, CDialogEx)
END_MESSAGE_MAP()


// sda 消息处理程序
