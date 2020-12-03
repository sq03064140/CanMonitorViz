#ifndef LANE_H_
#define LANE_H_

#include "controlcan.h"
#include "Plot_Data.h"
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <iostream>
#include "unistd.h"
#include <ctime>


// #define CAN_MONITO
#ifndef CAN_MONITOR
// CAN Monitor
#define LINE_R0_COEF        0x5FD // right lane coef A
#define LINE_R0_RANGE       0x5FF // right lane range
#define LINE_L0_COEF        0x5FA // 44B
#define LINE_L0_RANGE       0x5FC // 45B
#else
// Can Sensor调试接口数据
#define LINE_R0_COEF        0x42B // right lane coef A
#define LINE_R0_RANGE       0x43B // right lane range
#define LINE_L0_COEF        0x44B // 44B
#define LINE_L0_RANGE       0x45B // 45B
#endif

#define LINE_R1_COEF        0X5FE // right lane coef B(next)
#define LINE_R1_RANGE       0x5F4 // right lane range B
#define LINE_L1_COEF        0x5FB
#define LINE_L1_RANGE       0x5F3


class Lane
{
public:
    Lane();
    ~Lane();

    bool SetCoefInfo(VCI_CAN_OBJ CanInfo);
    bool SetViewInfo(VCI_CAN_OBJ CanInfo);
    int DisplayCanInfo(VCI_CAN_OBJ Can);
    int DisplayLineInfo();
    bool isReady();

    int ClearFlag();

    bool coef_flag;
    bool range_flag;

    VCI_CAN_OBJ CoefInfo;
    VCI_CAN_OBJ ViewInfo;

    Monitor::LINE LineInfo;

private:

    const float Unit_C0 = 0.01;
    const float Unit_C1 = 0.00098f;
    const float Unit_C2 = 1.0e-6;
    const float Unit_C3 = 4.0e-9;
    const float Unit_ViewRange = 0.5f;
};

#endif