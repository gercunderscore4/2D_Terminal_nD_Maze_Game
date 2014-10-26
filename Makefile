CC=g++
CFLAGS=-Wall -Wextra
EXEC=m

.PHONY: all
all: m4.o engine.o main

.PHONY: main
main: m4.o engine.o
	$(CC) $(CFLAGS) m4.o engine.o main.cpp -o $(EXEC) $(LFLAGS)

engine.o: engine.h engine.cpp
	$(CC) $(CFLAGS) -c engine.cpp

m4.o: m4.h m4.cpp
	$(CC) $(CFLAGS) -c m4.cpp

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: cleanall
cleanall:
	rm -rf *.o
	rm -f $(EXEC)
