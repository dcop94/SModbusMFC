#pragma once
#include <afxwin.h>
#include <afxcmn.h>

// Modbus Poll 스타일의 "자식 문서 창"
class CModbusDocWnd : public CWnd
{
public:
    CModbusDocWnd() {}
    virtual ~CModbusDocWnd() {}

    // 부모 Pane 안에 생성
    BOOL Create(CWnd* pParent, const CRect& rcInit, const CString& title = L"Mbpoll1");

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnWindowPosChanging(WINDOWPOS* wp);

private:
    CListCtrl m_grid; // 내부 데이터 그리드(Report)
    void InitGrid();
};
