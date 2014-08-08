CC=g++
CFLAGS=-Wall -Wextra -std=c++11
LFLAGS=-lSDL2

all: fileio.o m4.o display.o main

main: fileio.o m4.o display.o control.o main.cpp
	$(CC) $(CFLAGS) fileio.o m4.o display.o control.o main.cpp -o $@ $(LFLAGS)

fileio.o: fileio.h fileio.cpp
	$(CC) $(CFLAGS) -c fileio.cpp

m4.o: m4.h m4.cpp
	$(CC) $(CFLAGS) -c m4.cpp

display.o: m4.o display.h display.cpp
	$(CC) $(CFLAGS) -c display.cpp

control.o: m4.o control.h control.cpp
	$(CC) $(CFLAGS) -c control.cpp

clean:
	rm *.o
