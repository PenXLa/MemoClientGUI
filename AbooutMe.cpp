#include "stdafx.h"
#include "Mem.h"
#include "AbooutMe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAbooutMe::CAbooutMe(CWnd* pParent /*=NULL*/)
	: CDialog(CAbooutMe::IDD, pParent)
{

}

void CAbooutMe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAbooutMe, CDialog)
END_MESSAGE_MAP()
