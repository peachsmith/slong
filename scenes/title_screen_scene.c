#include "slong.h"
#include "common/util.h"
#include "entities/entity_types.h"
#include "input/input.h"
#include "scenes/scenes.h"

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
