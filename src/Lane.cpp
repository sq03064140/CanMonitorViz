//TODO 慢慢将CAN解析整理出来汇总在此 20200722
#include "../include/Lane.h"

Lane::Lane()
{
    memset(&this->ViewInfo, 0, sizeof(VCI_CAN_OBJ));
    memset(&this->CoefInfo, 0, sizeof(VCI_CAN_OBJ));
    memset(&this->LineInfo, 0, sizeof(Monitor::LINE));
    this->coef_flag = false;
    this->range_flag = false;
}

Lane::~Lane()
{
    //
}

bool Lane::SetCoefInfo(VCI_CAN_OBJ CanInfo)
{
    memcpy(&this->CoefInfo, &CanInfo, sizeof(VCI_CAN_OBJ));

    // Analyze CAN: line info
    // C3
    UINT CanVal_C3 = (this->CoefInfo.Data[0] << 8) + (this->CoefInfo.Data[1]); // 拼接Coef C3
    this->LineInfo.c3 = 0.0f;
    if (CanVal_C3 <= 0x7fff)
    {
        this->LineInfo.c3 = CanVal_C3 * this->Unit_C3;
    }
    else
    {
        this->LineInfo.c3 = -1.0f * (0xffff - CanVal_C3 + 1) * this->Unit_C3;
    }
    this->LineInfo.c3 = -1.0f * this->LineInfo.c3;

    // C2
    UINT CanVal_C2 = (this->CoefInfo.Data[2] << 8) + (this->CoefInfo.Data[3]);
    this->LineInfo.c2 = 0.0f;
    if (CanVal_C2 < 0x7fff)
    {
        this->LineInfo.c2 = CanVal_C2 * this->Unit_C2;
    }
    else
    {
        this->LineInfo.c2 = -1.0f * (0xffff - CanVal_C2 + 1) * this->Unit_C2;
    }
    this->LineInfo.c2 = -1.0f * this->LineInfo.c2;

    // C0
    UINT CanVal_C0 = (this->CoefInfo.Data[4] << 4) + (this->CoefInfo.Data[5] >> 4);
    // printf("CanVal C0 : %d\n",CanVal_C0);
    if (CanVal_C0 < 0x7ff)
    {
        this->LineInfo.c0 = CanVal_C0 * this->Unit_C0;
    }
    else
    {
        this->LineInfo.c0 = -1.0f * (0xfff - CanVal_C0 + 1) * this->Unit_C0;
    }
    this->LineInfo.c0 = -1.0f * this->LineInfo.c0;

    // C1
    UINT CanVal_C1 = ((this->CoefInfo.Data[5] & 0xf) << 6) + (this->CoefInfo.Data[6] >> 2);
    if (CanVal_C1 < 0x1ff)
    {
        this->LineInfo.c1 = CanVal_C1 * this->Unit_C1;
    }
    else
    {
        this->LineInfo.c1 = -1.0f * (0x3ff - CanVal_C1 + 1) * this->Unit_C1;
    }
    this->LineInfo.c1 = -1.0f * this->LineInfo.c1;

    //Line_QI
    BYTE LineQi = this->CoefInfo.Data[7] >> 6;
    this->LineInfo.QI = Monitor::LINE_QI(LineQi);
    //Line_Type
    BYTE LineType = (this->CoefInfo.Data[7] >> 3) & 0x7;
    // this->LineInfo.type = Monitor::LINE_TYPE(LineType);
    /*LineType DBC:0 undecided/1 solid/2 road edge/3 dashed/
                   4 double lane mark/ 5 Bott's dotts
                   6 Barrier / 7 forbidden value
    */

    switch (LineType)
    {
    case 0:
        this->LineInfo.type = Monitor::LINE_TYPE::full;
        break;
    case 1:
        this->LineInfo.type = Monitor::LINE_TYPE::full;
        break;
    case 2:
        this->LineInfo.type = Monitor::LINE_TYPE::edge;
        break;
    case 3:
        this->LineInfo.type = Monitor::LINE_TYPE::dash;
        break;
    case 4:
        this->LineInfo.type = Monitor::LINE_TYPE::dual;
        break;
    case 5:
        this->LineInfo.type = Monitor::LINE_TYPE::full;
        break;
    case 6:
        this->LineInfo.type = Monitor::LINE_TYPE::full;
        break;
    case 7:
        this->LineInfo.type = Monitor::LINE_TYPE::full;
        break;
    default:
        break;
    }

    this->coef_flag = true;
}

bool Lane::SetViewInfo(VCI_CAN_OBJ CanInfo)
{
    memcpy(&this->ViewInfo, &CanInfo, sizeof(VCI_CAN_OBJ));
    BYTE view_range;
    this->LineInfo.view_range = (this->ViewInfo.Data[0]) * Unit_ViewRange;

    this->range_flag = true;
}

int Lane::DisplayCanInfo(VCI_CAN_OBJ Can)
{
    printf("------------------------------\n");
    printf("CAN RX ID:0x%08X", Can.ID); //ID
    // Test&Viz CanFrame 代码
    if (Can.ExternFlag == 0)
        printf(" Standard "); //帧格式：标准帧
    if (Can.ExternFlag == 1)
        printf(" Extend   "); //帧格式：扩展帧
    if (Can.RemoteFlag == 0)
        printf(" Data   "); //帧类型：数据帧
    if (Can.RemoteFlag == 1)
        printf(" Remote ");            //帧类型：远程帧
    printf("DLC:0x%02X", Can.DataLen); //帧长度
    printf(" data:0x");                //数据
    for (int i = 0; i < Can.DataLen; i++)
    {
        printf(" %02X", Can.Data[i]);
    }
    printf(" TimeStamp:0x%08X", Can.TimeStamp); //时间标识。
    printf("\n");
    printf("------------------------------\n");
}

int Lane::DisplayLineInfo()
{
    // printf("---------Lane Info-------------\n");
    printf("C0,C1,C2,C3 :%2.4f ,%1.3f, %0.4f, %0.5f \n", LineInfo.c0, LineInfo.c1, LineInfo.c2, LineInfo.c3);
}

int Lane::ClearFlag()
{
    this->coef_flag = false;
    this->range_flag = false;
}

bool Lane::isReady()
{
    return (this->coef_flag && this->range_flag);
}
