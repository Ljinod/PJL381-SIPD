/**
 * @file    database.c
 * @brief   Database functions.
 * @author  Loudet  Julien   <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015/09)
 */


#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* atoi, exit */

#include "database.h"
#include "../core/beans/file_desc.h"
#include "../core/beans/user.h"


/**
 * @brief XXX Add brief description!
 *
 * @param fdesc
 *
 * @return
 */
int insertFile(FileDesc_t* fdesc)
{
    /* Escape file descriptor */
    safe_file_desc_for_db(fdesc);

    /* Now we can work with it */
    char *filegid     = fdesc->file_gid;
    char *fileid      = fdesc->file_id;
    char *filesize    = fdesc->size;
    char *path        = fdesc->path;
    char *type        = fdesc->type;
    char *description = fdesc->description;
    char *k           = fdesc->symmetric_key;
    char *iv          = fdesc->initialisation_vector;

    sqlite3 *db;
    char    *err_msg = 0;

    int rc = sqlite3_open(get_database_path(), &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    char sql[(
            strlen(filegid) + strlen(fileid) + strlen(k) + strlen(type) +
            strlen(description) + strlen(iv) + strlen(filesize) +
            strlen(path) + 100
            ) * sizeof(char)];

    strcpy(sql,"INSERT INTO fileDesc VALUES('");
    strcat(sql, filegid);
    strcat(sql, "','");
    strcat(sql, fileid);
    strcat(sql, "','");
    strcat(sql, filesize);
    strcat(sql, "','");
    strcat(sql, path);
    strcat(sql, "','");
    strcat(sql, type);
    strcat(sql, "','");
    strcat(sql, description);
    strcat(sql, "','");
    strcat(sql, k);
    strcat(sql, "','");
    strcat(sql, iv);
    strcat(sql, "');");

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(-1);
    }

    sqlite3_close(db);

    return 1;
}


/**
 * @brief XXX Add brief description!
 *
 * @param user
 *
 * @return
 */
int insertUser(User_t* user)
{
    /* Escape the user structure */
    safe_user_for_db(user);

    /* Now we can work with it */
    const char *usergid   = user->user_gid;
    const char *tcellip   = user->tcell_ip;
    const char *tcellport = user->tcell_port;
    const char *publickey = user->public_key;

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(get_database_path(), &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    char sql[(
            strlen(usergid) + strlen(tcellip) + strlen(publickey) +
            strlen(tcellport) + 100
            )];
    strcpy(sql,"INSERT INTO user VALUES('");
    strcat(sql, usergid);
    strcat(sql, "','");
    strcat(sql, tcellip);
    strcat(sql, "','");
    strcat(sql, tcellport);
    strcat(sql, "','");
    strcat(sql, publickey);
    strcat(sql, "');");

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(-1);
    }

    sqlite3_close(db);

    return 1;
}


/*
 * int insertMyInfo(char *myPrivateKey, char *MyTCellIP, char *MyUsergid)
 * {
 *   sqlite3 *db;
 *   char    *err_msg = 0;
 *
 *   int rc = sqlite3_open(DB_NAME, &db);
 *
 *   if (rc != SQLITE_OK)
 *   {
 *     fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
 *     sqlite3_close(db);
 *
 *     return 0;
 *   }
 *
 *   char sql[strlen(myPrivateKey) + strlen(MyTCellIP) + strlen(MyUsergid) + 100];
 *   strcpy(sql,"INSERT INTO MyInfo VALUES('");
 *   strcat(sql, myPrivateKey);
 *   strcat(sql, "','");
 *   strcat(sql, MyTCellIP);
 *   strcat(sql, "','");
 *   strcat(sql, MyUsergid);
 *   strcat(sql, "');");
 *
 *   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
 *
 *   if (rc != SQLITE_OK )
 *   {
 *     fprintf(stderr, "SQL error: %s\n", err_msg);
 *
 *     sqlite3_free(err_msg);
 *     sqlite3_close(db);
 *
 *     return 0;
 *   }
 *
 *   sqlite3_close(db);
 *
 *   return 1;
 * }
 */


/**
 * @brief XXX Add brief description!
 *
 * @param filegid
 *
 * @return
 */
FileDesc_t* getFileDesc (const char* filegid)
{
    FileDesc_t* fileDesc = malloc(sizeof(FileDesc_t));

    sqlite3      *db;
    sqlite3_stmt *res;

    int rc = sqlite3_open(get_database_path(), &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    char *sql = "SELECT * FROM fileDesc WHERE file_gid = @id";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK)
    {
        int idx = sqlite3_bind_parameter_index(res, "@id");
        sqlite3_bind_text(res, idx, filegid, strlen(filegid), SQLITE_STATIC);

    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    int step = sqlite3_step(res);

    if (step == SQLITE_ROW)
    {
        strcpy(fileDesc->file_gid     , (char *)sqlite3_column_text(res, 0));

        strcpy(fileDesc->file_id      , (char *)sqlite3_column_text(res, 1));

        strcpy(fileDesc->size         , (char *)sqlite3_column_text(res, 2));

        strcpy(fileDesc->path         , (char *)sqlite3_column_text(res, 3));

        strcpy(fileDesc->type         , (char *)sqlite3_column_text(res, 4));

        strcpy(fileDesc->description  , (char *)sqlite3_column_text(res, 5));

        strcpy(fileDesc->symmetric_key, (char *)sqlite3_column_text(res, 6));

        strcpy(fileDesc->initialisation_vector,
                (char *)sqlite3_column_text(res, 7));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return fileDesc;
}



int getFileDescNumber(void)
{
    sqlite3      *db;
    char         *sql;
    sqlite3_stmt *stmt;
    int           row = 0;
    sqlite3_open(get_database_path(), &db);

    sql = "SELECT COUNT(*) from fileDesc";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW)
    {
        row = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return row;
}



/**
 * @brief XXX Add brief description!
 *
 * @return
 */
FileDesc_t * getAllFileDesc(void)
{

    sqlite3 * db;
    char * sql;
    sqlite3_stmt * stmt;
    int row = 0;

    sqlite3_open(get_database_path(), &db);

    sql = "SELECT COUNT(*) from fileDesc";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW)
    {
        row = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    FileDesc_t * result = malloc((row+1)*sizeof(FileDesc_t));

    sql = "SELECT * FROM fileDesc";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int i = 0;
    while (1)
    {
        int s;

        s = sqlite3_step (stmt);
        if (s == SQLITE_ROW)
        {
            strcpy(result[i].file_gid   , (char *)sqlite3_column_text(stmt, 0));

            strcpy(result[i].file_id    , (char *)sqlite3_column_text(stmt, 1));

            strcpy(result[i].type       , (char *)sqlite3_column_text(stmt, 4));

            strcpy(result[i].description, (char *)sqlite3_column_text(stmt, 5));

            i++;
        }
        else if (s == SQLITE_DONE) {
            break;
        }
        else {
            fprintf (stderr, "Failed.\n");
            sqlite3_close(db);
            exit(-1);
        }
    }
    sqlite3_close(db);
    return result;
}


/*
 * char* getPrivateKey (char* usergid)
 * {
 *
 *   sqlite3 *db;
 *   sqlite3_stmt *res;
 *
 *   int rc = sqlite3_open(DB_NAME, &db);
 *
 *   if (rc != SQLITE_OK) {
 *
 *     fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
 *     sqlite3_close(db);
 *
 *     return NULL;
 *   }
 *
 *   char *sql = "SELECT MyPrivateKey FROM MyInfo WHERE MyUsergid = @id";
 *
 *   rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
 *
 *   if (rc == SQLITE_OK) {
 *
 *     int idx = sqlite3_bind_parameter_index(res, "@id");
 *     sqlite3_bind_text(res, idx, usergid, strlen(usergid), SQLITE_STATIC);
 *
 *   } else {
 *
 *     fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
 *   }
 *
 *   int step = sqlite3_step(res);
 *
 *   if (step == SQLITE_ROW) {
 *     char * pk = malloc(sizeof(char)*(strlen((char *)sqlite3_column_text(res, 0)) + 1));
 *     strcpy(pk,(char *)sqlite3_column_text(res, 0));
 *     sqlite3_finalize(res);
 *     sqlite3_close(db);
 *     return pk;
 *   }
 *
 *   sqlite3_finalize(res);
 *   sqlite3_close(db);
 *   return NULL;
 * }
 */


/**
 * @brief XXX Add brief description!
 *
 * @param usergid
 *
 * @return
 */
User_t* getUser (const char* usergid)
{
    User_t* user = malloc(sizeof(User_t));

    sqlite3 *db;
    sqlite3_stmt *res;

    int rc = sqlite3_open(get_database_path(), &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    char *sql = "SELECT * FROM user WHERE user_gid = @id";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {

        int idx = sqlite3_bind_parameter_index(res, "@id");
        sqlite3_bind_text(res, idx, usergid, strlen(usergid), SQLITE_STATIC);

    } else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    int step = sqlite3_step(res);

    if (step == SQLITE_ROW)
    {
        strcpy(user->user_gid  , (char *)sqlite3_column_text(res, 0));
        strcpy(user->tcell_ip  , (char *)sqlite3_column_text(res, 1));
        strcpy(user->tcell_port, (char *)sqlite3_column_text(res, 2));
        strcpy(user->public_key, (char *)sqlite3_column_text(res, 3));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return user;
}
