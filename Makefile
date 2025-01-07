CC=g++
CFLAGS=-Iinclude

SRC=$(wildcard src/**/*.cpp)
OBJ=$(SRC:.cpp=.o)

all: main

main: $(OBJ)
	$(CC) -o MediaPlayer main.cpp $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ) MediaPlayer
