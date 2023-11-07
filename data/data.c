#include "data/data.h"

typedef struct sl_dbconfig
{
    const char *host;
    const char *username;
    const char *password;
    const char *database;
    unsigned int port;
    const char *socket;
} sl_dbconfig;

static sl_dbconfig local_conf = {
    .host = "localhost",
    .username = "player1",
    .password = "password1",
    .database = "slong",
    .port = 3306,
    .socket = "/tmp/mysql.sock"};

static sl_dbconfig remote_conf = {
    .host = "jepmac",
    .username = "slongremote",
    .password = "slongremote",
    .database = "slong",
    .port = 3306,
    .socket = NULL};

static int cleanup(MYSQL *con)
{
    if (con == NULL)
        return 1;

    fprintf(stderr, "ERROR: %s\n", mysql_error(con));
    mysql_close(con);

    return 1;
}

int sl_get_paddle_positions(int *p1, int *p2)
{
    MYSQL *con;
    MYSQL *real;
    MYSQL_RES *result;
    MYSQL_ROW row;
#ifdef SLONG_REMOTE
    const sl_dbconfig *conf = &remote_conf;
#else
    const sl_dbconfig *conf = &local_conf;
#endif

    // initialize
    if ((con = mysql_init(NULL)) == NULL)
        return cleanup(con);

    // connect
    real = mysql_real_connect(
        con,
        conf->host,
        conf->username,
        conf->password,
        conf->database,
        conf->port,
        conf->socket,
        0);

    if (real == NULL)
        return cleanup(con);

    // execute query
    if (mysql_query(con, "SELECT * FROM paddles LIMIT 2"))
        return cleanup(con);

    // extract results
    if ((result = mysql_store_result(con)) == NULL)
        return cleanup(con);

    // get player 1 paddle position
    if ((row = mysql_fetch_row(result)))
    {
        const char *id_str = row[0];
        const char *pos_str = row[1];

        if (strlen(id_str) > 1)
            return cleanup(con);
            
        if (strlen(pos_str) > 2)
            return cleanup(con);

        *p1 = atoi(pos_str);
    }

    // get player 2 paddle position
    if ((row = mysql_fetch_row(result)))
    {
        const char *id_str = row[0];
        const char *pos_str = row[1];

        if (strlen(id_str) > 1)
            return cleanup(con);

        if (strlen(pos_str) > 2)
            return cleanup(con);

        *p2 = atoi(pos_str);
    }

    mysql_free_result(result);
    mysql_close(con);

    return 0;
}
