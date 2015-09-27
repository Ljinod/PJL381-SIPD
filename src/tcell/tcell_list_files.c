/**
 * @file    tcell_list_files.c
 * @brief   Ask the tcell to list the files.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <stdlib.h> /* malloc, free */
#include <unistd.h> /* close */

#include "../core/beans/my_info.h"
#include "../core/beans/file_desc.h"
#include "../database/database.h"
#include "../core/network/connection.h"
#include "../core/crypto/rsa_encryption.h"
#include "../core/message/message.h"
#include "tcell.h"


void tcell_list_files(MyInfo_t *my_info, int sock_fd)
{
    /* Ask the database for the number of files */
    int nb_files = getFileDescNumber();
    char str_nb_files[ARR_SIZE]; /* a tiny bit oversized */
    sprintf(str_nb_files, "%d", nb_files);

    /* Get the files descriptions */
    FileDesc_t* files_desc = getAllFileDesc();

    /* Send an ack to the client to tell him we are ready */
    send_msg_to(sock_fd, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* ack */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_fd));

    /* We first give him the number of files */
    send_msg_to(sock_fd, rsa_encrypt_msg(my_info->my_public_key,
                                         str_nb_files));

    /* ack */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_fd));

    /* We then transmit file by file */
    for(int i=0; i<nb_files; i++)
    {
        /* We create the message */
        char *msg = create_file_desc_message(&files_desc[i]);
        /* We encrypt it */
        char *enc_msg = rsa_encrypt_msg(my_info->my_public_key, msg);
        /* We send it */
        send_msg_to(sock_fd, enc_msg);

        /* Freeeeee */
        free(msg);
        free(enc_msg);

        /* We wait for an ack */
        rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_fd));
    }

    /* Everything is done, close connection! */
    free(files_desc);
    close(sock_fd);
}

