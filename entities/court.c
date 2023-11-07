#include "slong.h"
#include "common/util.h"
#include "entities/entity_types.h"

#define COURT_BUFSIZE 256

static void render_court(cr_app *app, cr_entity *court)
{
    cr_entity **handles = app->extension->entity_handles;
    cr_font *font = app->fonts[SL_FONT_KENNY_PIXEL];
    int res;
    char buffer[COURT_BUFSIZE];
    int n = COURT_BUFSIZE - 1;
    int w = app->entity_types[court->type].width;
    int h = app->entity_types[court->type].height;

    // Render the background.
    cr_rect r = { .x = 0, .y = 0, .w = w, .h = h };
    cr_set_color(app, CR_COLOR_VINIK_BLUE);
    cr_draw_rect(app, &r, 1);

    // Render the court boundaries.
    cr_set_color(app, 0xFFF0F0F0);
    cr_point p0 = {.x = 38, .y = 40};
    cr_point p1 = {.x = 198, .y = 40};
    cr_draw_line(app, &p0, &p1);

    p0.y = 140;
    p1.y = 140;
    cr_draw_line(app, &p0, &p1);

    // Render player 1 information.
    res = snprintf(
        buffer,
        n,
        "PLAYER 1\nSCORE: %d\nPOSITION: %d",
        handles[SL_HANDLE_PLAYER_1]->cursor_x,
        handles[SL_HANDLE_PLAYER_1]->y_pos - 40);

    if (res < 0 || res >= n) return;

    cr_draw_text(app, font, buffer, 40, 2);

    // Render player 2 information.
    res = snprintf(
        buffer,
        n,
        "PLAYER 2\nSCORE: %d\nPOSITION: %d",
        handles[SL_HANDLE_PLAYER_2]->cursor_x,
        handles[SL_HANDLE_PLAYER_2]->y_pos - 40);

    if (res < 0 || res >= n) return;

    cr_draw_text(app, font, buffer, 138, 2);
}

static void update_court(cr_app *app, cr_entity *court)
{
}

void sl_register_court(cr_entity_type *t)
{
    t->width = 240;
    t->height = 160;
    t->render = render_court;
    t->update = update_court;
}

cr_entity *sl_create_court(cr_app *app)
{
    cr_entity *court = NULL;

    if ((court = cr_create_entity(app)) == NULL) return NULL;

    court->type = SL_ENTITY_TYPE_COURT;

    return court;
}
