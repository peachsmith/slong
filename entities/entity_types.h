#ifndef SL_ENTITY_TYPES_H
#define SL_ENTITY_TYPES_H

#include "slong.h"

#define SL_ENTITY_TYPE_PADDLE 0
#define SL_ENTITY_TYPE_BALL 1
#define SL_ENTITY_TYPE_COURT 2
#define SL_ENTITY_TYPE_MAIN_MENU 3
#define SL_ENTITY_TYPE_BOUNDARY 4
#define SL_ENTITY_TYPE_MAX 5

void sl_register_paddle(cr_entity_type *);
cr_entity *sl_create_paddle(cr_app *, int, int);

void sl_register_ball(cr_entity_type *);
cr_entity *sl_create_ball(cr_app *, int, int);

void sl_register_court(cr_entity_type *);
cr_entity *sl_create_court(cr_app *);

void sl_register_main_menu(cr_entity_type *);
cr_entity *sl_create_main_menu(cr_app *);

void sl_register_boundary(cr_entity_type *);
cr_entity *sl_create_boundary(cr_app *, int, int);

#endif