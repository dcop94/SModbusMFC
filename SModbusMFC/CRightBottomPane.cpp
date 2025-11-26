// CRightBottomPane.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFC.h"
#include "afxdialogex.h"
#include "CRightBottomPane.h"


// CRightBottomPane 대화 상자

IMPLEMENT_DYNAMIC(CRightBottomPane, CDialogEx)

CRightBottomPane::CRightBottomPane(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_BOTTOM_PANE, pParent)
{

}

CRightBottomPane::~CRightBottomPane()
{
}

void CRightBottomPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRightBottomPane, CDialogEx)
END_MESSAGE_MAP()


// CRightBottomPane 메시지 처리기
