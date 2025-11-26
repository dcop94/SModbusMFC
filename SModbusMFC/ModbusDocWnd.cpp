#include "pch.h"
#include "ModbusDocWnd.h"

BEGIN_MESSAGE_MAP(CModbusDocWnd, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

BOOL CModbusDocWnd::Create(CWnd* pParent, const CRect& rcInit, const CString& title)
{
    // 캡션/리사이즈/최소/최대 박스가 있는 자식창
    DWORD style = WS_CHILD | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    DWORD exstyle = WS_EX_CLIENTEDGE;

    // 간단한 윈도우 클래스 등록
    CString cls = AfxRegisterWndClass(
        CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_3DFACE + 1),
        0);

    return CWnd::CreateEx(exstyle, cls, title, style, rcInit, pParent, 0);
}

int CModbusDocWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

    // 내부 리스트(Report) 생성
    CRect r; GetClientRect(&r);
    m_grid.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        r, this, 1001);

    DWORD ex = m_grid.GetExtendedStyle();
    m_grid.SetExtendedStyle(ex | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
        LVS_EX_DOUBLEBUFFER | LVS_EX_HEADERDRAGDROP);

    InitGrid();
    return 0;
}

void CModbusDocWnd::InitGrid()
{
    m_grid.InsertColumn(0, L"Name", LVCFMT_LEFT, 140);
    m_grid.InsertColumn(1, L"00000", LVCFMT_RIGHT, 80);

    for (int i = 0; i < 10; ++i) {
        CString s; s.Format(L"%d", i);
        m_grid.InsertItem(i, s);
        m_grid.SetItemText(i, 1, L"0");
    }
}

void CModbusDocWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_grid.m_hWnd) {
        // 캡션/프레임 고려 여백
        m_grid.MoveWindow(4, 24, max(0, cx - 8), max(0, cy - 28));
    }
}

// 부모 Pane 영역 밖으로 못 나가게 클램프
void CModbusDocWnd::OnWindowPosChanging(WINDOWPOS* wp)
{
    CWnd::OnWindowPosChanging(wp);

    CRect rcParent; GetParent()->GetClientRect(&rcParent);
    CRect rcNew(wp->x, wp->y, wp->x + wp->cx, wp->y + wp->cy);

    const int MINW = 300, MINH = 180;
    rcNew.right = max(rcNew.left + MINW, rcNew.right);
    rcNew.bottom = max(rcNew.top + MINH, rcNew.bottom);

    if (rcNew.left < rcParent.left)   rcNew.OffsetRect(rcParent.left - rcNew.left, 0);
    if (rcNew.top < rcParent.top)    rcNew.OffsetRect(0, rcParent.top - rcNew.top);
    if (rcNew.right > rcParent.right)  rcNew.OffsetRect(rcParent.right - rcNew.right, 0);
    if (rcNew.bottom > rcParent.bottom) rcNew.OffsetRect(0, rcParent.bottom - rcNew.bottom);

    wp->x = rcNew.left;  wp->y = rcNew.top;
    wp->cx = rcNew.Width(); wp->cy = rcNew.Height();
}
