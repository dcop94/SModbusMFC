#pragma once
#include "afxdialogex.h"
#include "CommonTypes.h"
#include "resource.h"

class CConnDlg : public CDialogEx {
    DECLARE_DYNAMIC(CConnDlg)
public:
    CConnDlg(CWnd* pParent = nullptr) : CDialogEx(IDD_CONN_DLG, pParent) {}
    ConnParams params;   // 입·출력 컨테이너

protected:
    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;
    afx_msg void OnSelChangeConnType();
    virtual void OnOK() override;
    DECLARE_MESSAGE_MAP()

private:
    // DDX 변수
    int     m_connTypeIdx = 0;     // 0:Serial, 1:TCP, 2:RTU over TCP
    CString m_port;  int m_baud = 38400; int m_databits = 8; int m_stopbits = 1; int m_parity = 0; // 0:N 1:E 2:O
    int     m_modeAscii = 0;         // 0 RTU, 1 ASCII
    CString m_host = L"127.0.0.1";   int m_tcpPort = 502;
    int     m_timeout = 1000, m_gap = 20;

    // 유틸
    void UpdateEnable();               // 그룹별 Enable/Disable
    void EnableGroup(UINT grpID, BOOL en);
    void FillSerialCombos();           // Baud/Parity/Data/Stop 채우기
    void EnumComPorts(CComboBox& cb);  // COM 포트 나열
};
