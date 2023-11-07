#include "slong.h"

#include "assets.h"
#include "entities/entity_types.h"
#include "input/input.h"
#include "scenes/scenes.h"

#include "common/util.h"
#include "common/collision.h"

static cr_entity      entities       [SL_MAX_ENTITIES];
static cr_func        input_handlers [SL_MAX_INPUT_HANDLERS];
static cr_entity*     menus          [SL_MAX_MENUS];
static cr_entity*     dialogs        [SL_MAX_DIALOGS];
static cr_entity*     overlays       [SL_MAX_OVERLAYS];
static cr_texture*    textures       [SL_MAX_TEXTURES];
static cr_font*       fonts          [SL_MAX_FONTS];
static cr_sound*      sounds         [SL_MAX_SOUNDS];
static cr_entity_type entity_types   [SL_ENTITY_TYPE_MAX];
static int            counters       [SL_COUNTER_MAX];
static cr_entity*     entity_handles [SL_MAX_ENTITY_HANDLES];
static cr_extension   extension;

static void default_input(cr_app *app) { }

static int should_update(cr_app *app, cr_entity *ent)
{
    cr_entity_func update = app->entity_types[ent->type].update;
    int pause_flag = cr_check_flag(ent, ENTITY_FLAG_PAUSE);
    int menu_flag = cr_check_flag(ent, ENTITY_FLAG_MENU);

    if (!ent->present || update == NULL)
        return 0;

    if (app->pause)
        return pause_flag && !menu_flag;
    
    return !pause_flag;
}

static int should_render(cr_app *app, cr_entity *ent)
{
    cr_entity_func render = app->entity_types[ent->type].render;
    int pause_flag = cr_check_flag(ent, ENTITY_FLAG_PAUSE);
    int menu_flag = cr_check_flag(ent, ENTITY_FLAG_MENU);

    if (!ent->present || render == NULL)
        return 0;

    if (app->pause)
        return !menu_flag;

    return !pause_flag && !menu_flag;
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

    // Update dialogs.
    if (app->dialog_count > 0)
    {
        cr_entity *d = app->dialogs[app->dialog_count - 1];
        app->entity_types[d->type].update(app, d);
    }

    // Update menus.
    if (app->menu_count > 0)
    {
        cr_entity *m = app->menus[app->menu_count - 1];
        app->entity_types[m->type].update(app, m);
    }

    // Update overlays.
    if (app->overlay_count > 0)
    {
        cr_entity *o = app->overlays[app->overlay_count - 1];
        app->entity_types[o->type].update(app, o);
    }

    // Handle collisions.
    if (!app->pause)
    {
        common_handle_collisions(app);
    }

    // main update loop
    for (int i = 0; i < app->entity_cap; i++)
    {
        cr_entity *ent = &(app->entities[i]);
        cr_entity_type t = app->entity_types[ent->type];
        if (should_update(app, ent))
        {
            t.update(app, ent);
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
        if (should_render(app, ent))
        {
            t.render(app, ent);
        }
    }

    //------------------------------------------------------------------------
    // menu layer
    if (app->pause)
    {
        // Render all open menus.
        for (int i = 0; i < app->menu_count; i++)
        {
            cr_entity *m = app->menus[i];
            app->entity_types[m->type].render(app, m);
        }

        // Only render the dialog on the top of the stack.
        if (app->dialog_count > 0)
        {
            cr_entity *d = app->dialogs[app->dialog_count - 1];
            app->entity_types[d->type].render(app, d);
        }

        // Render screen overlay effect.
        for (int i = 0; i < app->overlay_count; i++)
        {
            cr_entity *o = app->overlays[i];
            app->entity_types[o->type].render(app, o);
        }
    }

    //------------------------------------------------------------------------
    // debug layer
}

int sl_init_app(cr_app *app)
{
    cr_set_title(app, "Slong");

    util_init_entity_types(&(entity_types[0]));
    util_init_entities(&(entities[0]));

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

    // Load all assets.
    if (!sl_load_all_assets(app))
        return 0;

    // Initialize the extension structure.
    for (int i = 0; i < SL_COUNTER_MAX; i++)
        counters[i] = 0;

    extension.counters = counters;
    extension.entity_handles = entity_handles;
    app->extension = &extension;

    // Register the entity types.
    sl_register_ball(&(app->entity_types[SL_ENTITY_TYPE_BALL]));
    sl_register_paddle(&(app->entity_types[SL_ENTITY_TYPE_PADDLE]));
    sl_register_court(&(app->entity_types[SL_ENTITY_TYPE_COURT]));
    sl_register_boundary(&(app->entity_types[SL_ENTITY_TYPE_BOUNDARY]));
    sl_register_main_menu(&(app->entity_types[SL_ENTITY_TYPE_MAIN_MENU]));

    // Push the default input handler.
    cr_push_input_handler(app, default_input);

    // Load the initial scene.
    sl_load_title_screen(app);
    cr_push_input_handler(app, sl_title_screen_input);

    // Seed random number generation.
    time_t t;
    srand((unsigned int)(time(&t) & INT_MAX));

    return 1;
}
