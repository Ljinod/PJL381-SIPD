/*
 * @file    aes_encryption.c
 * @brief   XXX Add brief description!
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 *
 * @details
 *
 *          ===================================================================
 *          TO DO
 *          ===================================================================
 *          (*) Understand why when I try to decrypt a file that was encrypted
 *          using salt, the resulting file isn't the same...
 *
 *          ===================================================================
 *          IDEAS
 *          ===================================================================
 *          (*) maybe the aes_encrypt_file function needs a file descriptor
 *          instead of a path (hence changing its prototype to
 *          void aes_encrypt_file(FileDesc_t *enc_file_desc);
 */

#include "aes_encryption.h"
#include "../beans/file_desc.h"
#include "../tools/utils.h"

/* The following block is solely for the function basename: the version of this
 * function found in <libgen.h> may cause segfault...and may modify its
 * argument. Hence when possible we'll use the GNU version! */
#include <features.h>
#ifdef _GNU_SOURCE
#define _GNU_SOURCE
#else
#include <libgen.h>
#endif

#include <stdio.h>  /* popen, sprintf, fprintf, remove */
#include <string.h> /* strcspn, strtok, strcpy, strncmp, basename */
#include <unistd.h> /* access */
#include <stdlib.h> /* exit, getenv */



/**
 * @brief XXX Add brief description!
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
    FileDesc_t *enc_file_desc = malloc(sizeof(FileDesc_t));

    /* Extract the basename from the path.
     * We use a copy of the file_path because it is said in the man pages that
     * basename could modify the argument passed to it. */
    char file_path_copy[ARR_SIZE];
    strcpy(file_path_copy, file_path);
    strcpy(enc_file_desc->file_id, (basename(file_path_copy)));

    /* char arrays to store the command and the path of the encrypted file */
    char  file_enc_path[ARR_SIZE], cmd[CMD_SIZE], key_iv[ARR_SIZE];
    FILE *file_key_iv;

    /* When we encrypt a file we put it in the $HOME/sipd/temp folder. */
    sprintf(file_enc_path, "%s/%s/%s.enc", getenv("HOME") , TEMP_FOLDER,
                                           enc_file_desc->file_id);
    strcpy(enc_file_desc->path, file_enc_path);

    /* XXX Describe the openssl command! */
    sprintf(cmd, "openssl enc -aes-256-cbc -a -md sha1 -nosalt "
                 "-in %s -out %s -p", file_path, file_enc_path);

    /* The result of popen is a file stream containing the output of the
     * command it executed. Since we used the "-p" option, the symmetric key
     * and initilisation vector are printed after the command returns. Hence
     * the file stream contains the key and iv. */
    fprintf(stdout, "[INFO] Encrypting file - %s - using AES-256.\n",
            file_path);
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

        fprintf(stdout, "FGETS: %s\n", key_iv);

        char *tok;
        /* Separate label from its value */
        tok = strtok(key_iv, "=");
        /* Process the value depending on the label */
        if(strncmp(tok, "key", 3) == 0)
        {
            tok = strtok(NULL, "\0");
            strcpy(enc_file_desc->symmetric_key, tok);
            fprintf(stdout, "       KEY=%s\n", tok);
        }
        else if(strncmp(tok, "iv", 2) == 0)
        {
            tok = strtok(NULL, "\0");
            strcpy(enc_file_desc->initialisation_vector, tok);
            fprintf(stdout, "       IV=%s\n", tok);
        }
        else if(strncmp(tok, "salt", 4) == 0)
        {
            tok = strtok(NULL, "\0");
            strcpy(enc_file_desc->salt, tok);
            fprintf(stdout, "       SALT=%s\n", tok);
        }

        /* Wipe previous content entirely */
        memset(key_iv, '\0', ARR_SIZE);
    }

    /* If, when we close the file stream opened by popen, the returned value
     * is not 0 we can be sure that something wrong happened. */
    if(pclose(file_key_iv) != 0)
    {
        print_error_info("\n[ERROR] Encryption failed.\n");
        exit(-1);
    }

    /* If we've reached this part of the code then everything went fine! */
    fprintf(stdout, "\n[INFO] Encryption succeeded.\n");

    return enc_file_desc;
}


/**
 * @brief XXX Add brief description!
 *
 * @param enc_file_path
 *
 * @return
 */
const char* aes_decrypt_file(FileDesc_t* enc_file_desc)
{
    char cmd[CMD_SIZE], *dec_file_path = malloc(ARR_SIZE * sizeof(char));

    /* We compute the path for the decrypted file */
    sprintf(dec_file_path, "%s/%s/%s", getenv("HOME"), FILES_FOLDER,
                                       enc_file_desc->file_id);

    /* We then compute the command to issue */
    sprintf(cmd,"openssl enc -aes-256-cbc -d -a -md sha1 -in %s -out %s "
                "-K %s -iv %s", enc_file_desc->path, dec_file_path,
                enc_file_desc->symmetric_key,
                enc_file_desc->initialisation_vector);

    fprintf(stdout, "\n[INFO] Decrypting file: %s\n", enc_file_desc->file_id);

    /* XXX This behavior should be double-checked! I only did tests....*/
    if(system(cmd) != 0)
    {
        fprintf(stderr, "[ERROR] Something went wrong when trying to decrypt: "
                        "%s\n", enc_file_desc->file_id);

        /* Remove empty file */
        if(remove(dec_file_path) == 0)
        {
            fprintf(stderr, "[ERROR] Empty file was removed.\n");
        }
        exit(-1);
    }

    fprintf(stdout, "[INFO] Decryption succeeded.\n");

    return dec_file_path;
}

