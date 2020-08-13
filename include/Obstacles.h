#ifndef OBSTACLE_H_
#define OBSTACLE_H_
// 未完成，障碍物信息

#include "controlcan.h"
#include "Plot_Data.h"
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <iostream>
#include "unistd.h"
#include <cmath>

#define OBJ_0_A 0x700 // object A
#define OBJ_0_B 0x701

#define OBJ_1_A 0x704
#define OBJ_1_B 0x705

#define OBJ_2_A 0X708
#define OBJ_2_B 0X709

#define OBJ_3_A 0x70C
#define OBJ_3_B 0x70D

#define OBJ_4_A 0X710
#define OBJ_4_B 0x711

#define OBJ_5_A 0X714
#define OBJ_5_B 0x715

#define OBJ_6_A 0X718
#define OBJ_6_B 0x719

#define OBJ_7_A 0X71C
#define OBJ_7_B 0x71D

#define OBJ_8_A 0X720
#define OBJ_8_B 0x721

#define OBJ_9_A 0X724
#define OBJ_9_B 0x725



class Obstacles
{
public:
    Obstacles();
    ~Obstacles();

    bool SetCanAInfo(VCI_CAN_OBJ CanInfo);
    bool SetCanBInfo(VCI_CAN_OBJ CanInfo);
    int DisplayCanInfo(VCI_CAN_OBJ Can);
    int DisplayObsInfo();
    int DisplayObsCanAInfo(UINT Identifer, UINT LongitDist, UINT RelatiVel, UINT LAng, UINT RAng, UINT Class);
    int DisplayObsCanBInfo(UINT LL, UINT WW, UINT ExistProba, UINT MotionSta);
    bool isReady();
    int ClearFlag();

    bool can_a_flg;
    bool can_b_flg;

    VCI_CAN_OBJ CanAInfo;
    VCI_CAN_OBJ CanBInfo;

    Monitor::OBSTACLE Objt;
    int cpid; // 障碍物ID
    float LongitDist;
    float RelativeVelocity;
    float LeftAng, RightAng;
    /* obj_lane
    0b000: Not assigned         0b001: Ego lane 
    0b010: Next lane left       0b011: Next lane right 
    0b100: Next next lane left  0b101: Next next lane right
    */
    uint obj_lane;
    float obj_width;
    uint obj_motion_stat;
    float obj_exist_proba;
    int obj_class;

private:
    const float Unit_LongitDist = 0.05f;
    const float Unit_RelativeVelocity = 0.05f;
    const float Unit_Ang = 0.0002;
    const float Unit_Width = 0.05f;
    const float Unit_ExistProba = 0.00392157f;

    float GetRelativeVel(UINT CRelativeVelocity);
    float GetAng(UINT CLeftAng);
};

#endif
