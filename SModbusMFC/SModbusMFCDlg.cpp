
// SModbusMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SModbusMFC.h"
#include "SModbusMFCDlg.h"
#include "afxdialogex.h"
#include "CConnDlg.h"
#include "CommonTypes.h"
#include "COptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSModbusMFCDlg 대화 상자



CSModbusMFCDlg::CSModbusMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMODBUSMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSModbusMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSModbusMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CSModbusMFCDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CSModbusMFCDlg::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CSModbusMFCDlg::OnFileSaveAs)
	ON_COMMAND(ID_CONN_SINGLE, &CSModbusMFCDlg::OnConnSingle)
	ON_COMMAND(ID_CONN_MULTI, &CSModbusMFCDlg::OnConnMulti)
	ON_COMMAND(ID_SETUP_OPTIONS, &CSModbusMFCDlg::OnSetupOptions)

	ON_COMMAND(ID_WINDOW_NEW, &CSModbusMFCDlg::OnWindowNew)
	ON_COMMAND(ID_WINDOW_TILE, &CSModbusMFCDlg::OnWindowTile)
	ON_COMMAND(ID_WINDOW_CASCADE, &CSModbusMFCDlg::OnWindowCascade)
	ON_COMMAND(ID_WINDOW_TILE_H, &CSModbusMFCDlg::OnWindowTileH)
	ON_COMMAND(ID_WINDOW_TILE_V, &CSModbusMFCDlg::OnWindowTileV)
	ON_COMMAND(ID_WINDOW_CLOSE, &CSModbusMFCDlg::OnWindowClose)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, &CSModbusMFCDlg::OnWindowCloseAll)
	ON_COMMAND(ID_WINDOW_NEXT, &CSModbusMFCDlg::OnWindowNext)

	ON_MESSAGE(WM_APPEND_LOG, &CSModbusMFCDlg::OnAppendLog)

	ON_WM_SIZE()
END_MESSAGE_MAP()

void CSModbusMFCDlg::OnFileOpen()
{
	AfxMessageBox(L"열기");
}

void CSModbusMFCDlg::OnFileSave()
{
	AfxMessageBox(L"저장");
}

void CSModbusMFCDlg::OnFileSaveAs()
{
	AfxMessageBox(L"다른 이름으로 저장");
}

void CSModbusMFCDlg::OnConnSingle()
{
	CConnDlg dlg; 
	dlg.params = m_conn;
	if (dlg.DoModal() == IDOK) {
		m_conn = dlg.params;
		m_top.ApplyConnectionToActive(m_conn); // 활성 문서창으로 전달
	}
}

void CSModbusMFCDlg::OnConnMulti()
{
	AfxMessageBox(L"다중 연결");
}

void CSModbusMFCDlg::OnSetupOptions()
{
	COptionsDlg dlg;
	dlg.opts = m_opts;            // 기존값 채우기
	if (dlg.DoModal() == IDOK) {
		m_opts = dlg.opts;        // 저장
		// 활성 문서에 즉시 반영
		m_top.ApplyOptionsToActive(m_opts);
	}
}

// CSModbusMFCDlg 메시지 처리기

BOOL CSModbusMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// 보기 좋은 시작 크기
	MoveWindow(0, 0, 1280, 800, TRUE);
	CenterWindow();

	// 자식 Pane 생성
	m_left.Create(IDD_LEFT_PANE, this);
	m_top.Create(IDD_RIGHT_TOP_PANE, this);
	m_bottom.Create(IDD_RIGHT_BOTTOM_PANE, this);

	// ★ 확장스타일(디자이너에서 못했을 때 코드로 강제)
	m_top.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	m_top.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_bottom.ModifyStyleEx(0, WS_EX_CONTROLPARENT | WS_EX_CLIENTEDGE);

	// Poll 모드면 좌측 패널 숨기기
	if (m_pollMode) m_left.ShowWindow(SW_HIDE);
	m_top.ShowWindow(SW_SHOW);
	m_bottom.ShowWindow(SW_SHOW);

	// 스플리터 생성
	m_vsplit.Create(CSplitterBar::VERT, this, 30001);
	m_hsplit.Create(CSplitterBar::HORZ, this, 30002);

	// 최초 비율 세팅
	CRect rc; GetClientRect(&rc);
	double vPct = (double)m_leftWidthPx / max(1, rc.Width());
	double hPct = 1.0 - (double)m_bottomHeightPx / max(1, rc.Height());
	vPct = max(0.1, min(0.9, vPct));
	hPct = max(0.1, min(0.9, hPct));
	m_vsplit.SetDefaultPercent(vPct);
	m_hsplit.SetDefaultPercent(hPct);
	m_vsplit.SetPercent(vPct);
	m_hsplit.SetPercent(hPct);

	DoLayout();
	return TRUE;
}


void CSModbusMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSModbusMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSModbusMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSModbusMFCDlg::DoLayout()
{
	if (!m_top.m_hWnd || !m_bottom.m_hWnd) return;

	CRect rc; GetClientRect(&rc);
	const int PAD = 4;
	const int SPL = 6;

	// Poll 모드면 좌측 제거(= 전체 영역을 우영역으로)
	CRect rcLeft, rcRight;
	CRect rcV; // 수직 스플리터 영역

	if (!m_pollMode) {
		if (!m_left.m_hWnd) return;
		int leftW = (int)(rc.Width() * m_vsplit.GetPercent());
		leftW = max(leftW, m_minLeftPx);

		rcLeft = CRect(rc.left + PAD, rc.top + PAD,
			rc.left + leftW - PAD, rc.bottom - PAD);
		rcV = CRect(rcLeft.right, rc.top + PAD,
			rcLeft.right + SPL, rc.bottom - PAD);
		rcRight = CRect(rcV.right + PAD, rc.top + PAD,
			rc.right - PAD, rc.bottom - PAD);

		m_left.MoveWindow(rcLeft);
		m_vsplit.MoveWindow(rcV);
		m_leftWidthPx = rcLeft.Width();
		m_vsplit.ShowWindow(SW_SHOW);
		m_left.ShowWindow(SW_SHOW);
	}
	else {
		// 좌측 숨김
		rcRight = CRect(rc.left + PAD, rc.top + PAD,
			rc.right - PAD, rc.bottom - PAD);
		m_vsplit.ShowWindow(SW_HIDE);
		if (m_left.m_hWnd) m_left.ShowWindow(SW_HIDE);
	}

	// 우측 상/하 분할
	int topH = (int)(rcRight.Height() * m_hsplit.GetPercent());
	int rightH = rcRight.Height();
	int bottomH = rightH - topH - PAD - SPL;

	// 최소 높이 보정
	if (topH < m_minRightTopPx) {
		topH = m_minRightTopPx;
		bottomH = rightH - topH - PAD - SPL;
	}
	if (bottomH < m_minBottomPx) {
		bottomH = m_minBottomPx;
		topH = rightH - bottomH - PAD - SPL;
		topH = max(topH, m_minRightTopPx);
	}

	CRect rcTop(rcRight.left, rcRight.top,
		rcRight.right, rcRight.top + topH);
	CRect rcH(rcRight.left, rcTop.bottom,
		rcRight.right, rcTop.bottom + SPL);
	CRect rcBot(rcRight.left, rcH.bottom,
		rcRight.right, rcRight.bottom);

	m_top.MoveWindow(rcTop);
	m_hsplit.MoveWindow(rcH);
	m_bottom.MoveWindow(rcBot);

	m_bottomHeightPx = rcBot.Height();
}


void CSModbusMFCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	DoLayout();
}

void CSModbusMFCDlg::OnWindowNew()
{
	m_top.CreateOneDoc();
}

void CSModbusMFCDlg::OnWindowTile()
{
	m_top.PostMessage(WM_TOPPANE_TILE, 0, 0);
}

void CSModbusMFCDlg::OnWindowCascade() 
{ 
	m_top.PostMessage(WM_TOPPANE_CASCADE, 0, 0); 
}

void CSModbusMFCDlg::OnWindowTileH() 
{ 
	m_top.PostMessage(WM_TOPPANE_TILE_H, 0, 0); 
}

void CSModbusMFCDlg::OnWindowTileV() 
{ 
	m_top.PostMessage(WM_TOPPANE_TILE_V, 0, 0); 
}

void CSModbusMFCDlg::OnWindowClose() 
{ 
	m_top.PostMessage(WM_TOPPANE_CLOSE_ACTIVE, 0, 0); 
}

void CSModbusMFCDlg::OnWindowCloseAll() 
{ 
	m_top.PostMessage(WM_TOPPANE_CLOSE_ALL, 0, 0); 
}

void CSModbusMFCDlg::OnWindowNext() 
{ 
	m_top.PostMessage(WM_TOPPANE_ACTIVATE_NEXT, 0, 0); 
}

LRESULT CSModbusMFCDlg::OnAppendLog(WPARAM wParam, LPARAM lParam)
{
	// wParam/lParam에 포인터가 들어옵니다: new CString[4]{time,dir,lenS,preview}
	CString* p = reinterpret_cast<CString*>(wParam);
	if (p) {
		int len = _wtoi(p[2]);               // len 문자열을 int로 변환
		m_bottom.AddLog(p[0], p[1], len, p[3]);
		delete[] p;
	}
	return 0;
}




