#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "database.h"
#include "../core/tools/utils.h"


int create_database(void)
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(get_database_path(), &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    char *sql = "CREATE TABLE user(user_gid TEXT PRIMARY KEY, tcell_ip TEXT, "
                    "tcell_port TEXT, public_key TEXT);"
                "CREATE TABLE fileDesc(file_gid TEXT PRIMARY KEY, "
                    "file_id TEXT, size TEXT, path TEXT, type TEXT, "
                    "description TEXT, symmetric_key BLOB, "
                    "initialisation_vector BLOB);"
                "CREATE TABLE MyInfo(my_gid TEXT PRIMARY KEY, "
                    "my_tcell_ip TEXT, my_tcell_port TEXT, "
                    "my_public_key TEXT, my_private_key TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1;
}

