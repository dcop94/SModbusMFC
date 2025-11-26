#pragma once
#include "afxdialogex.h"


// CRightTopPane 대화 상자

class CRightTopPane : public CDialogEx
{
	DECLARE_DYNAMIC(CRightTopPane)

public:
	CRightTopPane(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRightTopPane();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHT_TOP_PANE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
