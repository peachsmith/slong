#=============================================================================
# Makefile for MinGW-w64
#=============================================================================

CC=gcc

INCLUDE_DIR=-I"$(CRUMBS_HOME)/mingw/include" -I"$(MARIADB_HOME)/include" -I..
LINK_DIR=-L"$(CRUMBS_HOME)/mingw/lib" -L"$(MARIADB_HOME)/lib"

CFLAGS=-Wall -Werror
LIBS=-lmingw32 -lmariadb -lcrumbs -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# This is needed to get access to SDL's main function without having to
# include the SDL header file.
MAIN_DEF=-Dmain=SDL_main

# When building on Windows, we will be connecting to the database remotely (on LAN).
DEFS=-D"SLONG_REMOTE=1"

SRC=../*.c ../common/*.c ../data/*.c ../entities/*.c ../input/*.c ../scenes/*.c

all:
	$(CC) $(MAIN_DEF) $(DEFS) $(SRC) -o slong.exe $(INCLUDE_DIR) -mwindows $(LINK_DIR) $(LIBS)

debug:
	$(CC) -g $(MAIN_DEF) $(DEFS) $(SRC) -o slong.exe $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)
