#ifndef DRAW_H
#define DRAW_H

#include "Plot_Data.h"
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <iostream>
//#include <GL/aux.h>
#include <math.h>
#include "Data_Receive.h"

#define PLOT_SCALE 0.1
#define MAX_CHAR  128

using namespace std;


namespace Monitor{

class DRAW{
public:
    DRAW(int &argc, char** argv, PLOT_DATA* const _plot_data)
    {
        /************  need mux !!! ***********/
        plot_data=_plot_data;
        /************  need mux !!! ***********/
        draw=this;
        srand((unsigned int)time(0));
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//使用双缓存模式和深度缓存
        glutInitWindowSize(800, 800);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Monitor");
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutIdleFunc(display);//设置空闲时调用的函数
        glutMainLoop();
    }

private:
    void init(void){
        //背景色
        glClearColor(0.0, 0.0, 0.0, 1.0);
        //将控制点坐标映射为曲线坐标
        //参数1：GL_MAP1_VERTEX_3，3维点坐标
        //参数2和3：控制参数t或u的取值范围[0, 1]
        //参数4：曲线内插值点间的步长3————3维坐标
        //参数5：曲线间的补偿为顶点数4个————总步长为12
        //参数6：控制点二维数组首元素地址

        //注意: 若是在这里设置了相关参数，后续对ctrlpoints内容更改曲线不变
        //glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

        //打开开关——允许3维坐标控制点到参数点转换开关
        glEnable(GL_MAP1_VERTEX_3);
        glShadeModel(GL_FLAT);

        //代码开关2：去掉本注释，可启用反走样
        /*
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);  //允许直线反走样
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);  // Antialias the lines
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        */
    }



    //3D空间中绘制2D效果，采用正交投影
    static void reshape(GLsizei w, GLsizei h)
    {
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (w <= h)
            glOrtho(-5.0, 5.0, -5.0*(GLfloat)h / (GLfloat)w, 5.0*(GLfloat)h / (GLfloat)w, -5.0, 5.0);
        else
            glOrtho(-5.0*(GLfloat)w / (GLfloat)h, 5.0*(GLfloat)w / (GLfloat)h, -5.0, 5.0, -5.0, 5.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    static void keyboard(unsigned char key, int x, int y)
    {
        if(key=='p'){
            cout<<"Pause or Start !!!"<<endl;
            pause_status=!pause_status;
        }
        else if (key==27) {   //ESC键
            exit(0);
        }
        return;

//        switch (key)
//        {
//        case 'p':{
//            cout<<"PPPPPPPP"<<endl;
//            pause_mutex.lock();
//        };
//        case 'g':{
//            cout<<"GGGGGGGG"<<endl;
//            pause_mutex.unlock();
//        };
//        case 27:{   //ESC键
//            cout<<"KKKKKKK"<<endl;
//            exit(0);
//            break;
//        };
//        default:
//            break;
//        }
    }







    static void display(void)
    {

        if(pause_status) return;


        glClear(GL_COLOR_BUFFER_BIT);
        /* 显示控制点 */
        glLoadIdentity();


        PLOT_DATA tmp_plot_data;
        data_mutex.lock();
        tmp_plot_data=*draw->plot_data;
        data_mutex.unlock();
        /***************************************** Draw the lines************************************************/


        glTranslatef(0.0f, -2.0f, 0.0f); //Axis Ofset Seting !!!!!! --Daike Kang
        glPointSize(3.0);
        glBegin(GL_POINTS);

        for(size_t i=0;i<tmp_plot_data.lines.size();i++){
           if(tmp_plot_data.lines[i].type==plan){
                       glColor3f(1.0, 0.0, 0.0);  //Color is here !!!!!!!!!!!! --Daike Kang
           }
           else if (tmp_plot_data.lines[i].type==forecast) {
                          glColor3f(0.0, 1.0, 1.0);  //Color is here !!!!!!!!!!!! --Daike Kang
           }
           else {
                         glColor3f(0.0, 1.0, 0.0);  //Color is here !!!!!!!!!!!! --Daike Kang
           }

           vector<vector<GLfloat>> plot_points;
            plot_points=draw->get_line_point(tmp_plot_data.lines[i],PLOT_SCALE);
            for (size_t i = 0; i < plot_points.size(); i++){
                glVertex3fv(&plot_points[i][0]);
            }
        }
        glEnd();


        /***************************************** Draw the obstacles************************************************/
        //glTranslatef(0.0f, -2.0f, 0.0f); //Axis Ofset Seting !!!!!! --Daike Kang
        //glPointSize(5.0);
        glBegin(GL_POINTS);

        for(size_t i=0;i<tmp_plot_data.obstacles.size();i++){
           if(tmp_plot_data.obstacles[i].type==ego){
                       glColor3f(1.0, 0.0, 0.0);  //Color is here !!!!!!!!!!!! --Daike Kang
           }
           else {
               if(tmp_plot_data.obstacles[i].lane==left_lane || tmp_plot_data.obstacles[i].lane==right_lane){
                    glColor3f(0.0, 1.0, 1.0);  //Color is here !!!!!!!!!!!! --Daike Kang
               }
               else if(tmp_plot_data.obstacles[i].lane==middle_lane){
                    glColor3f(0.0, 0.5, 1.5);  //Color is here !!!!!!!!!!!! --Daike Kang
               }
               else {
                    glColor3f(1.0, 1.0, 1.0);  //Color is here !!!!!!!!!!!! --Daike Kang
               }

           }

           vector<vector<GLfloat>> plot_points;
            plot_points=draw->get_obstacle_point(tmp_plot_data.obstacles[i],PLOT_SCALE,0.25);
            for (size_t i = 0; i < plot_points.size(); i++){
                glVertex3fv(&plot_points[i][0]);
            }
        }

        glEnd();

        /***************************************** Draw the obstacles************************************************/

        glColor3f(1.0f, 0.0f, 0.0f);
         glRasterPos2f(0.0f, 0.0f);


        glutSwapBuffers();
    }


    vector<vector<GLfloat>> get_line_point(const LINE& _line,float _plot_scale){
        vector<vector<GLfloat>> res;
        if(_line.QI==undetected) return res;
        float jiange;
        if(_line.type==dash) jiange=2.0;
        else if (_line.type==plan || _line.type==forecast) jiange=1.0;
        else jiange=0.5;

        for(float x=0;x<_line.view_range;x+=jiange){
            float y=_line.c0+x*_line.c1+pow(x,2)*_line.c2+pow(x,3)*_line.c3+pow(x,3)*_line.c4;
            res.push_back({-_plot_scale*y,_plot_scale*x,0.0});
        }
        return res;
    }

    vector<vector<GLfloat>> get_obstacle_point(const OBSTACLE& _obstacle,float _plot_scale,float _sample_scale){
        vector<vector<GLfloat>> res;
        vector<float> head_1={float(cos(_obstacle.head)),float(sin(_obstacle.head))};
        vector<float> head_2={float(-sin(_obstacle.head)),float(cos(_obstacle.head))};
        for(float i=-1.0;i<1.1;i+=_sample_scale){
            for(float j=-1.0;j<1.1;j+=_sample_scale){
                float tmp_x=_obstacle.x+head_1[0]*_obstacle.wheelbase*0.5*i+head_2[0]*_obstacle.width*0.5*j;
                float tmp_y=_obstacle.y+head_1[1]*_obstacle.wheelbase*0.5*i+head_2[1]*_obstacle.width*0.5*j;
                res.push_back({-_plot_scale*tmp_y,_plot_scale*tmp_x, 0.0});
            }
        }
        return res;
    }



private:
    //vector<vector<GLfloat>> ctrlpoints;
    PLOT_DATA* plot_data;
    static DRAW* draw;
};

DRAW* DRAW::draw;                        //静态成员初始化
}

#endif // DRAW_H

