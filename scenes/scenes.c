#include "scenes/scenes.h"
#include "common/util.h"
#include "data/data.h"

void sl_clear_scene(cr_app *app)
{
    for (int i = 0; i < app->entity_cap; i++)
    {
        app->entities[i].present = 0;
    }

    for (int i = 0; i < SL_MAX_ENTITY_HANDLES; i++)
    {
        app->extension->entity_handles[i] = NULL;
    }

    app->entity_count = 0;
    app->menu_count = 0;
    app->dialog_count = 0;
    app->overlay_count = 0;
}

void sl_load_title_screen(cr_app *app)
{    
    app->scene = SL_SCENE_TITLE_SCREEN;

    util_set_camera(app, CR_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // Create the main menu and set it as the current active menu.
    cr_entity *main_menu = sl_create_main_menu(app);
    app->menus[app->menu_count++] = main_menu;
    app->pause = 1;
}

void sl_load_playing_field(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;
    cr_entity *player1;
    cr_entity *player2;

    app->scene = SL_SCENE_PLAYING_FIELD;

    util_set_camera(app, CR_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    sl_create_court(app);                     // court
    sl_create_boundary(app, 38, 31);          // upper boundary
    sl_create_boundary(app, 38, 140);         // lower boundary
    player1 = sl_create_paddle(app, 38, 78);  // player 1
    player2 = sl_create_paddle(app, 194, 78); // player 2
    sl_create_ball(app, 115, 80);             // ball

    handles[SL_HANDLE_PLAYER_1] = player1;
    handles[SL_HANDLE_PLAYER_2] = player2;

    app->pause = 0;
}

/**
 * Since the boundaries have a height of 10 and the paddles have a
 * height of 25, the position of the paddles must be within the range
 * [41, 115] in order to fit within the boundaries. The position of the paddle
 * within this range is called the screen position.
 *
 * To make it easier on the user, we require an input in the range [1, 75].
 * We call this the user position, and we add 40 to convert it to the screen
 * position.
 */
void sl_perform_playing_field(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;
    int p1;
    int p2;

    if (app->ticks % 60)
        return;

    if (sl_get_paddle_positions(&p1, &p2))
        return;

    if (p1 >= 1 && p1 <= 75)
        handles[SL_HANDLE_PLAYER_1]->y_pos = p1 + 40;

    if (p2 >= 1 && p2 <= 75)
        handles[SL_HANDLE_PLAYER_2]->y_pos = p2 + 40;
}
