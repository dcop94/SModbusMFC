
// SModbusMFCDlg.h: 헤더 파일
//

#pragma once
#include "CLeftPane.h"
#include "CRightTopPane.h"
#include "CRightBottomPane.h"
#include "SplitterBar.h"

// CSModbusMFCDlg 대화 상자
class CSModbusMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CSModbusMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMODBUSMFC_DIALOG };
#endif

	// 자식 대회상자 (좌/우상/우하)
	CLeftPane m_left;
	CRightTopPane m_top;
	CRightBottomPane m_bottom;

	// 스플리터 (수직/수평)
	CSplitterBar m_vsplit; // 좌/우
	CSplitterBar m_hsplit; // 우 상/하

	// 초기 비율
	double m_pctV = 0.25;
	double m_pctH = 0.60;

	int m_leftWidthPx = 300;
	int m_bottomHeightPx = 240;

	int m_minLeftPx = 220;
	int m_minRightTopPx = 220;
	int m_minBottomPx = 180;

	// Poll 모드 스위치 : true 면 모폴처럼 좌측 패널 없이 2분할
	bool m_pollMode = true;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnConnSingle();
	afx_msg void OnConnMulti();
	afx_msg void OnSetupOptions();

	afx_msg void OnWindowNew();
	afx_msg void OnWindowTile();

	// 메뉴 핸들러 선언
	afx_msg void OnWindowCascade();
	afx_msg void OnWindowTileH();
	afx_msg void OnWindowTileV();
	afx_msg void OnWindowClose();
	afx_msg void OnWindowCloseAll();
	afx_msg void OnWindowNext();


	// 배치함수
	void DoLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	

	DECLARE_MESSAGE_MAP()

	
};
