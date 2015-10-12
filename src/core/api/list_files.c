/**
 * @file    list_files.c
 * @brief   List all the files stored on the tcell.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited on 2015-10-13 by Loudet Julien
 *           -- added comments)
 *
 */

#include <string.h> /* strtok */
#include <stdlib.h> /* atoi, malloc, free */

#include "../beans/my_info.h"
#include "../network/connection.h"
#include "../message/message.h"
#include "../crypto/rsa_encryption.h"


/**
 * @brief Asks the tcell the list of all the files stored.
 *
 * @param my_info The user's information as defined in the xml configuration
 *                file.
 *
 * @details This function asks the tcell to give the list of all the files that
 *          were stored.
 *
 *          The protocole used is as following:
 *          (1) the function tries to connect to the tcell. If the connection
 *          attempt fails, it exits;
 *          (2) it sends a "list files" message to the tcell;
 *          (3) it waits for an acknowledge from the tcell;
 *          (4) it receives the number of file descriptions the tcell
 *          is going to send then sends an acknowledge;
 *          (5) it receives the file descriptions one by one and print
 *          them as they arrive - sending an ackowledge every time one was
 *          received and processed;
 *          (6) if everything went well it prints "Done!" and exits.
 */
void list_files(MyInfo_t *my_info)
{
    /* Establish a connection to the tcell */
    int tcell_sock = connect_to(my_info->my_tcell_ip, my_info->my_tcell_port);

    /* Create a list files message */
    char *list_files_msg = create_list_files_message();

    /* RSA encrypt it */
    char *enc_list_files_msg = rsa_encrypt_msg(my_info->my_public_key,
                                               list_files_msg);
    free(list_files_msg);

    /* Send it to the tcell */
    send_msg_to(tcell_sock, enc_list_files_msg);
    free(enc_list_files_msg);

    /* Wait for the ack to ensure the request was understood */
    rsa_assert_ack(my_info->my_private_key, recv_msg_from(tcell_sock));
    /* Send ACK: we are ready as well, let's go */
    send_msg_to(tcell_sock, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* We receive the number of files */
    char *enc_str_nb_files = recv_msg_from(tcell_sock);
    int nb_files = atoi(rsa_decrypt_msg(my_info->my_private_key,
                                        enc_str_nb_files));
    free(enc_str_nb_files);

    /* Send ACK */
    send_msg_to(tcell_sock, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));

    /* We receive the files descriptions */
    for(int i=0; i<nb_files; i++)
    {
        char *enc_file_desc = recv_msg_from(tcell_sock);
        char *file_desc_msg = rsa_decrypt_msg(my_info->my_private_key,
                                              enc_file_desc);
        free(enc_file_desc);

        /* Process the file description and display it */
        char *msg_part, *field, *value, *msg_end;
        msg_part = strtok_r(file_desc_msg, MSG_SEP, &msg_end);
        while(msg_part != NULL)
        {
            char *field_end;
            field = strtok_r(msg_part, FIELD_VALUE_SEP, &field_end);

            if(field == NULL)
            {
                fprintf(stderr, "[ERROR] An error occurred while decrypting "
                                "        the message. Unable to process it.\n"
                                );
                exit(-1);
            }

            /* The field is not null we cant get a value */
            value = strtok_r(NULL, FIELD_VALUE_SEP, &field_end);

            /* Display the values */
            if(strcmp(field, FIELD_FILE_GID) == 0)
            {
                fprintf(stdout, "[LIST FILES] File (GID): %s\n", value);
            }
            else if(strcmp(field, FIELD_FILE_ID) == 0)
            {
                fprintf(stdout, "[LIST FILES] \tname: %s\n", value);
            }
            else if(strcmp(field, FIELD_TYPE) == 0)
            {
                fprintf(stdout, "[LIST FILES] \ttype: %s\n", value);
            }
            else if(strcmp(field, FIELD_DESC) == 0)
            {
                fprintf(stdout, "[LIST FILES] \tdesc: %s\n", value);
            }

            msg_part = strtok_r(NULL, MSG_SEP, &msg_end);
        }

        /* Add newlines for more clarity */
        fprintf(stdout, "\n\n");

        /* Send ack to the tcell in order to receive the next descriptor */
        send_msg_to(tcell_sock, rsa_encrypt_msg(my_info->my_public_key, MSG_ACK));
    }

    /* That's it we're done! */
    fprintf(stdout, "[LIST FILES] Done.\n");
}
