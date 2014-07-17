# lazy makefile

SHELL = /bin/sh
CC = g++
CPPFLAGS = -g -Wall -std=c++11
#CFLAGS = -I$(srcdir)
#LDFLAGS = -L$(srcdir)
#LDLIBS = 

.SUFFIXES:
.SUFFIXES: .cpp .hpp .o

SOURCES = m.cpp m4.cpp
EXEC = m

.PHONY: all
all:
	$(CC) $(CPPFLAGS) $(SOURCES) -o $(EXEC)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(EXEC)
