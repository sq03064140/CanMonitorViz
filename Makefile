
all:
	#gcc -o hello -L. -lcontrolcan -lpthread -lusb main.c
	#arm-none-linux-gnueabi-gcc -o hello -L. -L /home/caidunqing/controlcan -lcontrolcan -lpthread main.c
	g++ -std=c++11 -o monitor src/main.cpp src/Obstacles.cpp src/Lane.cpp /home/gaohang/code/CanMonitor/Lib/libcontrolcan.so -lpthread -lGL -lglut

clean:
	rm -f *.o monitor
