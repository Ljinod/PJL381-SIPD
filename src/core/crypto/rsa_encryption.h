/**
 * @file    rsa_encryption.h
 * @brief   XXX Add brief description!
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#ifndef __RSA_ENCRYPTION_H__
#define __RSA_ENCRYPTION_H__

#define RSA_SIZE (1024)

/**
 * @brief XXX Add brief description!
 *
 * @param msg
 * @param pub_key_path
 *
 * @return
 */
const char* rsa_encrypt_msg(const char *msg, const char *pub_key_path);


/**
 * @brief XXX Add brief description!
 *
 * @param enc_msg
 * @param priv_key_path
 *
 * @return
 */
const char* rsa_decrypt_msg(const char *enc_msg, const char *priv_key_path);

#endif
