/**
 * @file    store_file.c
 * @brief   Store a file.
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-04
 *
 * @details (Last edited by Loudet Julien on 2015/09)
 */

#include <stdio.h> /* sprintf */
#include <string.h> /* strcmp */
#include <assert.h> /* assert */

#include "../network/connection.h"
#include "../tools/utils.h"
#include "../message/message.h"


/* ==================== LEGACY CODE ===========================================
 * ============================================================================
 * char* storeFileOnTCell(const char *file_path, const void *file_to_store,
 *                        int file_size)
 * {
 *   char *file_name         = basename((char *)file_path);
 *   char *filename_on_tcell = malloc(sizeof(char) * (strlen(TCELL_FILES_DIR) +
 *                                                    strlen(file_name) + 1));
 *   // We create the name
 *   strcpy(filename_on_tcell, TCELL_FILES_DIR);
 *   strcat(filename_on_tcell, file_name);
 *
 *   // We only store the file if it is not already on the TCell: we do not handle
 *   // updates in the database just yet.
 *   if(access(filename_on_tcell, F_OK) == -1)
 *   {
 *     // We create the file
 *     FILE *file_tcell = fopen(filename_on_tcell, "wb");
 *     if(file_tcell == NULL)
 *     {
 *       print_error_info("ERROR: daemon.c - could not create file on TCell.");
 *     }
 *
 *     // We write the content we just received
 *     int res = fwrite(file_to_store, 1, file_size, file_tcell);
 *     if(res != file_size)
 *     {
 *       print_error("ERROR: daemon.c - could not write file on TCell.");
 *       exit(EXIT_FAILURE);
 *     }
 *
 *     // We close the file
 *     if(fclose(file_tcell) != 0)
 *     {
 *       print_error_info("ERROR: daemon.c - could not close file on TCell.");
 *       fclose(file_tcell);
 *     }
 *   }
 *
 *   return filename_on_tcell;
 * }
 * ============================================================================
 * ============================================================================
 *  else if(strcmp(action, MSG_STORE) == 0)
 *  {
 *    fprintf(stdout, "INFO: STORE FILE DESCRIPTION action recieve\n");
 *    char *file_name = strtok(NULL, MSG_SEPARATOR);
 *    // Send first OK to client to get the symmetric key
 *    sendSock(sock, MSG_OK, PUB_KEY);
 *    // Receive symmetric key
 *    char *sym_key = recvSock(sock, PRIV_KEY);
 *
 *    FileDesc_t *file_desc = createFileDesc(file_name, file_name,
 *                                           sym_key, "", "");
 *    if(insertFile(file_desc) == 0)
 *    {
 *      print_error("ERROR: could not insert into database.");
 *      sendSock(sock, MSG_ERROR, PUB_KEY);
 *    }
 *    else
 *    {
 *      // Send OK to client: database successfully updated
 *      sendSock(sock, MSG_OK, PUB_KEY);
 *    }
 *
 *    // Free resources
 *    free(file_desc);
 *    free(sym_key);
 *  }
 * ============================================================================
 * ============================================================================
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
    fprintf(stdout, "[STORE] Encrypting file...");
    FileDesc_t* file_desc = aes_encrypt_file(file_path);
    fprintf(stdout, "Ok.\n");

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
    const char *store_msg = create_store_message(file_desc);
    /* Encrypt it with user's rsa public key */
    const char *enc_store_msg = rsa_encrypt_msg(store_msg,
                                                my_info->my_public_key);
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
