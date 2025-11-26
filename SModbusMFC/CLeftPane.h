#pragma once
#include "afxdialogex.h"


// CLeftPane 대화 상자

class CLeftPane : public CDialogEx
{
	DECLARE_DYNAMIC(CLeftPane)

public:
	CLeftPane(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLeftPane();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT_PANE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
