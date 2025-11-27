#pragma once

#include "afxdialogex.h"
#include <afxwin.h>
#include <afxcmn.h>

#include <vector>
#include <memory>        // unique_ptr

#include "resource.h"    // 리소스 ID
#include "TopPaneMsgs.h"
#include "ModbusDocWnd.h"
#include "CommonTypes.h"

// CRightTopPane 대화 상자

class CRightTopPane : public CDialogEx {
    DECLARE_DYNAMIC(CRightTopPane)


public:
    CRightTopPane(CWnd* pParent = nullptr) : CDialogEx(IDD_RIGHT_TOP_PANE, pParent) {}
    ~CRightTopPane();

    void ApplyConnectionToActive(const ConnParams& p);
    void ApplyOptionsToActive(const PollOptions& o);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RIGHT_TOP_PANE };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
    virtual BOOL OnInitDialog();

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnTile(WPARAM, LPARAM);
    afx_msg LRESULT OnCascade(WPARAM, LPARAM);
    afx_msg LRESULT OnTileH(WPARAM, LPARAM);
    afx_msg LRESULT OnTileV(WPARAM, LPARAM);
    afx_msg LRESULT OnCloseActive(WPARAM, LPARAM);
    afx_msg LRESULT OnCloseAll(WPARAM, LPARAM);
    afx_msg LRESULT OnActivateNext(WPARAM, LPARAM);
    afx_msg LRESULT OnSetActive(WPARAM wParam, LPARAM);

    DECLARE_MESSAGE_MAP()

public:
    void CreateOneDoc(const CString& title = L"");

private:
    std::vector<std::unique_ptr<CModbusDocWnd>> m_docs;
    HWND m_hActive = nullptr;  // 활성 문서 HWND
    int  FindDocIndexByHwnd(HWND h) const;
};
