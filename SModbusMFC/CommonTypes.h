#pragma once
#include <afxstr.h>

enum class ConnType { Serial, TCP, RTU_OVER_TCP };

struct SerialParams {
    CString port;  // e.g. "COM10"
    int     baud = 38400;
    int     databits = 8; // 7/8
    wchar_t parity = L'N'; // 'N','E','O'
    int     stopbits = 1;   // 1/2
    bool    ascii = false;  // (UI 제공; libmodbus ASCII는 미지원)
};

struct TcpParams { CString host = L"127.0.0.1"; int port = 502; };

struct ConnParams {
    ConnType    type = ConnType::Serial;
    SerialParams ser;
    TcpParams    tcp;
    int respTimeoutMs = 1000;
    int gapMs = 20;    // poll 사이 간격
};

enum class RWFunc { ReadCoils = 1, ReadDI = 2, ReadHR = 3, ReadIR = 4 };

struct RWDef {
    int    slave = 1;
    RWFunc func = RWFunc::ReadHR;
    int    addr = 0;
    int    qty = 10;
    int    scanMs = 1000;
    bool   disableOnError = false;
    bool   plcBase1 = false;   // (Base 1)
    bool   addrInCol = false;  // 표시에만 영향
};



struct PollOptions {
    int slave = 1;
    int func = 3;     // 1,2,3,4 중 하나
    int addr = 0;
    int qty = 10;
    int scanMs = 1000;
};
