HEADERS += \
    #App.h   \
    include/Draw.h   \
    include/Plot_Data.h  \
    include/Data_Receive.h  \
    include/Obstacles.h  \
    include/Lane.h  \
    include/controlcan.h \
    \
    config/config.h

SOURCES += \
    #Draw.cpp \
    src/main.cpp \
    #src/Data_Receive.cpp  \
    src/Obstacles.cpp \
    src/Lane.cpp

INCLUDEPATH +=\
               /usr/local/include
               #/usr/local/include/opencv    \
               #/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_core.so \
        /my_work/Opencv_Draw/Real_Time_Monitor_Visualize/Lib/libcontrolcan.so
#        /usr/local/lib/libopencv_calib3d.so\
#        /usr/local/lib/libopencv_contrib.so\
#        /usr/local/lib/libopencv_features2d.so\
#        /usr/local/lib/libopencv_flann.so\
#        /usr/local/lib/libopencv_gpu.so\
#        /usr/local/lib/libopencv_highgui.so\
#        /usr/local/lib/libopencv_imgproc.so\
#        /usr/local/lib/libopencv_legacy.so\
#        /usr/local/lib/libopencv_ml.so\
#        /usr/local/lib/libopencv_nonfree.so\
#        /usr/local/lib/libopencv_objdetect.so\
#        /usr/local/lib/libopencv_ocl.so\
#        /usr/local/lib/libopencv_photo.so\
#        /usr/local/lib/libopencv_stitching.so\
#        /usr/local/lib/libopencv_superres.so\
#        /usr/local/lib/libopencv_video.so\
#        /usr/local/lib/libopencv_videostab.so
LIBS += -lGL -lglut


#TARGET = ./Draw.bin
