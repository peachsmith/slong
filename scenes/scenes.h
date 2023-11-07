#ifndef SL_SCENES_H
#define SL_SCENES_H

#include "slong.h"

// scenes
#define SL_SCENE_TITLE_SCREEN 0
#define SL_SCENE_PLAYING_FIELD 1

void sl_clear_scene(cr_app *);

// scene loaders
void sl_load_title_screen(cr_app *);
void sl_load_playing_field(cr_app *);

// scene behavior
void sl_perform_playing_field(cr_app*);

#endif