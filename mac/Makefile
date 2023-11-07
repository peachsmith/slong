#=============================================================================
# Makefile for macOS
#=============================================================================

CC=gcc

# This Makefile assumes that the MariaDB connector library was installed by Homebrew.
INCLUDE_DIR=-I"/usr/local/Cellar/mariadb/11.1.2/include/mysql" -I..
LINK_DIR=-L"/usr/local/Cellar/mariadb/11.1.2/lib" -F/Library/Frameworks

CFLAGS=-Wall -Werror
LIBS=-lmariadb -lcrumbs -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer

# This is needed to get access to SDL's main function without having to
# include the SDL header file.
MAIN_DEF=-D_main=_SDL_main

SRC=../*.c ../common/*.c ../entities/*.c ../input/*.c ../scenes/*.c ../data/*.c

all:
	$(CC) $(MAIN_DEF) $(SRC) -o slong $(INCLUDE_DIR) -mwindows $(LINK_DIR) $(LIBS)

debug:
	$(CC) -g $(MAIN_DEF) $(SRC) -o slong $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)
