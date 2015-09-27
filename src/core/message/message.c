/**
 * @file    message.c
 * @brief   The messages controlling the tcell.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <string.h> /* strlen, strcat, strcpy */
#include <stdlib.h> /* malloc */
#include <stdio.h>  /* sprintf */

#include "message.h"
#include "../beans/file_desc.h"


char* create_store_message(const FileDesc_t* file_desc)
{
    /* Create the string with the appropriate size */
    char *msg = malloc(sizeof(char) *
                       (1 +
                        strlen(ACT_STORE) + strlen(MSG_SEP) +
                        /* file gobal id */
                        strlen(FIELD_FILE_GID)   + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->file_gid)      + strlen(MSG_SEP) +

                        /* file id */
                        strlen(FIELD_FILE_ID)    + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->file_id)       + strlen(MSG_SEP) +

                        /* file size, for checking purposes */
                        strlen(FIELD_SIZE)       + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->size)          + strlen(MSG_SEP) +

                        /* file type, the field can be blank */
                        strlen(FIELD_TYPE)       + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->type)          + strlen(MSG_SEP) +

                        /* file description, this field can be blank */
                        strlen(FIELD_DESC)       + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->description)   + strlen(MSG_SEP) +

                        /* symmetric key */
                        strlen(FIELD_SYM_KEY)    + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->symmetric_key) + strlen(MSG_SEP) +

                        /* initialisation vector */
                        strlen(FIELD_IV)       + strlen(FIELD_VALUE_SEP) +
                        strlen(file_desc->initialisation_vector)
                       )
                      );

    /* Now that the correct memory space was allocated, we create the message
     * starting with:
     * the action to perform */
    strcpy(msg, ACT_STORE); strcat(msg, MSG_SEP);

    /* followed by the description of the file to store */
    strcat(msg, FIELD_FILE_GID); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->file_gid); strcat(msg, MSG_SEP);

    strcat(msg, FIELD_FILE_ID); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->file_id); strcat(msg, MSG_SEP);

    strcat(msg, FIELD_SIZE); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->size); strcat(msg, MSG_SEP);

    strcat(msg, FIELD_TYPE); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->type); strcat(msg, MSG_SEP);

    strcat(msg, FIELD_DESC); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->description); strcat(msg, MSG_SEP);

    strcat(msg, FIELD_SYM_KEY); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->symmetric_key); strcat(msg, MSG_SEP);

    strcat(msg, FIELD_IV); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_desc->initialisation_vector);

    /* Message complete! */
    return msg;
}


char* create_read_message(const char* file_gid)
{
    char *msg = malloc(sizeof(char) *
                       (1 +
                        strlen(ACT_READ) + strlen(MSG_SEP) +
                        strlen(FIELD_FILE_GID) + strlen(FIELD_VALUE_SEP) +
                        strlen(file_gid)
                       )
                      );

    /* Now that the correct memory space was allocated, we create the message
     * starting with:
     * the action to perform */
    strcpy(msg, ACT_READ); strcat(msg, MSG_SEP);

    /* and finally the file gid */
    strcat(msg, FIELD_FILE_GID); strcat(msg, FIELD_VALUE_SEP);
    strcat(msg, file_gid);

    /* Message complete! */
    return msg;
}


char* create_list_files_message(void)
{
    char *msg = malloc(sizeof(char) *
                       (1 +
                        strlen(ACT_LIST_FILES)
                       )
                      );

    strcpy(msg, ACT_LIST_FILES);

    /* Message complete! */
    return msg;
}


char* create_share_message(void);
