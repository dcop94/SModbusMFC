#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include <thread>
#include <atomic>
#include "TopPaneMsgs.h"
#include "CommonTypes.h"

#define WM_DOC_UPDATE_ROW (WM_APP + 301) // UI 갱신용

// Modbus Poll 스타일의 "자식 문서 창"
class CModbusDocWnd : public CWnd
{
public:
    CModbusDocWnd() {}
    virtual ~CModbusDocWnd() {}

    // 부모 Pane 안에 생성
    BOOL Create(CWnd* pParent, const CRect& rcInit, const CString& title = L"Mbpoll1");

    void ApplyOptions(const PollOptions& o);

    // 폴링 제어
    void StartPoll();
    void StopPoll();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnWindowPosChanging(WINDOWPOS* wp);

    afx_msg void OnLButtonDown(UINT, CPoint);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint pt);
    afx_msg void OnClose();  // X 버튼

    afx_msg LRESULT OnUpdateRow(WPARAM w, LPARAM l); // UI 갱신
    afx_msg void OnDestroy();

    afx_msg void OnTimer(UINT_PTR id);

private:
    CListCtrl m_grid; // 내부 데이터 그리드(Report)

    int m_slave = 1;
    int m_func = 3;
    int m_addr = 0;
    int m_qty = 10;
    int m_scanMs = 1000;

    //폴링 스레드
    std::thread m_thr;
    std::atomic_bool m_run{ false };

    //내부 유틸
    void PollLoop(); // 스레드 본체
    void UpdateGridRow(int row, int value);

    void InitGrid();
    void AppendLogTX(const CString& hex);
    void AppendLogRX(const CString& hex);
};
