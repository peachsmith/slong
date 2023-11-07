#include "input/input.h"
#include "scenes/scenes.h"

void sl_title_screen_input(cr_app *app)
{
    if (cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        app->done = 1;
        return;
    }

    if (cr_consume_input(app, CR_KEYCODE_SPACE))
    {
        cr_pop_input_handler(app);
        sl_clear_scene(app);
        sl_load_playing_field(app);
        cr_push_input_handler(app, sl_playing_field_input);
        return;
    }
}

void sl_playing_field_input(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    if (cr_consume_input(app, CR_KEYCODE_ESCAPE))
    {
        cr_pop_input_handler(app);
        sl_clear_scene(app);
        sl_load_title_screen(app);
        cr_push_input_handler(app, sl_title_screen_input);
        return;
    }

    if (cr_peek_input(app, CR_KEYCODE_RIGHTBRACKET))
        handles[SL_HANDLE_PLAYER_2]->data = 1;
}
