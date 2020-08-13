#include "../include/Draw.h"
//#include "../include/Data_Receive.h"

#include <iostream>

using namespace Monitor;
using namespace std;


int main(int argc, char** argv)
{

    PLOT_DATA plot_data;

    pthread_t tids[1]; //线程id
    int ret[1];

/********************Signal_Recieve_Thread******************************/
    ret[0] = pthread_create( &tids[0], nullptr, Signal_Recieve_Thread_Fun, (void*)&plot_data );
    if( ret[0] != 0 ) //创建线程成功返回0
    {
        cout << "Signal_Recieve_Thread----pthread_create error:error_code=" << ret[0] << endl;
    }


    DRAW draw(argc,argv,&plot_data);


    return 0;
}












