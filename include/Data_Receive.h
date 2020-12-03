#ifndef DATA_RECEIVE_H
#define DATA_RECEIVE_H

#include <mutex>
#include <iostream>
#include "unistd.h"
#include "Plot_Data.h"
#include <ctime>
// Can Rec&Analyzer class
#include "controlcan.h"
#include "Lane.h"
#include "Obstacles.h"
#include "../config/config.h"

#define RUN_PERIOD 0.0001
#define COUNT_MAX 80
using namespace std;

namespace Monitor
{
    mutex data_mutex;
    mutex pause_mutex;
    bool pause_status;
} // namespace Monitor

using namespace Monitor;

// #ifdef OFFLINE_MODE

void *Signal_Recieve_Thread_Fun(void *args)
{
    bool flag = false;
    ;
    while (1)
    {
        time_t now = time(0);
        cout << "Signal_Recieve_Thread_Fun is running!!!"
             << "   Curent time stamp: " << now << " !!!" << endl;

        data_mutex.lock();
        PLOT_DATA *plot_data = (PLOT_DATA *)args;

        //*****************Recieve the line data*******************//
        plot_data->lines.clear(); // 移除容器中所有数据 --用push_back必须用这个

        float cur = flag ? 0.002 : -0.002;
        plot_data->lines.resize(4);                                     // 设置容量为4
        plot_data->lines[0] = LINE(-6.0, 0, cur, 0, 0, 80, full, good); //调用构造函数，一行解决赋值问题 ，构造函数的格式在类里有-----KDK
        plot_data->lines[1] = LINE(-2.0, 0, cur, 0, 0, 80, dash, good); //调用构造函数，一行解决赋值问题 ，构造函数的格式在类里有-----KDK
        plot_data->lines[2] = LINE(2.0, 0, cur, 0, 0, 80, full, good);  //调用构造函数，一行解决赋值问题 ，构造函数的格式在类里有-----KDK
        plot_data->lines[3] = LINE(6.0, 0, cur, 0, 0, 80, dash, good);  //调用构造函数，一行解决赋值问题 ，构造函数的格式在类里有-----KDK

        plot_data->lines.push_back(LINE(0, 0, cur, 0, 0, 80, plan, good)); //调用构造函数，一行解决赋值问题 ，构造函数的格式在类里有-----KDK  //容量+1
        plot_data->lines.back().QI = good;

        plot_data->lines.push_back(LINE(4, 0, cur, 0, 0, 80, forecast, good)); //调用构造函数，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1

        //*****************Recieve the obstacle data*******************//
        plot_data->obstacles.clear();    //将原数据清空 --用push_back必须用这个
        plot_data->obstacles.resize(1);  // 设置容量为1
        plot_data->obstacles[0].x = 0.0; //老版接口也支持
        plot_data->obstacles[0].y = 0.0;
        plot_data->obstacles[0].head = 0.0;
        plot_data->obstacles[0].wheelbase = 3.0;
        plot_data->obstacles[0].width = 1.8;
        plot_data->obstacles[0].type = ego;

        plot_data->obstacles.push_back(OBSTACLE(0.0, 4.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = left_lane;
        plot_data->obstacles.push_back(OBSTACLE(0.0, -4.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = right_lane;
        plot_data->obstacles.push_back(OBSTACLE(4.0, 0.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = middle_lane;
        plot_data->obstacles.push_back(OBSTACLE(0.0, 8.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = left_left_lane;
        plot_data->obstacles.push_back(OBSTACLE(12.0, 0.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = middle_lane;
        plot_data->obstacles.back().source = front_radar;
        plot_data->obstacles.push_back(OBSTACLE(22.0, 0.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = middle_lane;
        plot_data->obstacles.back().source = corner_radar;
        plot_data->obstacles.push_back(OBSTACLE(32.0, 0.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = middle_lane;
        plot_data->obstacles.back().source = fusion;
        plot_data->obstacles.push_back(OBSTACLE(10.0, 8.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = left_left_lane;
        plot_data->obstacles.back().source = fusion;
        plot_data->obstacles.push_back(OBSTACLE(10.0, 4.0, 0.0, car, true, 3.0, 1.8)); //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        plot_data->obstacles.back().lane = left_lane;
        plot_data->obstacles.back().source = fusion;
        //plot_data->obstacles.push_back(OBSTACLE(0.0,8.0,0.0,car,true,3.0,1.8));      //建议用这个语句，一行解决赋值问题 ，构造函数的格式在类里有-----KDK //容量+1
        //plot_data->obstacles.push_back(OBSTACLE(4.0,-8.0,0.0,car,true,3.0,1.8));
        //plot_data->obstacles[1].x=0.0;
        //        plot_data->obstacles[1].y=4.0;
        //        plot_data->obstacles[1].head=0.0;
        //        plot_data->obstacles[1].wheelbase=3.0;
        //        plot_data->obstacles[1].width=1.8;
        //        plot_data->obstacles[1].type=car;

        data_mutex.unlock();
        flag = !flag;
        sleep(RUN_PERIOD);
    }
}
#endif // OFFLINE_MODEL

#ifndef OFFLINE_MODE
void *Signal_Recieve_Thread_Fun(void *args)
{
    bool is_init = false;
    PLOT_DATA *plot_data = (PLOT_DATA *)args;
    // CAN卡初始化
    if (VCI_OpenDevice(VCI_USBCAN2, 0, 0) == 1) //打开设备
    {
        printf(">>open deivce success!\n"); //打开设备成功
    }
    else
    {
        printf(">>open deivce error!\n");
        exit(1);
    }
    //初始化参数，严格参数二次开发函数库说明书。
    VCI_INIT_CONFIG config;
    /*
    AccCode/AccMask用来过滤CanID
    AccCode 启始ID:0x42b << 21 = 0x8560000;
    AccMask 0x30 << 21        = 0x6000000;
    */
    // config.AccCode = 0;
    // config.AccMask = 0xFFFFFFFF;
    config.AccCode = 0x85600000;
    config.AccMask = 0xFFFFFFFF;
    config.Filter = 1;     //接收所有帧
    config.Timing0 = 0x00; /*波特率500 Kbps  0x00  0x1C*/
    config.Timing1 = 0x1C;
    config.Mode = 0; //正常模式
    bool flag = false;
    if (VCI_InitCAN(VCI_USBCAN2, 0, 0, &config) != 1)
    {
        printf(">>Init CAN1 error\n");
        VCI_CloseDevice(VCI_USBCAN2, 0);
    }

    if (VCI_StartCAN(VCI_USBCAN2, 0, 0) != 1)
    {
        printf(">>Start CAN1 error\n");
        VCI_CloseDevice(VCI_USBCAN2, 0);
    }

    if (VCI_InitCAN(VCI_USBCAN2, 0, 1, &config) != 1)
    {
        printf(">>Init can2 error\n");
        VCI_CloseDevice(VCI_USBCAN2, 0);
    }
    if (VCI_StartCAN(VCI_USBCAN2, 0, 1) != 1)
    {
        printf(">>Start can2 error\n");
        VCI_CloseDevice(VCI_USBCAN2, 0);
    }

    // Can信息，接收设置
    uint count = 0;
    int reclen = 0;
    VCI_CAN_OBJ rec[3000]; //接收缓存，设为3000为佳。
    int i, j;
    int ind = 0;
    Lane line_r0, line_l0;
    Lane line_r1, line_l1;
    Obstacles Objt_0, Objt_1, Objt_2, Objt_3, Objt_4, Objt_5, Objt_6, Objt_7, Objt_8, Objt_9;
    while (1)
    {
        if ((reclen = VCI_Receive(VCI_USBCAN2, 0, ind, rec, 3000, 100)) > 0) //调用接收函数，如果有数据，进行数据处理显示。
        {
            for (j = 0; j < reclen; j++)
            {
                switch (rec[j].ID)
                {
                case LINE_R0_COEF:
                    line_r0.SetCoefInfo(rec[j]);
                    break;

                case LINE_R0_RANGE:
                    line_r0.SetViewInfo(rec[j]);
                    break;

                case LINE_L0_COEF:
                    line_l0.SetCoefInfo(rec[j]);
                    break;

                case LINE_L0_RANGE:
                    line_l0.SetViewInfo(rec[j]);
                    break;

                case LINE_R1_COEF:
                    line_r1.SetCoefInfo(rec[j]);
                    break;

                case LINE_R1_RANGE:
                    line_r1.SetViewInfo(rec[j]);
                    break;

                case LINE_L1_COEF:
                    line_l1.SetCoefInfo(rec[j]);
                    break;

                case LINE_L1_RANGE:
                    line_l1.SetViewInfo(rec[j]);
                    break;

                case OBJ_0_A:
                    Objt_0.SetCanAInfo(rec[j]);
                    break;

                case OBJ_0_B:
                    Objt_0.SetCanBInfo(rec[j]);
                    break;

                case OBJ_1_A:
                    Objt_1.SetCanAInfo(rec[j]);
                    break;

                case OBJ_1_B:
                    Objt_1.SetCanBInfo(rec[j]);
                    break;

                case OBJ_2_A:
                    Objt_2.SetCanAInfo(rec[j]);
                    break;

                case OBJ_2_B:
                    Objt_2.SetCanBInfo(rec[j]);
                    break;

                case OBJ_3_A:
                    Objt_3.SetCanAInfo(rec[j]);
                    break;

                case OBJ_3_B:
                    Objt_3.SetCanBInfo(rec[j]);
                    break;

                case OBJ_4_A:
                    Objt_4.SetCanAInfo(rec[j]);
                    break;

                case OBJ_4_B:
                    Objt_4.SetCanBInfo(rec[j]);
                    break;

                case OBJ_5_A:
                    Objt_5.SetCanAInfo(rec[j]);
                    break;

                case OBJ_5_B:
                    Objt_5.SetCanBInfo(rec[j]);
                    break;

                case OBJ_6_A:
                    Objt_6.SetCanAInfo(rec[j]);
                    break;

                case OBJ_6_B:
                    Objt_6.SetCanBInfo(rec[j]);
                    break;

                case OBJ_7_A:
                    Objt_7.SetCanAInfo(rec[j]);
                    break;

                case OBJ_7_B:
                    Objt_7.SetCanBInfo(rec[j]);
                    break;

                case OBJ_8_A:
                    Objt_8.SetCanAInfo(rec[j]);
                    break;

                case OBJ_8_B:
                    Objt_8.SetCanBInfo(rec[j]);
                    break;

                case OBJ_9_A:
                    Objt_9.SetCanAInfo(rec[j]);
                    break;

                case OBJ_9_B:
                    Objt_9.SetCanBInfo(rec[j]);
                    break;

                default:
                    // printf("Wrong CAN ID\n");
                    break;
                }

                // Refresh Plot Data by the cycle 4 line refresh
                bool obj_flg = Objt_0.isReady() && Objt_1.isReady() && Objt_2.isReady() && Objt_3.isReady() && Objt_4.isReady();
                bool obj_flg_1 = Objt_5.isReady() && Objt_6.isReady() && Objt_7.isReady() && Objt_8.isReady() && Objt_9.isReady();
                bool line_flg = line_l0.isReady() && line_r0.isReady() && line_l1.isReady() && line_r1.isReady();
                data_mutex.lock();
                if (is_init == false)
                {
                    plot_data->obstacles.clear();
                    plot_data->obstacles.push_back(OBSTACLE(0.0, 0.0, 0.0, ego, true, 3.0, 1.8));
                    is_init = true;
                }
                if (obj_flg && obj_flg_1)
                {
                    plot_data->obstacles.clear();
                    plot_data->obstacles.push_back(OBSTACLE(0.0, 0.0, 0.0, ego, true, 3.0, 1.8));

                    if (Objt_0.isReady() && Objt_0.cpid != 0)
                    {
                        // Objt_0.DisplayObsInfo();
                        plot_data->obstacles.push_back(Objt_0.Objt);
                        Objt_0.ClearFlag();
                    }
                    if (Objt_1.isReady() && Objt_1.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_1.Objt);
                        Objt_1.ClearFlag();
                    }
                    if (Objt_2.isReady() && Objt_2.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_2.Objt);
                        Objt_2.ClearFlag();
                    }
                    if (Objt_3.isReady() && Objt_3.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_3.Objt);
                        Objt_3.ClearFlag();
                    }
                    if (Objt_4.isReady() && Objt_4.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_4.Objt);
                        Objt_4.ClearFlag();
                    }
                    if (Objt_5.isReady() && Objt_5.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_5.Objt);
                        Objt_5.ClearFlag();
                    }
                    if (Objt_6.isReady() && Objt_6.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_6.Objt);
                        Objt_6.ClearFlag();
                    }
                    if (Objt_7.isReady() && Objt_7.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_7.Objt);
                        Objt_7.ClearFlag();
                    }
                    if (Objt_8.isReady() && Objt_8.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_8.Objt);
                        Objt_8.ClearFlag();
                    }
                    if (Objt_9.isReady() && Objt_9.cpid != 0)
                    {
                        plot_data->obstacles.push_back(Objt_9.Objt);
                        Objt_9.ClearFlag();
                    }
                }

                if (line_flg)
                {
                    count++;
                    if (count > COUNT_MAX)
                    {
                        count = 0;
                        plot_data->lines.clear();
                        // printf("Line Trigg\n");
                        if (line_l0.isReady()) // 中间2层车道线
                        {
                            plot_data->lines.push_back(line_l0.LineInfo);
                            line_l0.ClearFlag();
                        }
                        if (line_r0.isReady())
                        {
                            plot_data->lines.push_back(line_r0.LineInfo);
                            line_r0.ClearFlag();
                        }
                        if (line_r1.isReady())
                        {
                            plot_data->lines.push_back(line_r1.LineInfo);
                            line_r1.ClearFlag();
                        }
                        if (line_l1.isReady()) // 中间2层车道线
                        {
                            plot_data->lines.push_back(line_l1.LineInfo);
                            line_l1.ClearFlag();
                        }
                    }
                    else
                    {
                        // TODO
                    }
                }
                data_mutex.unlock();
            } // End for
        }
        sleep(RUN_PERIOD);
    }
}
#endif // ONLINE_MODEL

#endif // DATA_RECEIVE_H
