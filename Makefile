## Makefile
#CC = gcc
#CFLAGS = -Wall -std=c99 -Iinclude
#LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11
#
#SRC = src/*.c
#OUT = game
#
#all:
#	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)
#
#raylib1: all
#	./$(OUT)
#
#clean:
#	rm -f $(OUT)


#CC = gcc
#
#CFLAGS = -Wall -std=c99 -Iinclude -I../raylib/src
#
#RAYLIB = ../raylib/src/libraylib.a
#
#LIBS = $(RAYLIB) -lm -ldl -lpthread -lGL -lX11
#
#SRC = src/*.c
#OUT = game_v1
#
#all:
#	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)
#
#run: all
#	./$(OUT)
#
#clean:
#	rm -f $(OUT)


CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc

CFLAGS = -Wall -std=c99 -Iinclude
WIN_CFLAGS = -Wall -std=c99 -Iinclude -I../raylib/src

SRC = $(wildcard src/*.c)

LINUX_OUT = build/linux/bug_hunter
WIN_OUT = build/win/bug_hunter.exe

LINUX_RAYLIB = ../raylib/linux/libraylib.a
WIN_RAYLIB = ../raylib/win/libraylib.a


.PHONY: linux win release clean


linux:
	mkdir -p build/linux
	$(CC) $(SRC) -o $(LINUX_OUT) $(CFLAGS) \
		$(LINUX_RAYLIB) \
		-lm -ldl -lpthread -lGL -lX11


win:
	mkdir -p build/win
	$(WIN_CC) $(SRC) -o $(WIN_OUT) $(WIN_CFLAGS) \
		$(WIN_RAYLIB) \
		-lopengl32 -lgdi32 -lwinmm


release: linux win
	rm -rf release

	mkdir -p release/bug_hunter-linux-x86_64
	mkdir -p release/bug_hunter-windows-x86_64

	cp build/linux/bug_hunter release/bug_hunter-linux-x86_64/
	cp build/win/bug_hunter.exe release/bug_hunter-windows-x86_64/

	cp -r levels release/bug_hunter-linux-x86_64/
	cp -r levels release/bug_hunter-windows-x86_64/


clean:
	rm -rf build release
