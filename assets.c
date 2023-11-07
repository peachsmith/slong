#include "assets.h"

#include <stdio.h>
#include <stdlib.h>

int sl_load_all_assets(cr_app *app)
{
    // fonts
    if (cr_load_font(app, "assets/fonts/Kenney Pixel.ttf", 16) == NULL)
    {
        fprintf(stderr, "failed to load Kenney Pixel font\n");
        return 0;
    }

    return 1;
}
