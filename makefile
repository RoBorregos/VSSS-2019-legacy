CC = g++
CFLAGS = -std=c++11
SRCS = main.cpp
PROG = main

OPENCV = `pkg-config opencv --cflags --libs`
LIBS =  libraries/packet/packet.cpp libraries/serial_transmitt/serial_transmitt.cpp libraries/vision/circle_detection.cpp

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(OPENCV)