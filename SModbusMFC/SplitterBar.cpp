#include "pch.h"              // ★ 반드시 첫 줄에 pch 포함
#include "SplitterBar.h"

BEGIN_MESSAGE_MAP(CSplitterBar, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

BOOL CSplitterBar::Create(ORIENT o, CWnd* pParent, UINT nID)
{
    m_orient = o;

    // 커서: 수직 분할(좌우)면 SIZEWE, 수평 분할(상하)면 SIZENS
    HCURSOR hCur = ::LoadCursor(NULL, o == VERT ? IDC_SIZEWE : IDC_SIZENS);

    // 간단한 등록 클래스 사용
    CString cls = AfxRegisterWndClass(
        CS_DBLCLKS,
        hCur,
        (HBRUSH)(COLOR_3DFACE + 1),
        0);

    // Child + Visible 로 생성
    return CWnd::CreateEx(
        WS_EX_TRANSPARENT,   // 배경 투명(깜빡임 감소용)
        cls, _T(""),
        WS_CHILD | WS_VISIBLE,
        CRect(0, 0, 0, 0),
        pParent, nID);
}

void CSplitterBar::OnPaint()
{
    CPaintDC dc(this);
    CRect rc; GetClientRect(&rc);

    // 기본 3DFACE 색으로 채우고
    dc.FillSolidRect(rc, ::GetSysColor(COLOR_3DFACE));
    // 테두리 음각
    dc.DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
}

void CSplitterBar::OnLButtonDown(UINT, CPoint pt)
{
    SetCapture();
    m_drag = true;

    // 부모 클라 영역 저장
    GetParent()->GetClientRect(&m_parentRect);

    // 드래그 시작점은 스크린 좌표로 저장(계산 편하게)
    ClientToScreen(&pt);
    m_dragStart = pt;
}

void CSplitterBar::OnLButtonUp(UINT, CPoint)
{
    if (m_drag) {
        ReleaseCapture();
        m_drag = false;
    }
}

void CSplitterBar::OnMouseMove(UINT, CPoint pt)
{
    if (!m_drag) return;

    // 현재 마우스 위치(스크린 좌표)
    CPoint scr = pt; ClientToScreen(&scr);

    // 현재 스플리터의 화면 위치
    CRect rc; GetWindowRect(&rc);

    const int DEADZONE = 6;
    if (m_orient == VERT)
    {
        if (abs(scr.x - m_dragStart.x) < DEADZONE) return;
    }
    else
    {
        if (abs(scr.y - m_dragStart.y) < DEADZONE) return;
    }

    double pct;


    if (m_orient == VERT) {
        // 수직 스플리터: X 이동량을 비율로 환산 (좌폭 비율)
        int newX = rc.CenterPoint().x + (scr.x - m_dragStart.x);
        pct = double(newX - m_parentRect.left) / double(m_parentRect.Width());
    }
    else {
        // 수평 스플리터: Y 이동량을 비율로 환산 (상단 높이 비율)
        int newY = rc.CenterPoint().y + (scr.y - m_dragStart.y);
        pct = double(newY - m_parentRect.top) / double(m_parentRect.Height());
    }

    pct = max(0.1, min(0.9, pct));
    pct = floor(pct * 100.0 + 0.5) / 100.0;

    SetPercent(pct);
    m_dragStart = scr;

    // 부모에게 "크기 바뀌었으니 다시 배치해" 신호
    GetParent()->SendMessage(WM_SIZE, 0, 0);
}

void CSplitterBar::OnLButtonDbClk(UINT, CPoint)
{
    // 더블클릭하면 기본 위치로 리셋
    SetPercent(m_defaultPercent);
    GetParent()->SendMessage(WM_SIZE, 0, 0);
}