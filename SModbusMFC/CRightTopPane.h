#pragma once
#include "afxdialogex.h"
#include "ModbusDocWnd.h"
#include <afxwin.h>
#include <afxcmn.h>
#include <vector>
#include <memory>



// CRightTopPane 대화 상자

class CRightTopPane : public CDialogEx {
    DECLARE_DYNAMIC(CRightTopPane)

#define WM_TOPPANE_TILE (WM_APP + 101)
    afx_msg LRESULT OnTile(WPARAM, LPARAM);
public:
    CRightTopPane(CWnd* pParent = nullptr) : CDialogEx(IDD_RIGHT_TOP_PANE, pParent) {}
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RIGHT_TOP_PANE };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

public:
    void CreateOneDoc(const CString& title = L"");

private:
    CListCtrl m_listData;
    void InitList();
    void Layout(int cx, int cy);
    std::vector<std::unique_ptr<CModbusDocWnd>> m_docs;
};
