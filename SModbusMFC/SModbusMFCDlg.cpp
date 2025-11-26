
// SModbusMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SModbusMFC.h"
#include "SModbusMFCDlg.h"
#include "afxdialogex.h"

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
	AfxMessageBox(L"단독 연결");
}

void CSModbusMFCDlg::OnConnMulti()
{
	AfxMessageBox(L"다중 연결");
}

void CSModbusMFCDlg::OnSetupOptions()
{
	AfxMessageBox(L"옵션");
}

// CSModbusMFCDlg 메시지 처리기

BOOL CSModbusMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	const int W = 1200, H = 800;
	CRect r; GetWindowRect(&r);
	MoveWindow(r.left, r.top, W, H, TRUE);

	// 자식 대화상자 3개 생성 및 표시
	m_left.Create(IDD_LEFT_PANE, this);
	m_top.Create(IDD_RIGHT_TOP_PANE, this);
	m_bottom.Create(IDD_RIGHT_BOTTOM_PANE, this);
	m_left.ShowWindow(SW_SHOW);
	m_top.ShowWindow(SW_SHOW);
	m_bottom.ShowWindow(SW_SHOW);

	// 스플리터 2개 생성 
	m_vsplit.Create(CSplitterBar::VERT, this, 30001); // 좌우 분할
	m_hsplit.Create(CSplitterBar::HORZ, this, 30002); // 우측 상하 분할
	
	// 최초 비율 세팅
	CRect rc; GetClientRect(&rc);
	double vPct = (double)m_leftWidthPx / max(1, rc.Width()); // 좌폭 비율
	double hPct = 1.0 - (double)m_bottomHeightPx / max(1, rc.Height()); // 상단 비율 
	
	// 클램프 + 스냅
	vPct = max(0.1, min(0.9, vPct));
	hPct = max(0.1, min(0.9, hPct));
	m_vsplit.SetDefaultPercent(vPct);
	m_hsplit.SetDefaultPercent(hPct);
	m_vsplit.SetPercent(vPct);
	m_hsplit.SetPercent(hPct);
	
	// 최초 레이아웃 배치
	DoLayout();

	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
	if (!m_left.m_hWnd) return;

	CRect rc; GetClientRect(&rc);

	// 여백과 스플리터 두께
	const int PAD = 4; // 창 외곽 여백
	const int SPL = 6; // 스플리터 두께

	// 현재 비율을 픽셀로 변환
	int leftW = (int)(rc.Width() * m_vsplit.GetPercent());
	int topH = (int)(rc.Height() * m_hsplit.GetPercent());

	// 최소 크기 보정 (왼쪽, 오른쪽-상단, 하단)
	leftW = max(leftW, m_minLeftPx);

	// 우측 사각형
	CRect rcLeft(rc.left + PAD, rc.top + PAD,
		rc.left + leftW - PAD, rc.bottom - PAD);
	CRect rcV(rcLeft.right, rc.top + PAD,
		rcLeft.right + SPL, rc.bottom - PAD);
	CRect rcRight(rcV.right + PAD, rc.top + PAD,
		rc.right - PAD, rc.bottom - PAD);

	// 상단 높이 보정: 상단은 최소 m_minRightTopPx, 하단은 최소 m_minBottomPx
	int rightHeight = rcRight.Height();
	int bottomH = rightHeight - topH - PAD - SPL;
	if (topH < m_minRightTopPx) 
	{
		topH = m_minRightTopPx;
		bottomH = rightHeight - topH - PAD - SPL;
	}
	if (bottomH < m_minBottomPx) 
	{
		bottomH = m_minBottomPx;
		topH = rightHeight - bottomH - PAD - SPL;
		topH = max(topH, m_minRightTopPx);
	}

	// 실제 배치
	CRect rcTop(rcRight.left, rcRight.top,
		rcRight.right, rcRight.top + topH);
	CRect rcH(rcRight.left, rcTop.bottom,
		rcRight.right, rcTop.bottom + SPL);
	CRect rcBot(rcRight.left, rcH.bottom,
		rcRight.right, rcRight.bottom);

	m_left.MoveWindow(rcLeft);
	m_top.MoveWindow(rcTop);
	m_bottom.MoveWindow(rcBot);
	m_vsplit.MoveWindow(rcV);
	m_hsplit.MoveWindow(rcH);

	// ❸ 현재 픽셀 값을 갱신(사용자 드래그 후에도 “고정폭/고정높이 느낌” 유지에 활용 가능)
	m_leftWidthPx = rcLeft.Width();
	m_bottomHeightPx = rcBot.Height();


}

void CSModbusMFCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	DoLayout();
}

