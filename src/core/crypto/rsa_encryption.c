/**
 * @file    rsa_encryption.c
 * @brief   XXX Add brief description!
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#include <string.h> /* strcspn */
#include <stdio.h> /* sprintf, fprintf */

#include "rsa_encryption.h"
#include "../tools/utils.h"


/**
 * @brief XXX Add brief description!
 *
 * @param msg
 * @param pub_key_path
 *
 * @return
 */
const char* rsa_encrypt_msg(const char *pub_key_path, const char *msg)
{
    char  cmd[CMD_SIZE];
    char *enc_msg = malloc(RSA_SIZE * sizeof(char));

    /* XXX Explain command! */
    sprintf(cmd, "echo \"%s\" | openssl rsautl -encrypt -pubin "
                 "-inkey %s | openssl enc -base64 -A", msg, pub_key_path);

    /* Execute command and get the output */
    FILE *enc_msg_stream = popen((const char*) cmd, "r");

    /* Since we asked to convert the output in base64 on one line we only
     * need to do fgets once */
    if(fgets(enc_msg, RSA_SIZE, enc_msg_stream) != NULL)
    {
        /* Remove newline character */
        enc_msg[strcspn(enc_msg, "\n")] = '\0';
    }

    /* We check when closing the stream that the command executed correctly */
    if(pclose(enc_msg_stream) != 0)
    {
        fprintf(stderr, "[ERROR] Could not encrypt the message using the "
                        "public rsa key: %s\n[ERROR] Exiting\n", pub_key_path);
        exit(-1);
    }

    /* We return the encrypted message */
    return enc_msg;
}


/**
 * @brief XXX Add brief description!
 *
 * @param enc_msg
 * @param priv_key_path
 *
 * @return
 */
const char* rsa_decrypt_msg(const char *priv_key_path, const char *enc_msg)
{
    char  cmd[CMD_SIZE];
    char *dec_msg = malloc(RSA_SIZE * sizeof(char));

    /* XXX Explain command! */
    sprintf(cmd, "echo \"%s\" | openssl enc -base64 -A -d | "
                 "openssl rsautl -decrypt -inkey %s", enc_msg, priv_key_path);

    /* We get the decrypted message from the output of the command */
    FILE *dec_msg_stream = popen((const char *)cmd, "r");

    /* Once again through the -A option the output is only on one line */
    if(fgets(dec_msg, RSA_SIZE, dec_msg_stream) != NULL)
    {
        /* Remove newline character */
        dec_msg[strcspn(dec_msg, "\n")] = '\0';
    }

    if(pclose(dec_msg_stream) != 0)
    {
        fprintf(stderr, "[ERROR] Could not decrypt the message using the "
                        "private rsa key: %s\n[ERROR] Exiting\n",
                        priv_key_path);
        exit(-1);
    }

    /* Decrypted message! */
    return dec_msg;
}

