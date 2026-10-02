# Makefile
CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude
LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11

SRC = src/*.c
OUT = game

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)

raylib1: all
	./$(OUT)

clean:
	rm -f $(OUT)

