#include "slong.h"
#include "common/collision.h"
#include "common/util.h"
#include "entities/entity_types.h"

static int get_ball_x_vel(cr_entity *ball)
{
    if (ball->x_vel > 1 || ball->x_vel < 1)
    {
        if (!(ball->ticks % ball->x_vel))
            return ball->x_vel > 0 ? 1 : -1;

        return 0;
    }

    return ball->x_vel;
}

static int get_ball_y_vel(cr_entity *ball)
{
    if (ball->y_vel > 1 || ball->y_vel < 1)
    {
        if (!(ball->ticks % ball->y_vel))
            return ball->y_vel > 0 ? 1 : -1;

        return 0;
    }

    return ball->y_vel;
}

static void render_ball(cr_app *app, cr_entity *ball)
{
    if (!util_is_on_screen(app, ball))
        return;

    cr_rect r = {
        .x = ball->x_pos,
        .y = ball->y_pos,
        .w = app->entity_types[ball->type].width,
        .h = app->entity_types[ball->type].height,
    };

    cr_set_color(app, 0xFFF0F0F0);
    cr_draw_rect(app, &r, 1);
}

static void update_ball(cr_app *app, cr_entity *ball)
{
    if (app->ticks % 5)
        return;

    cr_entity **handles = app->extension->entity_handles;
    int x_vel = app->entity_types[ball->type].get_x_vel(ball);
    int y_vel = app->entity_types[ball->type].get_y_vel(ball);

    ball->x_pos += x_vel;
    ball->y_pos += y_vel;

    if (!util_is_on_screen(app, ball))
    {
        // Increment the appropriate score counter.
        if (ball->x_pos < 100) handles[SL_HANDLE_PLAYER_2]->cursor_x++;
        if (ball->x_pos >= 100) handles[SL_HANDLE_PLAYER_1]->cursor_x++;

        // Reset the ball
        ball->x_pos = 115;
        ball->y_pos = 80;
        ball->ticks = 0;
        ball->x_vel = -ball->x_vel;
        ball->y_vel = (rand() % 6) - 3;

        if (ball->y_vel == 0) ball->y_vel = -1;
    }

    ball->ticks++;
}

static void collide_ball(
    cr_app *app,
    cr_entity *ball,
    cr_entity *other,
    cr_collision *t_res)
{
    if (other->data)
        return;

    if (t_res->cn.x)
        ball->x_vel = -ball->x_vel;
    
    if (t_res->cn.y)
        ball->y_vel = -ball->y_vel;
}

void sl_register_ball(cr_entity_type *t)
{
    t->width = 5;
    t->height = 5;
    t->render = render_ball;
    t->update = update_ball;
    t->get_x_vel = get_ball_x_vel;
    t->get_y_vel = get_ball_y_vel;
    t->collide = collide_ball;
    t->spur = 1;
}

cr_entity *sl_create_ball(cr_app *app, int x, int y)
{
    cr_entity *ball = NULL;

    if ((ball = cr_create_entity(app)) == NULL)
        return NULL;

    ball->type = SL_ENTITY_TYPE_BALL;
    ball->x_pos = x;
    ball->y_pos = y;
    ball->x_vel = 1;
    ball->y_vel = 1;

    return ball;
}
