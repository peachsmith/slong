#include "slong.h"
#include "common/util.h"
#include "entities/entity_types.h"

static void render_paddle(cr_app *app, cr_entity *paddle)
{
    if (paddle->data)
    {
        if (paddle->ticks < 60)
        {
            cr_font *font = app->fonts[SL_FONT_KENNY_PIXEL];
            cr_draw_text(app, font, "HOUSE RULES!", paddle->x_pos - 60, 80);
        }

        return;
    }

    cr_rect r = {
        .x = paddle->x_pos,
        .y = paddle->y_pos,
        .w = app->entity_types[paddle->type].width,
        .h = app->entity_types[paddle->type].height,
    };

    cr_set_color(app, 0xFFF0F0F0);
    cr_draw_rect(app, &r, 1);
}

static void update_paddle(cr_app *app, cr_entity *paddle)
{
    if (!paddle->data) return;

    paddle->ticks++;

    if (paddle->ticks > 120) paddle->present = 0;
}

void sl_register_paddle(cr_entity_type *t)
{
    t->width = 5;
    t->height = 25;
    t->render = render_paddle;
    t->update = update_paddle;
}

cr_entity *sl_create_paddle(cr_app *app, int x, int y)
{
    cr_entity *paddle = NULL;

    if ((paddle = cr_create_entity(app)) == NULL) return NULL;

    paddle->type = SL_ENTITY_TYPE_PADDLE;
    paddle->x_pos = x;
    paddle->y_pos = y;

    return paddle;
}
