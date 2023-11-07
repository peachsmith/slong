#include "slong.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    printf("Slong: slow pong\n");
    printf("Version: 1.0.0-alpha\n");
    printf("Author: John Powell\n");

    if (!cr_initialize())
    {
        return 1;
    }

    cr_app *app = cr_create_app();
    if (app == NULL)
    {
        cr_terminate();
        return 1;
    }

    if (!sl_init_app(app))
    {
        cr_destroy_app(app);
        cr_terminate();
        return 1;
    }

    while (!app->done)
    {
        cr_begin_frame(app);
        app->update(app);
        app->draw(app);
        cr_end_frame(app);
    }

    cr_destroy_app(app);
    cr_terminate();

    return 0;
}
