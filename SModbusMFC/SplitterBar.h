#pragma once
#include <afxwin.h>     // MFC core
#include <afxdialogex.h>

class CSplitterBar : public CWnd
{
public:
    enum ORIENT { VERT, HORZ };  // VERT: 좌/우 분할, HORZ: 상/하 분할

    // 스플리터 생성 (부모 안에 Child로)
    BOOL Create(ORIENT o, CWnd* pParent, UINT nID);

    // 분할 비율(0.1 ~ 0.9 사이로 클램프)
    void   SetPercent(double pct) { m_percent = max(0.1, min(0.9, pct)); }
    double GetPercent() const { return m_percent; }

    // 기본 퍼센트 (더블클릭 시 리셋용)
    void SetDefaultPercent(double pct)
    {
        m_defaultPercent = max(0.1, min(0.9, pct));
    }

    double GetDefaultPercent() const
    {
        return m_defaultPercent;
    }

protected:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
    afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
    afx_msg void OnLButtonDbClk(UINT, CPoint);
    DECLARE_MESSAGE_MAP()

private:
    ORIENT m_orient = VERT;
    bool   m_drag = false;
    CPoint m_dragStart;     // 드래그 시작 지점(스크린 좌표)
    CRect  m_parentRect;    // 부모 클라이언트 영역(드래그 중 계산용)
    double m_percent = 0.25; // 수직: 좌측폭 비율, 수평: 상단높이 비율
    double m_defaultPercent = 0.25;
};
