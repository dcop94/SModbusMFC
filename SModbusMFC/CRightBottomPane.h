#pragma once
#include "afxdialogex.h"


// CRightBottomPane 대화 상자

class CRightBottomPane : public CDialogEx
{
	DECLARE_DYNAMIC(CRightBottomPane)

public:
	CRightBottomPane(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRightBottomPane();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHT_BOTTOM_PANE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
