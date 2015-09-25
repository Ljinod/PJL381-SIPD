/*
 * @file    aes_encryption.c
 * @brief   XXX Add brief description!
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 *
 * @details
 *
 *
 *          ===================================================================
 *          IDEAS
 *          ===================================================================
 *          (*) maybe the aes_encrypt_file function needs a file descriptor
 *          instead of a path (hence changing its prototype to
 *          void aes_encrypt_file(FileDesc_t *file_desc);
 */

#include "aes_encryption.h"
#include "../beans/file_desc.h"
#include "../tools/utils.h"

#include <stdio.h>  /* popen, sprintf, fprintf */
#include <string.h> /* strcspn, strtok, strcpy, strncmp */
#include <unistd.h> /* access */
#include <stdlib.h> /* exit */



/**
 * @brief
 *
 * @param file_path
 *
 * @return
 */
FileDesc_t* aes_encrypt_file(const char *file_path)
{
    /* Check if file exists and we have read permission */
    if(access(file_path, R_OK) != 0)
    {
        fprintf(stderr, "File doesn't exist OR you do not have READ right.\n");
        exit(-1);
    }

    /* The file descriptor that we will send to the tcell */
    FileDesc_t *file_desc = malloc(sizeof(FileDesc_t));

    /* char arrays to store the command and the path of the encrypted file */
    char  file_enc_path[ARR_SIZE], cmd[ARR_SIZE], key_iv[ARR_SIZE];
    FILE *file_key_iv;

    /* The file id is its path on the system.
     * When we encrypt a file we put it in the $HOME/sipd/temp folder. */
    sprintf(file_enc_path, "%s.enc", file_path);

    /* XXX Describe the openssl command! */
    sprintf(cmd, "openssl enc -md sha1 -nosalt -aes-256-cbc -a -in %s "
                 "-out %s -p", file_path, file_enc_path);

    /* The result of popen is a file stream containing the output of the
     * command it executed. Since we used the "-p" option, the symmetric key
     * and initilisation vector are printed after the command returns. Hence
     * the file stream contains the key and iv. */
    fprintf(stdout, "[INFO] Encrypting file using AES-256.\n");
    fprintf(stdout, "\n"
                    "[[ /!\\ ]] You will be asked to enter a [[ /!\\ ]]\n"
                    "[[ /!\\ ]] password. It MUST be of at   [[ /!\\ ]]\n"
                    "[[ /!\\ ]] at least 1 character!        [[ /!\\ ]]\n"
                    "[[ /!\\ ]] This password will NOT be    [[ /!\\ ]]\n"
                    "[[ /!\\ ]] needed when decrypting. It is[[ /!\\ ]]\n"
                    "[[ /!\\ ]] used to generate the key     [[ /!\\ ]]\n"
                    "[[ /!\\ ]] for the encryption.          [[ /!\\ ]]\n"
                    "\n");
    file_key_iv = popen((const char*) cmd, "r");

    /* Process the command output line by line */
    while(fgets(key_iv, ARR_SIZE, file_key_iv) != NULL)
    {
        /* Strip the newline that fgets keeps */
        key_iv[strcspn(key_iv, "\n")] = '\0';

        char *tok;
        /* Separate label from its value */
        tok = strtok(key_iv, "=");
        /* Process the value depending on the label */
        if(strncmp(tok, "key", 3) == 0)
        {
            tok = strtok(NULL, "\0");
            strcpy(file_desc->symmetric_key, tok);
        }
        else if(strncmp(tok, "iv", 2) == 0)
        {
            tok = strtok(NULL, "\0");
            strcpy(file_desc->initialisation_vector, tok);
        }

        /* Wipe previous content entirely */
        memset(key_iv, '\0', ARR_SIZE);
    }

    /* If, when we close the file stream opened by popen, the returned value
     * is not 0 we can be sure that something wrong happened. */
    if(pclose(file_key_iv) != 0)
    {
        print_error_info("[ERROR] Encryption failed.\n");
        exit(-1);
    }

    /* If we've reached this part of the code then everything went fine! */
    fprintf(stdout, "\n[INFO] Encryption succeeded.\n");

    return file_desc;
}


/**
 * @brief
 *
 * @param enc_file_path
 *
 * @return
 */
const char* aes_decrypt_file(FileDesc_t* enc_file_desc)
{
    /* sprintf(buffer, "openssl enc -aes-256-cbc -md sha1 -d -a -in music.enc "
     *                 "-out music-dec.flac "
     *                 "-K symmetric_key "
     *                 "-iv initialisation_vector"); */
    return NULL;
}

