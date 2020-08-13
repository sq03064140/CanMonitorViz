//TODO 慢慢将CAN解析整理出来汇总在此 20200722
#include "../include/Obstacles.h"

Obstacles::Obstacles()
{
    memset(&this->CanAInfo, 0, sizeof(VCI_CAN_OBJ));
    memset(&this->CanBInfo, 0, sizeof(VCI_CAN_OBJ));
    memset(&this->Objt, 0, sizeof(Monitor::LINE));
    this->can_a_flg = false;
    this->can_b_flg = false;
    this->cpid = 0;
    this->LongitDist = 0.0f;
}

Obstacles::~Obstacles()
{
    //
}

bool Obstacles::SetCanAInfo(VCI_CAN_OBJ CanInfo)
{
    memcpy(&this->CanAInfo, &CanInfo, sizeof(VCI_CAN_OBJ));
    // Can报文按Bit/Byte组数据
    UINT Can_Identifer = (this->CanAInfo.Data[0]) >> 1;
    UINT Can_LongitDist = (this->CanAInfo.Data[1] << 4) + (this->CanAInfo.Data[2] >> 4);
    UINT Can_RelativeVelocity = ((this->CanAInfo.Data[2] & 0x0f) << 8) + (this->CanAInfo.Data[3]);
    UINT Can_LeftAng = ((this->CanAInfo.Data[4] & 0x3f) << 7) + (this->CanAInfo.Data[5] >> 1);
    UINT Can_RightAng = (this->CanAInfo.Data[6] << 5) + (this->CanAInfo.Data[7] >> 3);
    UINT Can_Class = this->CanAInfo.Data[7] & 0x07;
    this->DisplayObsCanAInfo(Can_Identifer, Can_LongitDist, Can_RelativeVelocity, Can_LeftAng, Can_RightAng, Can_Class);

    // 由Can来解析物理值
    this->cpid = Can_Identifer;
    this->LongitDist = Can_LongitDist * this->Unit_LongitDist;
    this->RelativeVelocity = this->GetRelativeVel(Can_RelativeVelocity);
    this->LeftAng = this->GetAng(Can_LeftAng);
    this->RightAng = this->GetAng(Can_RightAng);
    this->obj_class = Can_Class;

    if (this->cpid == 0)
    {
        this->can_a_flg = true;
        return false;
    }

    // 由物理值解析Obj所需值
    float Obj_angle = (this->LeftAng + this->RightAng) / 2.0f;
    float sin_ang = sin(Obj_angle); // * 180.0f / M_PI
    float cos_ang = cos(Obj_angle); // * 180.0f / M_PI
    this->Objt.x = this->LongitDist * cos_ang;
    this->Objt.y = this->LongitDist * (-1.0f * sin_ang);
    this->Objt.x_var = this->RelativeVelocity * sin_ang;
    this->Objt.y_var = this->RelativeVelocity * cos_ang;
    switch (this->obj_class)
    {
    case 0:
        this->Objt.type = Monitor::OBSTACLE_TYPE::car;
        break;
    case 1:
        this->Objt.type = Monitor::OBSTACLE_TYPE::trucks;
        break;
    case 2:
        this->Objt.type = Monitor::OBSTACLE_TYPE::car;
        break;
    case 3:
        this->Objt.type = Monitor::OBSTACLE_TYPE::bike;
        break;
    case 4:
        this->Objt.type = Monitor::OBSTACLE_TYPE::bike;
        break;
    case 5:
        this->Objt.type = Monitor::OBSTACLE_TYPE::man;
        break;

    default:
        // case 6- ?
        break;
    }
    this->can_a_flg = true;
}

bool Obstacles::SetCanBInfo(VCI_CAN_OBJ CanInfo)
{
    memcpy(&this->CanBInfo, &CanInfo, sizeof(VCI_CAN_OBJ));
    UINT Can_Lane = this->CanBInfo.Data[1] & 0x07;
    UINT Can_Width = this->CanBInfo.Data[3] >> 1;
    UINT Can_ExistProba = this->CanBInfo.Data[4];
    UINT Can_MotionStat = this->CanBInfo.Data[7] >> 4;
    // this->DisplayObsCanBInfo(Can_Lane, Can_Width, Can_ExistProba, Can_MotionStat);

    // 由Can来解析物理值
    this->obj_lane = Can_Lane;
    this->obj_width = Can_Width * this->Unit_Width;
    this->obj_exist_proba = Can_ExistProba * this->Unit_ExistProba;
    this->obj_motion_stat = Can_MotionStat;

    // 结构体赋值
    this->Objt.width = this->obj_width;
    switch (this->obj_lane)
    {
    case 0:
        this->Objt.lane = Monitor::OBSTACLE_LANE::unknow_lane;
        break;

    case 1:
        this->Objt.lane = Monitor::OBSTACLE_LANE::middle_lane;
        break;

    case 2:
        this->Objt.lane = Monitor::OBSTACLE_LANE::left_lane;
        break;

    case 3:
        this->Objt.lane = Monitor::OBSTACLE_LANE::right_lane;
        break;

    case 4:
        this->Objt.lane = Monitor::OBSTACLE_LANE::left_left_lane;
        break;

    case 5:
        this->Objt.lane = Monitor::OBSTACLE_LANE::right_right_lane;
        break;

    default:
        break;
    }

    this->can_b_flg = true;
    return can_b_flg;
}

int Obstacles::DisplayCanInfo(VCI_CAN_OBJ Can)
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

int Obstacles::DisplayObsInfo()
{
    printf("---------Obstracle Info-------------\n");
    printf("Obj_ID, LogituDist , RelativeVel :%d ,%3.2f, %2.2f \n",
           this->cpid, this->LongitDist, this->RelativeVelocity);
    printf("Class, LeftAng, RightAng :%d, %1.4f, %1.4f \n",
           this->obj_class, this->LeftAng, this->RightAng);
    printf("CanB Value [LaneNum, Width, ExistProba, MotionSta] : %d, %1.3f, %1.3f, %d \n", this->obj_lane, this->obj_width, this->obj_exist_proba, this->obj_motion_stat);
    printf("\n");
}

int Obstacles::ClearFlag()
{
    this->can_a_flg = false;
    this->can_b_flg = false;
    return 0;
}

float Obstacles::GetRelativeVel(UINT CRelativeVelocity)
{
    float fRelativVel = 0.0f;
    if (CRelativeVelocity < 0x7ff)
    {
        fRelativVel = CRelativeVelocity * this->Unit_RelativeVelocity;
    }
    else
    {
        fRelativVel = -1.0f * (0xfff - CRelativeVelocity + 1) * this->Unit_RelativeVelocity;
    }
    return fRelativVel;
}
float Obstacles::GetAng(UINT CLeftAng)
{
    float fLeftAng;
    if (CLeftAng < 0xfff)
    {
        fLeftAng = CLeftAng * this->Unit_Ang;
    }
    else
    {
        fLeftAng = -1.0f * (0x1fff - CLeftAng + 1) * this->Unit_Ang;
    }
    return fLeftAng;
}

int Obstacles::DisplayObsCanAInfo(UINT Identifer, UINT LongitDist, UINT RelatiVel, UINT LAng, UINT RAng, UINT Class)
{
    printf("Id/LongiDist/RelaVel : %02X, %03X, %03X \n", Identifer, LongitDist, RelatiVel);
    printf("Clas,LAng,RAng : %01X, %04X, %04X\n", Class, LAng, RAng);

    return 0;
}

int Obstacles::DisplayObsCanBInfo(UINT LL, UINT WW, UINT ExistProba, UINT MotionSta)
{
    printf("Lane/Width/ExistProb,MotionSta : %01X, %02X, %02X， %01X \n", LL, WW, ExistProba, MotionSta);
    return 0;
}

bool Obstacles::isReady()
{
    return (this->can_a_flg && this->can_b_flg);
}
