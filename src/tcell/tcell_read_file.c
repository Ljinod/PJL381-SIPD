/**
 * @file    tcell_read_file.c
 * @brief   Send a file to the client.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <string.h> /* strtok_r, strcpy */
#include <stdlib.h> /* free */

#include "../core/beans/my_info.h"
#include "../core/beans/file_desc.h"
#include "../database/database.h"
#include "../core/network/connection.h"
#include "../core/network/transfer_file.h"
#include "../core/crypto/rsa_encryption.h"
#include "../core/message/message.h"


void tcell_read_file(MyInfo_t *my_info, int sock_fd, char *read_msg)
{
    char *msg_part, *field, *value, *msg_end, file_gid[ARR_SIZE];

    /* Extract the file gid from the message */
    msg_part = strtok_r(read_msg, MSG_SEP, &msg_end);
    while(msg_part != NULL)
    {
        char *field_end;
        field = strtok_r(msg_part, FIELD_VALUE_SEP, &field_end);

        if(field == NULL)
        {
            fprintf(stderr, "[ERROR] An error occurred while decrypting "
                            "        the message. Unable to process it.\n");
            exit(-1);
        }

        /* The field is not null we cant get the file gid */
        value = strtok_r(NULL, FIELD_VALUE_SEP, &field_end);
        if(strcmp(field, FIELD_FILE_GID) == 0)
        {
            strcpy(file_gid, value);
        }

        msg_part = strtok_r(NULL, MSG_SEP, &msg_end);
    }

    /* Ask the database for the file desc */
    FileDesc_t *file_desc = getFileDesc(file_gid);

    /* We do have it, send ack to the client */
    send_msg_to(sock_fd, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* Wait for the go signal */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_fd));

    /* Send the client the name of the file, its size, the symmetric key
     * and initialisation vector used for its encryption */
    char *msg_recv_file = create_recv_file_message(file_desc);
    /* Encrypt the message */
    char *enc_msg_recv = rsa_encrypt_msg(my_info->my_public_key,
                                         msg_recv_file);
    free(msg_recv_file);
    /* Send the client the message */
    send_msg_to(sock_fd, enc_msg_recv);
    free(enc_msg_recv);

    /* Wait for the ack */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_fd));

    /* Send the file */
    send_file_to(sock_fd, file_desc);

}

