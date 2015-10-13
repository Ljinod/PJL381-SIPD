/**
 * @file    store_file.c
 * @brief   Store a file.
 * @author  Loudet Julien    <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- added comments)
 */

#include <stdio.h> /* sprintf */
#include <string.h> /* strcmp */
#include <assert.h> /* assert */

#include "../network/connection.h"
#include "../network/transfer_file.h"
#include "../tools/utils.h"
#include "../message/message.h"
#include "../crypto/aes_encryption.h"
#include "../crypto/rsa_encryption.h"
#include "api.h"


/**
 * @brief Store a file on the tcell.
 *
 * @details The protocole used is as following:
 *          (1) The function tries to connect to the tcell, if the connection
 *          attempt fails it exits;
 *          (2) it encrypts the file to store;
 *          (3) it sends the tcell a "store message" which contains a
 *          description of the file;
 *          (4) it waits for an acknowledge from the tcell, signalling that the
 *          tcell receives the file information correctly and that it does not
 *          already have an entry in the database for that particular file;
 *          (5) it transfers the file;
 *          (6) it waits for a final acknowledge from the tcell to ensure that
 *          the transfer ended correctly.
 *
 * @param my_info The user's information as defined in the xml configuration
 *                file.
 * @param file_path The path of the file to store.
 */
void store_file(MyInfo_t *my_info, const char *file_path)
{
    /* Establish a connection to the tcell: we do it now because if we can't
     * connect to it then there is no point in doing the rest...
     * And we don't even need to process the returned value since the function
     * connect_to does this for us */
    int sock_tcell = connect_to(my_info->my_tcell_ip, my_info->my_tcell_port);

    /* Encrypt the file to send using AES alogithm: the aes_encrypt_file
     * function creates the FileDesc_t structure and fills some of the fields;
     * namely: file_id, size, path, symmetric_key, initialisation_vector */
    FileDesc_t* file_desc = aes_encrypt_file(file_path);

    /* Fill in the file_gid: it is a combination of the user's gid and the
     * file id */
    sprintf(file_desc->file_gid, "%s-%s", my_info->my_gid, file_desc->file_id);

    /* Ask the user for the rest of the fields:
     * the type */
    char buffer[ARR_SIZE], disp_msg[ARR_SIZE];
    sprintf(disp_msg, "[STORE] Would you like to provide a type for < %s >?\n"
                      "        This field can be left empty (max %i "
                      "characters).\n"
                      "[STORE] > ", file_desc->file_id, ARR_SIZE);
    get_user_input(buffer, ARR_SIZE, disp_msg);
    strcpy(file_desc->type, buffer);

    /* the description */
    memset(disp_msg, '\0', ARR_SIZE); /* reset just in case */
    sprintf(disp_msg, "[STORE] Would you like to provide a description for "
                      " < %s >?\n"
                      "        This field can be left empty (max %i characters"
                      ").\n"
                      "[STORE] > ", file_desc->file_id, ARR_SIZE);
    get_user_input(buffer, ARR_SIZE, disp_msg);
    strcpy(file_desc->description, buffer);

    /* Create the store message */
    char *store_msg = create_store_message(file_desc);
    /* Encrypt it with user's rsa public key */
    char *enc_store_msg = rsa_encrypt_msg(my_info->my_public_key, store_msg);
    /* Send the store message to the tcell */
    send_msg_to(sock_tcell, enc_store_msg);
    /* Wait for the ack */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_tcell));
    /* Free memory */
    free(store_msg);
    free(enc_store_msg);

    /* Transfer the file */
    send_file_to(sock_tcell, file_desc);
    /* Wait for the ack */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(sock_tcell));

    fprintf(stdout, "[STORE] File < %s > transfered successfully.\n",
                   file_desc->file_id);
}
