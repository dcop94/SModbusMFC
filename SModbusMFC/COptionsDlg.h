#pragma once
#include "afxdialogex.h"
#include "CommonTypes.h"
#include "resource.h"

// COptionsDlg 대화 상자

class COptionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
    COptionsDlg(CWnd* pParent = nullptr) : CDialogEx(IDD_OPT_DLG, pParent) {}
    PollOptions opts;   // 입출력 컨테이너

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_OPT_DLG };
#endif

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
    afx_msg void OnCbnSelchangeFunc();
    virtual void OnOK();
    DECLARE_MESSAGE_MAP()

private:
    void LoadToUI();
    bool SaveFromUI();  // false면 검증 실패
};
