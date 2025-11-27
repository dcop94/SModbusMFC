// CRightTopPane.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFCDlg.h"
#include "afxdialogex.h"
#include "CRightTopPane.h"
#include "ModbusDocWnd.h"
#include "CommonTypes.h"


CRightTopPane::~CRightTopPane() = default;


// CRightTopPane 대화 상자

IMPLEMENT_DYNAMIC(CRightTopPane, CDialogEx)

BEGIN_MESSAGE_MAP(CRightTopPane, CDialogEx)
    ON_WM_SIZE()
    ON_MESSAGE(WM_TOPPANE_TILE, &CRightTopPane::OnTile)
    ON_MESSAGE(WM_TOPPANE_CASCADE, &CRightTopPane::OnCascade)
    ON_MESSAGE(WM_TOPPANE_TILE_H, &CRightTopPane::OnTileH)
    ON_MESSAGE(WM_TOPPANE_TILE_V, &CRightTopPane::OnTileV)
    ON_MESSAGE(WM_TOPPANE_CLOSE_ACTIVE, &CRightTopPane::OnCloseActive)
    ON_MESSAGE(WM_TOPPANE_CLOSE_ALL, &CRightTopPane::OnCloseAll)
    ON_MESSAGE(WM_TOPPANE_ACTIVATE_NEXT, &CRightTopPane::OnActivateNext)
    ON_MESSAGE(WM_TOPPANE_SET_ACTIVE, &CRightTopPane::OnSetActive)
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

int CRightTopPane::FindDocIndexByHwnd(HWND h) const
{
    for (int i = 0;i < (int)m_docs.size();++i)
        if (m_docs[i] && m_docs[i]->m_hWnd == h) return i;
    return -1;
}

LRESULT CRightTopPane::OnSetActive(WPARAM wParam, LPARAM)
{
    m_hActive = (HWND)wParam;
    if (::IsWindow(m_hActive)) ::SetFocus(m_hActive);
    return 0;
}

LRESULT CRightTopPane::OnCascade(WPARAM, LPARAM)
{
    if (m_docs.empty()) return 0;
    CRect rc; GetClientRect(&rc);
    const int pad = 10, step = 28;

    int k = 0;
    for (auto& d : m_docs)
    {
        if (!d || !::IsWindow(d->m_hWnd)) continue;
        CRect r(rc.left + pad + k * step, rc.top + pad + k * step,
            rc.left + pad + k * step + (rc.Width() * 2 / 3),
            rc.top + pad + k * step + (rc.Height() * 2 / 3));
        d->MoveWindow(r, TRUE);
        ++k;
    }
    return 0;
}

LRESULT CRightTopPane::OnTileH(WPARAM, LPARAM)
{
    if (m_docs.empty()) return 0;
    CRect rc; GetClientRect(&rc);
    const int pad = 10;
    int n = (int)m_docs.size();
    int cellH = (rc.Height() - pad * (n + 1)) / n;
    int y = rc.top + pad;

    for (auto& d : m_docs)
    {
        if (!d || !::IsWindow(d->m_hWnd)) continue;
        CRect r(rc.left + pad, y, rc.right - pad, y + cellH);
        d->MoveWindow(r, TRUE);
        y += cellH + pad;
    }
    return 0;
}

LRESULT CRightTopPane::OnTileV(WPARAM, LPARAM)
{
    if (m_docs.empty()) return 0;
    CRect rc; GetClientRect(&rc);
    const int pad = 10;
    int n = (int)m_docs.size();
    int cellW = (rc.Width() - pad * (n + 1)) / n;
    int x = rc.left + pad;

    for (auto& d : m_docs)
    {
        if (!d || !::IsWindow(d->m_hWnd)) continue;
        CRect r(x, rc.top + pad, x + cellW, rc.bottom - pad);
        d->MoveWindow(r, TRUE);
        x += cellW + pad;
    }
    return 0;
}

LRESULT CRightTopPane::OnCloseActive(WPARAM, LPARAM)
{
    if (!::IsWindow(m_hActive)) return 0;
    int idx = FindDocIndexByHwnd(m_hActive);
    if (idx >= 0) {
        m_docs[idx]->DestroyWindow();
        m_docs.erase(m_docs.begin() + idx);
        m_hActive = nullptr;
    }
    return 0;
}

LRESULT CRightTopPane::OnCloseAll(WPARAM, LPARAM)
{
    for (auto& d : m_docs) if (d && ::IsWindow(d->m_hWnd)) d->DestroyWindow();
    m_docs.clear();
    m_hActive = nullptr;
    return 0;
}

LRESULT CRightTopPane::OnActivateNext(WPARAM, LPARAM)
{
    if (m_docs.empty()) return 0;
    // 현재 활성 다음 인덱스
    int idx = FindDocIndexByHwnd(m_hActive);
    int next = (idx + 1) % (int)m_docs.size();
    if (m_docs[next] && ::IsWindow(m_docs[next]->m_hWnd)) {
        m_hActive = m_docs[next]->m_hWnd;
        m_docs[next]->BringWindowToTop();
        ::SetFocus(m_hActive);
    }
    return 0;
}

void CRightTopPane::ApplyConnectionToActive(const ConnParams& p)
{
    UNREFERENCED_PARAMETER(p);
    // TODO: 활성 문서창에 설정 전달/컨텍스트 준비
}

void CRightTopPane::ApplyOptionsToActive(const PollOptions& o)
{
    if (!::IsWindow(m_hActive)) return;
    int idx = FindDocIndexByHwnd(m_hActive);
    if (idx < 0) return;
    if (auto* doc = m_docs[idx].get())
        doc->ApplyOptions(o);
}
// CRightTopPane 메시지 처리기
