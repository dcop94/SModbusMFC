// CRightTopPane.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFC.h"
#include "afxdialogex.h"
#include "CRightTopPane.h"


// CRightTopPane 대화 상자

IMPLEMENT_DYNAMIC(CRightTopPane, CDialogEx)

BEGIN_MESSAGE_MAP(CRightTopPane, CDialogEx)
    ON_MESSAGE(WM_TOPPANE_TILE, &CRightTopPane::OnTile)
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CRightTopPane::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CreateOneDoc(L"Mbpoll1");
    return TRUE;

    //// 리소스의 IDC_LIST_DATA와 연결
    //m_listData.SubclassDlgItem(IDC_LIST_DATA, this);

    //// 확장 스타일
    //m_listData.SetExtendedStyle(m_listData.GetExtendedStyle()
    //    | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_HEADERDRAGDROP);

    //InitList();
    //return TRUE;
}

void CRightTopPane::InitList()
{
    // 컬럼 구성 (Name, 00000 … Poll 기본 느낌)
    m_listData.InsertColumn(0, L"Name", LVCFMT_LEFT, 140);
    m_listData.InsertColumn(1, L"00000", LVCFMT_RIGHT, 80);

    // 더미 10행
    for (int i = 0;i < 10;++i) {
        CString row; row.Format(L"%d", i);
        m_listData.InsertItem(i, row);
        m_listData.SetItemText(i, 1, L"0");
    }
}

void CRightTopPane::Layout(int cx, int cy)
{
    if (m_listData.m_hWnd) m_listData.MoveWindow(6, 6, max(0, cx - 12), max(0, cy - 12));
}


void CRightTopPane::CreateOneDoc(const CString& title)
{
    CRect rc; GetClientRect(&rc);

    //여백 
    CRect init(rc.left + 12, rc.top + 12, rc.left + max(480, rc.Width() / 2), rc.top + max(300, rc.Height() / 2));

    std::unique_ptr<CModbusDocWnd> wnd(new CModbusDocWnd());

    // 캡션
    CString cap = title;
    if (cap.IsEmpty()) {
        int idx = static_cast<int>(m_docs.size()) + 1;
        cap.Format(L"Mbpoll%d", idx);
    }

    if (wnd->Create(this, init, cap)) {
        wnd->ShowWindow(SW_SHOW);
        m_docs.emplace_back(std::move(wnd));
    }
}

void CRightTopPane::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // 부모 크기 변경 시, 문서창이 부모 밖으로 나가면
    // 각 창의 OnWindowPosChanging에서 클램프됩니다.
    for (auto& d : m_docs) {
        if (!d || !::IsWindow(d->m_hWnd)) continue;
        CRect r; d->GetWindowRect(&r);
        ScreenToClient(&r);
        d->MoveWindow(r); // 위치 유지(클램프 유효화)
    }
}

LRESULT CRightTopPane::OnTile(WPARAM, LPARAM)
{
    if (m_docs.empty()) return 0;

    CRect rc; GetClientRect(&rc);
    const int pad = 10;

    // 간단한 2열 타일 (개수에 따라 행/열 계산)
    int n = (int)m_docs.size();
    int cols = (n >= 3) ? 2 : 1;
    int rows = (n + cols - 1) / cols;

    int cellW = (rc.Width() - pad * (cols + 1)) / cols;
    int cellH = (rc.Height() - pad * (rows + 1)) / rows;

    int k = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols && k < n; ++c, ++k) {
            CRect cell(rc.left + pad + c * (cellW + pad),
                rc.top + pad + r * (cellH + pad),
                rc.left + pad + c * (cellW + pad) + cellW,
                rc.top + pad + r * (cellH + pad) + cellH);
            auto& d = m_docs[k];
            if (d && ::IsWindow(d->m_hWnd)) d->MoveWindow(cell, TRUE);
        }
    }
    return 0;
}

// CRightTopPane 메시지 처리기
