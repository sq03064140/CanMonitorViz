#ifndef PLOT_DATA_H
#define PLOT_DATA_H

#include <vector>

namespace Monitor{

enum LINE_TYPE {full,dash,dual,edge,plan,forecast};

enum LINE_QI {undetected,bad,medium,good};

enum OBSTACLE_TYPE {car,trucks,bike,man,ego};

enum OBSTACLE_LANE {unknow_lane,left_left_lane,left_lane,middle_lane,right_lane,right_right_lane};

enum OBSTACLE_SOURCE{front_camera,front_radar,corner_radar,fusion};

class LINE{
public:
    LINE(float _c0=0,float _c1=0,float _c2=0,float _c3=0,float _c4=0,float _viewrange=100.0,LINE_TYPE _type=full,LINE_QI _QI=good){
        c0=_c0;c1=_c1;c2=_c2;c3=_c3;c4=_c4;
        view_range=_viewrange;type=_type;QI=_QI;
    }
    float c0;
    float c1;
    float c2;
    float c3;
    float c4;
    float view_range;
    LINE_TYPE type;
    LINE_QI QI;
};

class OBSTACLE{
public:
    OBSTACLE(float _x=0,float _y=0,float _head=0,OBSTACLE_TYPE _type=car,
             bool _valid=true,float _wheelbase=3.0,float _width=float(1.8),
             float _x_var=0,float _y_var=0,float _xy_var=0,OBSTACLE_LANE _lane=unknow_lane,OBSTACLE_SOURCE _source=front_camera){
        x=_x;y=_y;head=_head;
        type=_type;valid=_valid;
        wheelbase=_wheelbase;width=_width;
        x_var=_x_var;y_var=_y_var;xy_var=_xy_var;
        lane=_lane;source=_source;
    }
    float x;
    float y;
    float head;
    float x_var;
    float y_var;
    float xy_var;
    OBSTACLE_TYPE type;
    bool valid;
    float wheelbase;
    float width;
    OBSTACLE_LANE lane;
    OBSTACLE_SOURCE source;
};

class PLOT_DATA{
public:
    std::vector<LINE> lines;
    std::vector<OBSTACLE> obstacles;
};



}

#endif // PLOT_DATA_H
