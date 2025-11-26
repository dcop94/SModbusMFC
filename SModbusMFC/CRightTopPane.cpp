// CRightTopPane.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFC.h"
#include "afxdialogex.h"
#include "CRightTopPane.h"


// CRightTopPane 대화 상자

IMPLEMENT_DYNAMIC(CRightTopPane, CDialogEx)

CRightTopPane::CRightTopPane(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_TOP_PANE, pParent)
{

}

CRightTopPane::~CRightTopPane()
{
}

void CRightTopPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRightTopPane, CDialogEx)
END_MESSAGE_MAP()


// CRightTopPane 메시지 처리기
