/**
 * @file    aes_encryption.h
 * @brief   XXX Add brief description!
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#ifndef __AES_ENCRYPTION_H__
#define __AES_ENCRYPTION_H__

#include "../beans/file_desc.h"

#define TEMP_FOLDER  ((const char *) "sipd/temp")
#define FILES_FOLDER ((const char *) "sipd/files")

/**
 * @brief XXX Add brief description!
 *
 * @param file_path
 *
 * @return
 */
FileDesc_t* aes_encrypt_file(const char *file_path);


/**
 * @brief XXX Add brief description!
 *
 * @param enc_file_desc
 *
 * @return
 */
char* aes_decrypt_file(FileDesc_t *enc_file_desc);


#endif

