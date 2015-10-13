/**
 * @file    file_desc.h
 * @brief   The file descriptor header.
 * @author  Loudet Julien    <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- added comments)
 *
 *           This header file contains all the fields that compose a file
 *           descriptor. The fields correspond to:
 *
 *           (*) file_gid: the global identifier of a file. It is generated
 *           when a file is first send to a tcell. It is, for now, the
 *           concatenation of the file name, the user's global identifier and
 *           the time (date + time) at which the file was sent.
 *
 *           (*) file_id: the original name of the file. It is extracted from
 *           the path of the file when first send to a tcell.
 *           For instance if the file path is: "/home/julien/random/toto.txt"
 *           then its file_id would be "toto.txt".
 *
 *           (*) size: the size of the file that is transfered. This field is
 *           used when transfering a file: the sender fills in this information
 *           then sends it before the file so that the receiver can check if
 *           the file was entirely transmitted. Usually this size corresponds
 *           to the size of the encrypted file.
 *
 *           (*) path: the path of the file.
 *
 *           (*) description: the description of the file. This field is given
 *           by the owner of the file when first sending it. It can be left
 *           blank.
 *
 *           (*) salt: NOT IN USE YET.
 *           This field would contain the salt used for the encryption. For now
 *           it is not used because we did not figured out how to decrypt a
 *           file that has been encrypted while using salt... (I didn't have
 *           enough time to take a close look at that, this should be simple
 *           to add).
 *
 *           (*) symmetric_key: the symmetric key used for the encryption.
 *
 *           (*) initialisation_vector: the initialisation vector used for the
 *           encryption.
 *
 *
 *           ==================================================================
 *           TODO
 *           ==================================================================
 *           - add the salt to the encryption
 *           - optimize the memory usage: allocating ARR bytes for each field
 *           could be an overkill...
 *
 */

#ifndef __FILE_DESC_H__
#define __FILE_DESC_H__

#include "../tools/utils.h"

typedef struct FileDesc
{
    char file_gid[ARR_SIZE];
    char file_id[ARR_SIZE];
    char size[ARR_SIZE];
    char path[ARR_SIZE];
    char type[ARR_SIZE];
    char description[ARR_SIZE];
    char salt[ARR_SIZE]; /* For future version! */
    char symmetric_key[ARR_SIZE];
    char initialisation_vector[ARR_SIZE];
} FileDesc_t;

#endif
