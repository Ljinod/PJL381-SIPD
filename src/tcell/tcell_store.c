/**
 * @file    tcell_store.c
 * @brief   Store a file on the tcell.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <time.h> /* localtime, struct tm, time */
#include <stdio.h> /* fprintf */
#define _GNU_SOURCE /* strtok_r */
#include <string.h> /* strtok_r, strcpy, strcat */

#include "../core/beans/my_info.h"
#include "../core/message/message.h"
#include "../core/crypto/rsa_encryption.h"
#include "../core/network/connection.h"
#include "../core/network/transfer_file.h"
#include "../database/database.h"
#include "tcell.h"

void tcell_store(MyInfo_t *my_info, int sock_fd, char *store_msg)
{
    FileDesc_t file_desc;
    char *msg_part, *field, *value, *msg_end;

    msg_part = strtok_r(store_msg, MSG_SEP, &msg_end);

    /* Get the file description from the store message */
    while(msg_part != NULL)
    {
        char *field_end;
        field = strtok_r(msg_part, FIELD_VALUE_SEP, &field_end);

        if(field == NULL)
        {
            fprintf(stderr, "[ERROR] An error occurred while decrypting the "
                    "        message. Unable to process it.\n");
            exit(-1);
        }

        /* The field is not null we cant get a value */
        value = strtok_r(NULL, FIELD_VALUE_SEP, &field_end);

        if(value == NULL)
        {
            fprintf(stderr, "[WARNING] The value of the field < %s > \n"
                            "          was not provided.\n", field);
        }

        /* Fill in the correct field of the file descriptor */
        if(strcmp(field, FIELD_FILE_GID) == 0)
        {
            strcpy(file_desc.file_gid, value);
        }
        else if(strcmp(field, FIELD_FILE_ID) == 0)
        {
            strcpy(file_desc.file_id, value);
        }
        else if(strcmp(field, FIELD_SIZE) == 0)
        {
            strcpy(file_desc.size, value);
        }
        else if(strcmp(field, FIELD_TYPE) == 0)
        {
            strcpy(file_desc.type, value);
        }
        else if(strcmp(field, FIELD_DESC) == 0)
        {
            strcpy(file_desc.description, value);
        }
        else if(strcmp(field, FIELD_SYM_KEY) == 0)
        {
            strcpy(file_desc.symmetric_key, value);
        }
        else if(strcmp(field, FIELD_IV) == 0)
        {
            strcpy(file_desc.initialisation_vector, value);
        }


        msg_part = strtok_r(NULL, MSG_SEP, &msg_end);
    }


    /* Update the file gid to add the time */
    char strtime[ARR_SIZE];
    time_t t = time(NULL);
    struct tm s_tm = *localtime(&t);
    sprintf(strtime, "%d%d%d%d%d%d", s_tm.tm_year + 1900, s_tm.tm_mon + 1,
                                     s_tm.tm_mday, s_tm.tm_hour, s_tm.tm_min,
                                     s_tm.tm_sec);

    strcat(file_desc.file_gid, "-");
    strcat(file_desc.file_gid, strtime);

    /* Set the path of the file on the tcell */
    char *tcell_files_path = get_tcell_files_path();
    char buf_path[ARR_SIZE];
    sprintf(buf_path, "%s/%s", tcell_files_path, file_desc.file_gid);
    strcpy(file_desc.path, buf_path);
    free(tcell_files_path);

    /* Insert a new entry in the database */
    if(insertFile(&file_desc) != 1)
    {
        fprintf(stderr, "[ERROR] Could not insert file into the database.\n");
        exit(-1);
    }

    /* Everyting went well, send an ack to start the file transfer */
    send_msg_to(sock_fd, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* File transfer! */
    recv_file_from(sock_fd, file_desc.size, file_desc.path);

    /* Send an ack to end the transaction */
    send_msg_to(sock_fd, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    fprintf(stdout, "[INFO] File successfully transfered!\n");
}

