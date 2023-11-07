#include "slong.h"
#include "common/util.h"
#include "entities/entity_types.h"

void sl_register_boundary(cr_entity_type *t)
{
    t->width = 161;
    t->height = 10;
}

cr_entity *sl_create_boundary(cr_app *app, int x, int y)
{
    cr_entity *boundary = NULL;

    if ((boundary = cr_create_entity(app)) == NULL) return NULL;

    boundary->type = SL_ENTITY_TYPE_BOUNDARY;
    boundary->x_pos = x;
    boundary->y_pos = y;

    return boundary;
}
