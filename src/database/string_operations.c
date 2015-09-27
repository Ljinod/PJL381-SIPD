/**
 * @file    string_operations.c
 * @brief   Process the string so that they are safe to insert into a
 *          database.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <string.h>


#include "database.h"
#include "../core/beans/user.h"
#include "../core/beans/file_desc.h"


void escape_string_for_db(char *string, char chr_to_escape)
{
    char *wrong_chr;

    while((wrong_chr = strchr(string, chr_to_escape)) != NULL &&
          !(wrong_chr - string == strlen(string) &&
            chr_to_escape == '\0'))
    {
        wrong_chr[0] = '-';
    }
}


void safe_string_for_db(char *string)
{
    escape_string_for_db(string, '\\');
    escape_string_for_db(string, '\b');
    escape_string_for_db(string, '\n');
    escape_string_for_db(string, '\r');
    escape_string_for_db(string, '\t');
    escape_string_for_db(string, '\'');
    escape_string_for_db(string, '\0');
    escape_string_for_db(string, '%');
    escape_string_for_db(string, '\"');
}


void safe_file_desc_for_db(FileDesc_t *file_desc)
{
    safe_string_for_db(file_desc->file_gid);
    safe_string_for_db(file_desc->file_id);
    safe_string_for_db(file_desc->size);
    safe_string_for_db(file_desc->path);
    safe_string_for_db(file_desc->type);
    safe_string_for_db(file_desc->description);
    safe_string_for_db(file_desc->symmetric_key);
    safe_string_for_db(file_desc->initialisation_vector);
}


void safe_user_for_db(User_t* user)
{
    safe_string_for_db(user->user_gid);
    safe_string_for_db(user->tcell_ip);
    safe_string_for_db(user->tcell_port);
    safe_string_for_db(user->public_key);
}
