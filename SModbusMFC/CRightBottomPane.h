#pragma once
#include "afxdialogex.h"


// CRightBottomPane 대화 상자

class CRightBottomPane : public CDialogEx {
    DECLARE_DYNAMIC(CRightBottomPane)
public:
    CRightBottomPane(CWnd* pParent = nullptr) : CDialogEx(IDD_RIGHT_BOTTOM_PANE, pParent) {}
    
    void AddLog(const CString& time, const CString& dir, int len, const CString& preview);
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RIGHT_BOTTOM_PANE };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
private:
    CListCtrl m_listLog;
    void InitList();
    void Layout(int cx, int cy);
};
