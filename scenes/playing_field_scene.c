#include "slong.h"
#include "input/input.h"
#include "scenes/scenes.h"
#include "entities/entity_types.h"

#include "common/util.h"

static int cleanup(MYSQL *con)
{
    if (con != NULL)
    {
        fprintf(stderr, "ERROR: %s\n", mysql_error(con));
        mysql_close(con);
    }

    return 1;
}

static int get_paddle_positions(int *p1, int *p2)
{
    MYSQL *con;
    MYSQL *real;
    MYSQL_RES *result;
    MYSQL_ROW row;

    // connect to a database
    if ((con = mysql_init(NULL)) == NULL)
    {
        return cleanup(con);
    }

    // external ip: 98.251.136.123
    // const char *host = "192.168.0.137"; // vm
    // const char *host = "192.168.0.197"; // laptop
    const char *host = "jepmac"; // mac
    real = mysql_real_connect(
        con,
        host,      // host
        "peach",   // username
        "smith",   // password
        "slong",   // database
        3306,      // port
        NULL,      // socket name
        0          // flags
    );
    if (real == NULL)
    {
        return cleanup(con);
    }

    // Execute a query.
    if (mysql_query(con, "SELECT * FROM paddles LIMIT 2"))
    {
        return cleanup(con);
    }

    // Examine the results.
    if ((result = mysql_store_result(con)) == NULL)
    {
        return cleanup(con);
    }

    if (row = mysql_fetch_row(result))
    {
        const char *id_str = row[0];
        const char *pos_str = row[1];

        if (strlen(id_str) > 1)
        {
            printf("invalid id for paddle 1\n");
            return cleanup(con);
        }

        if (strlen(pos_str) > 2)
        {
            printf("invalid position for paddle 1\n");
            return cleanup(con);
        }

        *p1 = atoi(pos_str);
    }

    if (row = mysql_fetch_row(result))
    {
        const char *id_str = row[0];
        const char *pos_str = row[1];

        if (strlen(id_str) > 1)
        {
            printf("invalid id for paddle 2\n");
            return cleanup(con);
        }

        if (strlen(pos_str) > 2)
        {
            printf("invalid position for paddle 2\n");
            return cleanup(con);
        }

        *p2 = atoi(pos_str);
    }

    // Clean up.
    mysql_free_result(result);
    mysql_close(con);
    return 0;
}

void sl_load_playing_field(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    app->scene = SL_SCENE_PLAYING_FIELD;

    util_set_camera(app, CR_CAMERA_NONE);
    app->cam.x = 0;
    app->cam.y = 0;

    // paddle boundaries:
    // 41 - 115

    sl_create_court(app); // court
    sl_create_boundary(app, 38, 31);
    sl_create_boundary(app, 38, 140);
    handles[SL_HANDLE_PLAYER_1] = sl_create_paddle(app, 38, 78);  // player 1
    handles[SL_HANDLE_PLAYER_2] = sl_create_paddle(app, 194, 78); // player 2
    sl_create_ball(app, 115, 80);                                 // ball

    app->pause = 0;
}

void sl_perform_playing_field(cr_app *app)
{
    cr_entity **handles = app->extension->entity_handles;

    if (!(app->ticks % 60))
    {
        int p1 = 0, p2 = 0;
        if (!get_paddle_positions(&p1, &p2))
        {
            // printf("player 1's paddle position: %d\n", p1);
            // printf("player 2's paddle position: %d\n", p2);

            if (p1 >= 1 && p1 <= 75)
            {
                handles[SL_HANDLE_PLAYER_1]->y_pos = p1 + 40;
            }

            if (p2 >= 1 && p2 <= 75)
            {
                handles[SL_HANDLE_PLAYER_2]->y_pos = p2 + 40;
            }
        }
    }
}
