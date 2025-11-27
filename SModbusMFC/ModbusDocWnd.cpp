#include "pch.h"
#include "ModbusDocWnd.h"
#include "CommonTypes.h"

BEGIN_MESSAGE_MAP(CModbusDocWnd, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_LBUTTONDOWN()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_MESSAGE(WM_DOC_UPDATE_ROW, &CModbusDocWnd::OnUpdateRow)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CModbusDocWnd::Create(CWnd* pParent, const CRect& rcInit, const CString& title)
{
    // 캡션/리사이즈/최소/최대 박스가 있는 자식창
    DWORD style = WS_CHILD | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU |
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    DWORD exstyle = WS_EX_CLIENTEDGE;

    // 간단한 윈도우 클래스 등록
    CString cls = AfxRegisterWndClass(
        CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_3DFACE + 1),0);

    return CWnd::CreateEx(exstyle, cls, title, style, rcInit, pParent, 0);
}


int CModbusDocWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

    // 내부 리스트(Report) 생성
    CRect r; GetClientRect(&r);
    m_grid.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, r, this, 1001);

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

void CModbusDocWnd::OnLButtonDown(UINT, CPoint)
{
    // 본문 클릭 시 활성 알림
    if (GetParent()) GetParent()->PostMessage(WM_TOPPANE_SET_ACTIVE, (WPARAM)m_hWnd, 0);
}

void CModbusDocWnd::OnNcLButtonDown(UINT nHitTest, CPoint pt)
{
    // 타이틀바 클릭 시도에도 활성 알림
    if (GetParent()) GetParent()->PostMessage(WM_TOPPANE_SET_ACTIVE, (WPARAM)m_hWnd, 0);
    CWnd::OnNcLButtonDown(nHitTest, pt);
}

void CModbusDocWnd::OnClose()
{
    // X 버튼 → 부모에게 “활성 닫기” 요청으로 위임
    if (GetParent()) {
        GetParent()->PostMessage(WM_TOPPANE_SET_ACTIVE, (WPARAM)m_hWnd, 0);
        GetParent()->PostMessage(WM_TOPPANE_CLOSE_ACTIVE, 0, 0);
    }
    else {
        DestroyWindow();
    }
}

static CString NowMs()
{
    SYSTEMTIME st; GetLocalTime(&st);
    CString s; s.Format(L"%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return s;
}

void CModbusDocWnd::AppendLogTX(const CString& hex)
{
    // 메인 Dlg로 전파
    CString* p = new CString[4];
    p[0] = NowMs();        // time
    p[1] = L"TX";          // dir
    p[2].Format(L"%d", hex.GetLength() / 3); // len(대충 바이트 수 추정) 
    p[3] = hex;            // preview
    GetParent()->GetParent()->PostMessage(WM_APPEND_LOG, (WPARAM)p, 0);
}

void CModbusDocWnd::AppendLogRX(const CString& hex)
{
    CString* p = new CString[4];
    p[0] = NowMs();
    p[1] = L"RX";
    p[2].Format(L"%d", hex.GetLength() / 3);
    p[3] = hex;
    GetParent()->GetParent()->PostMessage(WM_APPEND_LOG, (WPARAM)p, 0);
}

void CModbusDocWnd::OnTimer(UINT_PTR id)
{
    // ★ 지금은 가짜 요청/응답을 만들어 로그만 쌓음
    //    (후에 실제 libmodbus 호출로 바꿔치기)
    CString tx, rx;
    tx.Format(L"01 03 %02X %02X 00 0A", m_addr & 0xFF, (m_addr >> 8) & 0xFF);
    rx = L"01 03 14 00 00 00 00 00 00 00 00"; // 예시

    AppendLogTX(tx);
    AppendLogRX(rx);
    CWnd::OnTimer(id);
}

void CModbusDocWnd::ApplyOptions(const PollOptions& o)
{
    // 읽기 파라미터 갱신
    m_slave = o.slave;
    m_func = o.func;
    m_addr = o.addr;
    m_qty = o.qty;
    m_scanMs = o.scanMs;

    // 행 수 변경 시 그리드 재구성
    InitGrid();

    // 실행 중이면 다음 루프부터 새 파라미터로 동작
    // (스레드에서 m_* 멤버를 참조하므로 즉시 반영됨)
}

void CModbusDocWnd::StartPoll()
{
    StopPoll();
    m_run = true;
    m_thr = std::thread(&CModbusDocWnd::PollLoop, this);
}

void CModbusDocWnd::StopPoll()
{
    if (m_run.exchange(false)) {
        if (m_thr.joinable()) m_thr.join();
    }
}

void CModbusDocWnd::OnDestroy()
{
    StopPoll();
    CWnd::OnDestroy();
}

// 스레드 → UI 안전 갱신
void CModbusDocWnd::UpdateGridRow(int row, int value)
{
    PostMessage(WM_DOC_UPDATE_ROW, (WPARAM)row, (LPARAM)value);
}

LRESULT CModbusDocWnd::OnUpdateRow(WPARAM w, LPARAM l)
{
    int row = (int)w, val = (int)l;
    if (m_grid.m_hWnd && row >= 0 && row < m_qty) {
        CString s; s.Format(L"%d", val);
        m_grid.SetItemText(row, 1, s);
    }
    return 0;
}

void CModbusDocWnd::PollLoop()
{
    // ex) modbus_set_slave(m_ctx, m_slave);

    int tick = 0;
    while (m_run) {
        // ---- 실제 장비 읽기 붙일 자리 ----
        // int rc = 0;
        // if (m_func == 3) {
        //     std::vector<uint16_t> buf(m_qty);
        //     rc = modbus_read_registers(m_ctx, m_addr, m_qty, buf.data());
        //     if (rc == m_qty) for (int i=0;i<m_qty;++i) UpdateGridRow(i, buf[i]);
        // }
        // else if (m_func == 4) { ... }
        // else if (m_func == 1/2) { ... (bit) }

        // ---- 데모: 증가값 채우기 ----
        for (int i = 0; i < m_qty; ++i) {
            UpdateGridRow(i, (tick + i) % 1000);
        }
        ++tick;

        // 주기
        for (int i = 0; i < 10 && m_run; ++i) Sleep(max(1, m_scanMs / 10));
    }
}
