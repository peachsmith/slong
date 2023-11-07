#include "slong.h"
#include "assets.h"
#include "common/util.h"
#include "entities/entity_types.h"

static void render_main_menu(cr_app *app, cr_entity *menu)
{
    cr_font *font = app->fonts[SL_FONT_KENNY_PIXEL];
    int x_origin = 60;
    int y_origin = 20;
    int w = app->entity_types[menu->type].width;
    int h = app->entity_types[menu->type].height;

    // Render the menu background.
    cr_rect r = { .x = 0, .y = 0, .w = w, .h = h };
    cr_set_color(app, CR_COLOR_VINIK_BLUE);
    cr_draw_rect(app, &r, 1);

    // Render the menu text.
    cr_draw_text(app, font, "SLONG",                 x_origin + 43, y_origin);
    cr_draw_text(app, font, "(Slow Pong)",           x_origin + 30, y_origin + 15);
    cr_draw_text(app, font, "Press space to start.", x_origin + 8,  y_origin + 60);
}

static void update_main_menu(cr_app *app, cr_entity *menu)
{
}

void sl_register_main_menu(cr_entity_type *t)
{
    t->width = 240;
    t->height = 160;

    t->render = render_main_menu;
    t->update = update_main_menu;
}

cr_entity *sl_create_main_menu(cr_app *app)
{
    cr_entity *menu = NULL;

    if ((menu = cr_create_entity(app)) == NULL)
        return NULL;

    menu->type = SL_ENTITY_TYPE_MAIN_MENU;
    cr_set_flag(menu, ENTITY_FLAG_PAUSE);
    cr_set_flag(menu, ENTITY_FLAG_MENU);

    return menu;
}
