// CLeftPane.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFC.h"
#include "afxdialogex.h"
#include "CLeftPane.h"


// CLeftPane 대화 상자

IMPLEMENT_DYNAMIC(CLeftPane, CDialogEx)

CLeftPane::CLeftPane(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_PANE, pParent)
{

}

CLeftPane::~CLeftPane()
{
}

void CLeftPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLeftPane, CDialogEx)
END_MESSAGE_MAP()


// CLeftPane 메시지 처리기
