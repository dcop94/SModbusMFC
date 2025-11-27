// COptionsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "SModbusMFC.h"
#include "afxdialogex.h"
#include "COptionsDlg.h"
#include "CommonTypes.h"


IMPLEMENT_DYNAMIC(COptionsDlg, CDialogEx)
BEGIN_MESSAGE_MAP(COptionsDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_CMB_FUNC, &COptionsDlg::OnCbnSelchangeFunc)
END_MESSAGE_MAP()

BOOL COptionsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Function 콤보 채우기
    auto* cmb = (CComboBox*)GetDlgItem(IDC_CMB_FUNC);
    cmb->AddString(L"01 Read Coils (1)");
    cmb->AddString(L"02 Read Discrete (2)");
    cmb->AddString(L"03 Read Holding (3)");
    cmb->AddString(L"04 Read Input (4)");

    LoadToUI();
    return TRUE;
}

void COptionsDlg::LoadToUI()
{
    SetDlgItemInt(IDC_ED_SLAVE, opts.slave);
    SetDlgItemInt(IDC_ED_ADDR, opts.addr);
    SetDlgItemInt(IDC_ED_QTY, opts.qty);
    SetDlgItemInt(IDC_ED_SCAN, opts.scanMs);

    int sel = 2; // default 03
    switch (opts.func) {
    case 1: sel = 0; break; case 2: sel = 1; break;
    case 3: sel = 2; break; case 4: sel = 3; break;
    }
    ((CComboBox*)GetDlgItem(IDC_CMB_FUNC))->SetCurSel(sel);
}

void COptionsDlg::OnCbnSelchangeFunc()
{
    // 필요시 기능별 힌트/유효범위 안내 등
}

bool COptionsDlg::SaveFromUI()
{
    BOOL ok = FALSE;
    int slave = GetDlgItemInt(IDC_ED_SLAVE, &ok);  if (!ok || slave < 1 || slave > 247) { AfxMessageBox(L"Slave ID: 1~247"); return false; }
    int addr = GetDlgItemInt(IDC_ED_ADDR, &ok);  if (!ok || addr < 0) { AfxMessageBox(L"Address: 0 이상"); return false; }
    int qty = GetDlgItemInt(IDC_ED_QTY, &ok);  if (!ok || qty <= 0 || qty > 2000) { AfxMessageBox(L"Quantity: 1~2000"); return false; }
    int scan = GetDlgItemInt(IDC_ED_SCAN, &ok);  if (!ok || scan < 50 || scan > 60000) { AfxMessageBox(L"Scan: 50~60000ms"); return false; }

    int sel = ((CComboBox*)GetDlgItem(IDC_CMB_FUNC))->GetCurSel();
    int func = (sel == 0 ? 1 : sel == 1 ? 2 : sel == 2 ? 3 : 4);

    opts.slave = slave;
    opts.func = func;
    opts.addr = addr;
    opts.qty = qty;
    opts.scanMs = scan;
    return true;
}

void COptionsDlg::OnOK()
{
    if (!SaveFromUI()) return;
    CDialogEx::OnOK();
}