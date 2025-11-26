// CRightBottomPane.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFC.h"
#include "afxdialogex.h"
#include "CRightBottomPane.h"


// CRightBottomPane 대화 상자

IMPLEMENT_DYNAMIC(CRightBottomPane, CDialogEx)

BEGIN_MESSAGE_MAP(CRightBottomPane, CDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CRightBottomPane::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_listLog.SubclassDlgItem(IDC_LIST_LOG, this);
    m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle()
        | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_HEADERDRAGDROP);

    InitList();
    return TRUE;
}

void CRightBottomPane::InitList()
{
    m_listLog.InsertColumn(0, L"Time", LVCFMT_LEFT, 140);
    m_listLog.InsertColumn(1, L"Dir", LVCFMT_LEFT, 60);
    m_listLog.InsertColumn(2, L"Len", LVCFMT_RIGHT, 60);
    m_listLog.InsertColumn(3, L"Preview", LVCFMT_LEFT, 400);

    // 더미 로그 3줄
    m_listLog.InsertItem(0, L"12:00:00.000"); m_listLog.SetItemText(0, 1, L"TX"); m_listLog.SetItemText(0, 2, L"6");  m_listLog.SetItemText(0, 3, L"01 03 00 00 00 0A");
    m_listLog.InsertItem(1, L"12:00:00.005"); m_listLog.SetItemText(1, 1, L"RX"); m_listLog.SetItemText(1, 2, L"25"); m_listLog.SetItemText(1, 3, L"01 03 14 00 ...");
    m_listLog.InsertItem(2, L"12:00:01.005"); m_listLog.SetItemText(2, 1, L"RX"); m_listLog.SetItemText(2, 2, L"25"); m_listLog.SetItemText(2, 3, L"01 03 14 00 ...");
}

void CRightBottomPane::Layout(int cx, int cy)
{
    if (m_listLog.m_hWnd) m_listLog.MoveWindow(6, 6, max(0, cx - 12), max(0, cy - 12));
}

void CRightBottomPane::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    Layout(cx, cy);
}


// CRightBottomPane 메시지 처리기
