#include "pch.h"
#include "CConnDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CConnDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CConnDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_CMB_CONNTYPE, &CConnDlg::OnSelChangeConnType)
END_MESSAGE_MAP()

// ---- 유틸: 그룹박스 하위 컨트롤 일괄 Enable/Disable
void CConnDlg::EnableGroup(UINT grpID, BOOL en)
{
    CWnd* pGrp = GetDlgItem(grpID);
    if (!pGrp) return;

    // 그룹 사각형 안의 자식들을 순회해서 토글
    CRect rc; pGrp->GetWindowRect(&rc); ScreenToClient(&rc);

    for (CWnd* p = GetWindow(GW_CHILD); p; p = p->GetWindow(GW_HWNDNEXT)) {
        CRect r; p->GetWindowRect(&r); ScreenToClient(&r);
        if (rc.PtInRect(r.TopLeft()) || rc.PtInRect(r.BottomRight()))
            p->EnableWindow(en);
    }
    // 그룹박스 자체 텍스트는 항상 보이게
    pGrp->EnableWindow(TRUE);
}

// ---- 유틸: COM 포트 열거 (간단·빠른 방식)
void CConnDlg::EnumComPorts(CComboBox& cb)
{
    cb.ResetContent();
    // COM1~COM256 시도
    WCHAR name[16];
    for (int i = 1;i <= 256;++i) {
        wsprintfW(name, L"COM%d", i);
        // 존재 여부 확인: CreateFile 테스트는 느리므로 QueryDosDevice 사용
        WCHAR target[256];
        DWORD res = QueryDosDeviceW(name, target, _countof(target));
        if (res != 0) cb.AddString(name);
    }
    if (cb.GetCount() == 0) cb.AddString(L"COM1");
}

// ---- 유틸: 시리얼 콤보 채우기
void CConnDlg::FillSerialCombos()
{
    CComboBox* cPort = (CComboBox*)GetDlgItem(IDC_CMB_PORT);
    CComboBox* cBaud = (CComboBox*)GetDlgItem(IDC_CMB_BAUD);
    CComboBox* cData = (CComboBox*)GetDlgItem(IDC_CMB_DATABITS);
    CComboBox* cPar = (CComboBox*)GetDlgItem(IDC_CMB_PARITY);
    CComboBox* cStop = (CComboBox*)GetDlgItem(IDC_CMB_STOPBITS);

    EnumComPorts(*cPort);

    const int bauds[] = { 9600, 19200, 38400, 57600, 115200 };
    for (int b : bauds) {
        CString s; s.Format(L"%d", b); cBaud->AddString(s);
    }
    cData->AddString(L"7");
    cData->AddString(L"8");

    cPar->AddString(L"N");
    cPar->AddString(L"E");
    cPar->AddString(L"O");

    cStop->AddString(L"1");
    cStop->AddString(L"2");
}

BOOL CConnDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 연결 타입 콤보 채우기
    CComboBox* cType = (CComboBox*)GetDlgItem(IDC_CMB_CONNTYPE);
    cType->AddString(L"Serial Port");
    cType->AddString(L"TCP");
    cType->AddString(L"RTU over TCP");

    // 시리얼 콤보들 채우기
    FillSerialCombos();

    // params -> 화면 바인딩
    switch (params.type) {
    case ConnType::Serial:       m_connTypeIdx = 0; break;
    case ConnType::TCP:          m_connTypeIdx = 1; break;
    case ConnType::RTU_OVER_TCP: m_connTypeIdx = 2; break;
    }

    m_port = params.ser.port;
    m_baud = params.ser.baud;
    m_databits = params.ser.databits;
    m_stopbits = params.ser.stopbits;
    m_parity = (params.ser.parity == L'N' ? 0 : params.ser.parity == L'E' ? 1 : 2);
    m_modeAscii = params.ser.ascii ? 1 : 0;

    m_host = params.tcp.host;
    m_tcpPort = params.tcp.port;

    m_timeout = params.respTimeoutMs;
    m_gap = params.gapMs;

    UpdateData(FALSE);

    // 콤보 기본 선택 보정
    ((CComboBox*)GetDlgItem(IDC_CMB_CONNTYPE))->SetCurSel(m_connTypeIdx);
    // Baud/Port/등은 문자열 매칭 또는 인덱스로 SetCurSel
    // 포트
    if (!m_port.IsEmpty()) {
        int idx = ((CComboBox*)GetDlgItem(IDC_CMB_PORT))->FindStringExact(-1, m_port);
        if (idx >= 0) ((CComboBox*)GetDlgItem(IDC_CMB_PORT))->SetCurSel(idx);
    }
    // Baud
    {
        CComboBox* c = (CComboBox*)GetDlgItem(IDC_CMB_BAUD);
        CString s; s.Format(L"%d", m_baud);
        int idx = c->FindStringExact(-1, s);
        if (idx >= 0) c->SetCurSel(idx);
    }
    // DataBits
    ((CComboBox*)GetDlgItem(IDC_CMB_DATABITS))->SetCurSel(m_databits == 7 ? 0 : 1);
    // Parity
    ((CComboBox*)GetDlgItem(IDC_CMB_PARITY))->SetCurSel(m_parity);
    // StopBits
    ((CComboBox*)GetDlgItem(IDC_CMB_STOPBITS))->SetCurSel(m_stopbits == 2 ? 1 : 0);
    // RTU/ASCII
    CheckRadioButton(IDC_RAD_RTU, IDC_RAD_ASCII, m_modeAscii ? IDC_RAD_ASCII : IDC_RAD_RTU);

    UpdateEnable();
    return TRUE;
}

void CConnDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // 실제 IDC 이름에 맞춰 매핑하세요
    DDX_CBIndex(pDX, IDC_CMB_CONNTYPE, m_connTypeIdx);

    DDX_Text(pDX, IDC_ED_TIMEOUT, m_timeout);
    DDX_Text(pDX, IDC_ED_GAP, m_gap);

    // Serial
    DDX_CBString(pDX, IDC_CMB_PORT, m_port);

    // TCP
    DDX_Text(pDX, IDC_ED_HOST, m_host);
    DDX_Text(pDX, IDC_ED_PORT, m_tcpPort);
}

void CConnDlg::OnSelChangeConnType()
{
    CComboBox* cType = (CComboBox*)GetDlgItem(IDC_CMB_CONNTYPE);
    m_connTypeIdx = cType->GetCurSel();
    UpdateEnable();
}

void CConnDlg::UpdateEnable()
{
    BOOL bSerial = (m_connTypeIdx == 0);
    BOOL bTcp = (m_connTypeIdx != 0); // TCP/RTU TCP 둘 다 TCP 섹션

    EnableGroup(IDC_SERIAL, bSerial);
    EnableGroup(IDC_TCP, bTcp);
}

void CConnDlg::OnOK()
{
    UpdateData(TRUE);

    // 연결 타입
    params.type = (m_connTypeIdx == 0 ? ConnType::Serial
        : m_connTypeIdx == 1 ? ConnType::TCP : ConnType::RTU_OVER_TCP);

    // Serial
    params.ser.port = m_port;
    params.ser.baud = m_baud;
    params.ser.databits = (((CComboBox*)GetDlgItem(IDC_CMB_DATABITS))->GetCurSel() == 0 ? 7 : 8);
    params.ser.parity = (m_parity == 0 ? L'N' : m_parity == 1 ? L'E' : L'O');
    params.ser.stopbits = (((CComboBox*)GetDlgItem(IDC_CMB_STOPBITS))->GetCurSel() == 1 ? 2 : 1);
    params.ser.ascii = (IsDlgButtonChecked(IDC_RAD_ASCII) == BST_CHECKED);

    // Baud
    CString sBaud;
    auto* cBaud = (CComboBox*)GetDlgItem(IDC_CMB_BAUD);
    if (cBaud->GetCount() > 0) {
        int idx = cBaud->GetCurSel();
        if (idx >= 0) { cBaud->GetLBText(idx, sBaud); params.ser.baud = _wtoi(sBaud); }
    }

    // Data bits
    int idxData = ((CComboBox*)GetDlgItem(IDC_CMB_DATABITS))->GetCurSel();
    params.ser.databits = (idxData == 0 ? 7 : 8);

    // Parity
    int idxPar = ((CComboBox*)GetDlgItem(IDC_CMB_PARITY))->GetCurSel();
    params.ser.parity = (idxPar == 0 ? L'N' : idxPar == 1 ? L'E' : L'O');

    // Stop bits
    int idxStop = ((CComboBox*)GetDlgItem(IDC_CMB_STOPBITS))->GetCurSel();
    params.ser.stopbits = (idxStop == 1 ? 2 : 1);

    // RTU/ASCII
    params.ser.ascii = (IsDlgButtonChecked(IDC_RAD_ASCII) == BST_CHECKED);

    // TCP
    params.tcp.host = m_host;
    params.tcp.port = m_tcpPort;

    // 공통
    params.respTimeoutMs = m_timeout;
    params.gapMs = m_gap;

    CDialogEx::OnOK();
}
