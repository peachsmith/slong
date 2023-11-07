#ifndef SL_SLONG_H
#define SL_SLONG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#include <crumbs/crumbs.h>
#include <mysql.h>

// fonts
#define SL_FONT_KENNY_PIXEL 0

// counters
#define SL_COUNTER_SCORE 0
#define SL_COUNTER_MAX 20

// entity handles
#define SL_HANDLE_PLAYER_1 0
#define SL_HANDLE_PLAYER_2 1
#define SL_HANDLE_BALL 2
#define SL_MAX_ENTITY_HANDLES 10

// limits
#define SL_MAX_ENTITIES 256
#define SL_MAX_INPUT_HANDLERS 20
#define SL_MAX_MENUS 10
#define SL_MAX_DIALOGS 10
#define SL_MAX_OVERLAYS 10
#define SL_MAX_TEXTURES 10
#define SL_MAX_FONTS 10
#define SL_MAX_SOUNDS 10

struct cr_extension
{
    cr_entity **entity_handles;
    int *counters;
};

int sl_init_app(cr_app *);

#endif