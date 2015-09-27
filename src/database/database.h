/**
 * @file    database.h
 * @brief   Database functions.
 * @author  Loudet  Julien   <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015/09)
 */

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "../core/beans/file_desc.h"
#include "../core/beans/user.h"


#define TCELL_DB_PATH ((const char*) "sipd/tcell/database")
#define DB_NAME       ((const char*) "tcell.db")


static inline const char* get_database_path(void)
{
    char *home_path = getenv("HOME");
    char *database_path = malloc(sizeof(char) * (strlen(home_path) +
                            strlen(TCELL_DB_PATH) + strlen(DB_NAME) + 3));
    sprintf(database_path, "%s/%s/%s", home_path, TCELL_DB_PATH, DB_NAME);
    return database_path;
}

int create_database(void);


int insertFile(FileDesc_t* fdesc);


/* insert row in user */
int insertUser(User_t* user);


/* get row from user */
User_t* getUser (const char* usergid);


/* get row from fileDesc */
FileDesc_t* getFileDesc (const char* filegid);


int getFileDescNumber(void);


/**
 * @brief XXX Add brief description!
 *
 * @return
 */
FileDesc_t * getAllFileDesc(void);


User_t* getUser (const char* usergid);


void safe_string_for_db(char *string);

void safe_file_desc_for_db(FileDesc_t *file_desc);

void safe_user_for_db(User_t* user);

#endif
