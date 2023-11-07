#include "slong.h"

#include "assets.h"
#include "entities/entity_types.h"
#include "input/input.h"
#include "scenes/scenes.h"

#include "common/util.h"
#include "common/collision.h"
#include "common/dialog.h"

static cr_entity entities[SL_MAX_ENTITIES];
static cr_func input_handlers[SL_MAX_INPUT_HANDLERS];
static cr_entity *menus[SL_MAX_MENUS];
static cr_entity *dialogs[SL_MAX_DIALOGS];
static cr_entity *overlays[SL_MAX_OVERLAYS];
static cr_texture *textures[SL_MAX_TEXTURES];
static cr_font *fonts[SL_MAX_FONTS];
static cr_sound *sounds[SL_MAX_SOUNDS];
static cr_entity_type entity_types[SL_ENTITY_TYPE_MAX];

// 0 notes
// 1 critters
// 2 score
// [3:10] critter slots (8 slots)
static int counters[SL_COUNTER_MAX];
static cr_entity *entity_handles[SL_MAX_ENTITY_HANDLES];
static cr_extension extension;

static int default_get_x_vel(cr_entity *e)
{
    return e->x_vel;
}

static int default_get_y_vel(cr_entity *e)
{
    return e->y_vel;
}

static void default_input_handler(cr_app *app)
{
}

static void update(cr_app *app)
{
    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check < 1)
        {
            return;
        }
    }

    app->ticks++;

    // Handle input.
    if (app->input != NULL && app->input_count > 0)
    {
        app->input[app->input_count - 1](app);
    }

    // update dialogs
    if (app->dialog_count > 0)
    {
        cr_entity *d = app->dialogs[app->dialog_count - 1];
        app->entity_types[d->type].update(app, d);
    }

    // update menus
    if (app->menu_count > 0)
    {
        cr_entity *m = app->menus[app->menu_count - 1];
        app->entity_types[m->type].update(app, m);
    }

    // update overlays
    if (app->overlay_count > 0)
    {
        cr_entity *o = app->overlays[app->overlay_count - 1];
        app->entity_types[o->type].update(app, o);
    }

    // handle collisions
    if (!app->pause)
    {
        common_handle_collisions(app);
    }

    // main update loop
    for (int i = 0; i < app->entity_cap; i++)
    {
        cr_entity *ent = &(app->entities[i]);
        cr_entity_type t = app->entity_types[ent->type];
        int pause_flag = cr_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = cr_check_flag(ent, ENTITY_FLAG_MENU);
        if (ent->present && t.update != NULL)
        {
            if (app->pause)
            {
                if (pause_flag && !menu_flag)
                {
                    t.update(app, ent);
                }
            }
            else if (!pause_flag)
            {
                t.update(app, ent);
            }
        }
    }

    // scene behavior
    if (app->scene == SL_SCENE_PLAYING_FIELD)
    {
        sl_perform_playing_field(app);
    }
}

static void draw(cr_app *app)
{
    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check < 1)
        {
            return;
        }
    }

    //------------------------------------------------------------------------
    // background layer

    //------------------------------------------------------------------------
    // foreground layer
    for (int i = 0; i < app->entity_cap; i++)
    {
        cr_entity *ent = &(app->entities[i]);
        cr_entity_type t = app->entity_types[ent->type];
        int pause_flag = cr_check_flag(ent, ENTITY_FLAG_PAUSE);
        int menu_flag = cr_check_flag(ent, ENTITY_FLAG_MENU);
        if (ent->present && t.render != NULL)
        {
            // We render all entities without the pause flag, regardless of
            // whether or not the application is paused.
            // However, entities that have the pause flag will only be
            // rendered if the application is paused.
            if (app->pause)
            {
                if (!menu_flag)
                {
                    t.render(app, ent);
                }
            }
            else if (!pause_flag && !menu_flag)
            {
                t.render(app, ent);
            }
        }
    }

    //------------------------------------------------------------------------
    // menu layer
    if (app->pause)
    {
        // render all open menus
        for (int i = 0; i < app->menu_count; i++)
        {
            cr_entity *m = app->menus[i];
            app->entity_types[m->type].render(app, m);
        }

        // only render the dialog on the top of the stack
        if (app->dialog_count > 0)
        {
            cr_entity *d = app->dialogs[app->dialog_count - 1];
            app->entity_types[d->type].render(app, d);
        }

        // render screen overlay effects
        for (int i = 0; i < app->overlay_count; i++)
        {
            cr_entity *o = app->overlays[i];
            app->entity_types[o->type].render(app, o);
        }
    }

    //------------------------------------------------------------------------
    // debug layer

    if (app->debug.camera)
    {
        util_draw_camera(app);
    }
}

int sl_init_app(cr_app *app)
{
    cr_set_title(app, "Slong");

    // default values for entity types
    for (int i = 0; i < SL_ENTITY_TYPE_MAX; i++)
    {
        entity_types[i].id = 0;
        entity_types[i].width = 0;
        entity_types[i].height = 0;
        entity_types[i].render = NULL;
        entity_types[i].update = NULL;
        entity_types[i].advance = NULL;
        entity_types[i].collide = NULL;
        entity_types[i].get_x_vel = default_get_x_vel;
        entity_types[i].get_y_vel = default_get_y_vel;
        entity_types[i].interactable = 0;
        entity_types[i].interact = NULL;
        entity_types[i].control = 0;
        entity_types[i].spur = 0;
        entity_types[i].slope = 0;
    }

    // default values of entities
    for (int i = 0; i < SL_MAX_ENTITIES; i++)
    {
        entities[i].present = 0;
        entities[i].type = 0;
        entities[i].x_pos = 0;
        entities[i].y_pos = 0;
        entities[i].x_vel = 0;
        entities[i].y_vel = 0;
        entities[i].x_acc = 0;
        entities[i].y_acc = 0;
        entities[i].x_t = 0;
        entities[i].y_t = 0;
        entities[i].flags = 0;
        entities[i].data = 0;
        entities[i].animation_ticks = 0;
        entities[i].ticks = 0;
        entities[i].iframes = 0;
        entities[i].carrier = NULL;
        entities[i].text = NULL;
        entities[i].text_len = 0;
        entities[i].tick_limit = 0;
        entities[i].result = 0;
        entities[i].cursor_x = 0;
        entities[i].cursor_y = 0;
        entities[i].scroll_x = 0;
        entities[i].scroll_y = 0;
    }

    app->entity_cap = SL_MAX_ENTITIES;
    app->entities = entities;
    app->input = &(input_handlers[0]);
    app->menus = &(menus[0]);
    app->dialogs = &(dialogs[0]);
    app->overlays = &(overlays[0]);
    app->textures = &(textures[0]);
    app->fonts = &(fonts[0]);
    app->sounds = &(sounds[0]);
    app->entity_types = entity_types;
    app->update = update;
    app->draw = draw;

    // asset loading
    if (!sl_load_all_assets(app))
    {
        return 0;
    }

    // extension initialization
    for (int i = 0; i < SL_COUNTER_MAX; i++)
    {
        counters[i] = 0;
    }
    extension.counters = counters;
    extension.entity_handles = entity_handles;
    app->extension = &extension;

    // entity type registration
    sl_register_ball(&(app->entity_types[SL_ENTITY_TYPE_BALL]));
    sl_register_paddle(&(app->entity_types[SL_ENTITY_TYPE_PADDLE]));
    sl_register_court(&(app->entity_types[SL_ENTITY_TYPE_COURT]));
    sl_register_boundary(&(app->entity_types[SL_ENTITY_TYPE_BOUNDARY]));
    sl_register_main_menu(&(app->entity_types[SL_ENTITY_TYPE_MAIN_MENU]));

    // push the default input handler
    cr_push_input_handler(app, default_input_handler);

    // Load the initial scene.
    sl_load_title_screen(app);
    cr_push_input_handler(app, sl_title_screen_input);

    // Seed random number generation.
    time_t t;
    srand((unsigned int)(time(&t) & INT_MAX));

    return 1;
}
