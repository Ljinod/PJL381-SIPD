/**
 * @file    read_file.c
 * @brief   Ask the tcell for a file.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <stdlib.h> /* free */

#include "../beans/my_info.h"
#include "../network/connection.h"
#include "../message/message.h"
#include "../crypto/rsa_encryption.h"
#include "../crypto/aes_encryption.h"
#include "../network/transfer_file.h"
#include "api.h"


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

