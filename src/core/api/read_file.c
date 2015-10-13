/**
 * @file    read_file.c
 * @brief   Ask the tcell for a file.
 * @author  Loudet Julien    <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited on 2015-10-13 by Loudet Julien
 *           -- added comments)
 */

#include <stdlib.h> /* free */

#include "../beans/my_info.h"
#include "../network/connection.h"
#include "../message/message.h"
#include "../crypto/rsa_encryption.h"
#include "../crypto/aes_encryption.h"
#include "../network/transfer_file.h"
#include "api.h"


/**
 * @brief Asks the tcell for a file.
 *
 * @details The protocole used is a following:
 *          (1) the function tries to connect to the tcell, if the connection
 *          attempt fails it exits;
 *          (2) it sends a "read message" to the tcell;
 *          (3) it waits for an acknowledge from the tcell signaling
 *          that the request was understood - i.e. the file gid provided is
 *          correct;
 *          (4) it asks the tcell to start the transfer;
 *          (5) it receives the file description containing all the
 *          information needed to decrypt the file;
 *          (6) it sends an ackowledge to the tcell which triggers the
 *          transfer of the file;
 *          (7) it stores the file on a temporary location for it to be
 *          decrypted;
 *          (8) it decrypts the file and stores the result in the
 *          folder containing the files received from the tcell;
 *          (9) if everything went well a message is printed giving the path of
 *          the decrypted file.
 *
 *          NB: the (4) part of the protocole is necessary because of the way
 *          the sockets work: the size of the message to receive must be
 *          specified and, so that we receive it entirely, usually the size
 *          given to the "recv" function is greater than the actual size of the
 *          message.
 *          This has a consequence: the "recv" blocks until it's received size
 *          bytes; so if we want to not mix the messages we must use one trick:
 *          once the message was sent the sender must then wait for a message.
 *          This forces the exit of the blocking state of the receiver because
 *          both ends of a socket cannot be receiving at the same time.
 *
 *
 * @param my_info The user's information as defined in the xml configuration
 *                file.
 * @param file_gid The file's unique identifier. One can get this id after a
 *                 call to the function list_file.
 */
void read_file(MyInfo_t *my_info, const char *file_gid)
{
    /* Connect to the tcell */
    int tcell_sock = connect_to(my_info->my_tcell_ip, my_info->my_tcell_port);

    /* Create the read message */
    char *read_msg = create_read_message(file_gid);

    /* Encrypt it */
    char *enc_msg = rsa_encrypt_msg(my_info->my_public_key, read_msg);
    free(read_msg);

    /* Send it to the tcell */
    send_msg_to(tcell_sock, enc_msg);
    free(enc_msg);

    /* Wait for the ack */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(tcell_sock));

    /* Send the go signal to launch the transfer  */
    send_msg_to(tcell_sock, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* First we wait for the file information */
    char *enc_recv_msg = recv_msg_from(tcell_sock);
    /* We decrypt the information */
    char *recv_file_msg = rsa_decrypt_msg(my_info->my_private_key,
                                          enc_recv_msg);
    free(enc_recv_msg);

    /* We create a temporary file desc to hold the information and, later,
     * decrypt the file */
    FileDesc_t file_desc;

    /* Process the message to extract the information */
    char *msg_part, *field, *value, *msg_end;
    msg_part = strtok_r(recv_file_msg, MSG_SEP, &msg_end);
    while(msg_part != NULL)
    {
        char *field_end;
        field = strtok_r(msg_part, FIELD_VALUE_SEP, &field_end);

        if(field == NULL)
        {
            fprintf(stderr, "[ERROR] An error occurred while decrypting\n"
                            "        the message. Unable to process it.\n");
            exit(-1);
        }

        /* The field is not null we cant get a value */
        value = strtok_r(NULL, FIELD_VALUE_SEP, &field_end);

        /* Extract the values */
        if(strcmp(field, FIELD_FILE_ID) == 0)
        {
            strcpy(file_desc.file_id, value);
        }
        else if(strcmp(field, FIELD_SIZE) == 0)
        {
            strcpy(file_desc.size, value);
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

    /* We affect a temp path to the encrypted file */
    char buf_path[ARR_SIZE];
    sprintf(buf_path, "%s/%s/%s.enc", getenv("HOME"), TEMP_FOLDER,
                                      file_desc.file_id);
    strcpy(file_desc.path, buf_path);

    /* We send an ack to the tcell, let's transfer */
    send_msg_to(tcell_sock, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* We receive the file */
    recv_file_from(tcell_sock, file_desc.size, file_desc.path);

    /* We decrypt it */
    char *dec_file_path = aes_decrypt_file(&file_desc);

    /* That's it !*/
    fprintf(stdout, "[READ] File received: %s\n", dec_file_path);
    free(dec_file_path);
}

