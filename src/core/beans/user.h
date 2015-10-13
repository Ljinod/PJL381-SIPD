/**
 * @file    user.h
 * @brief   The information of another user.
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- added comments)
 *
 *           This header file contains all the information needed to be able to
 *           communicate with another user.
 *           The fields correspond to:
 *
 *           (*) user_gid: the global identifier of the user.
 *
 *           (*) tcell_ip: the ip address of the user's tcell. The expected
 *           format is: "192.168.1.1".
 *
 *           (*) tcell_port: the port on which the user's tcell is listening.
 *
 *           (*) public_key: the user's public key.
 *
 *
 *           NB: This file will be used by the tcell to share files. Since this
 *           function has not been implemented yet the structure of this header
 *           file might change.
 *
 *           ==================================================================
 *           TODO
 *           ==================================================================
 *           - determine wether the "public_key" field should represent the
 *           path of the public key or the public key itself. Since these
 *           information will be stored in the database both choice are valid.
 */


#ifndef __USER_H__
#define __USER_H__

#include "../tools/utils.h"

typedef struct User
{
    char user_gid[ARR_SIZE];
    char tcell_ip[ARR_SIZE];
    char tcell_port[ARR_SIZE];
    char public_key[ARR_SIZE];
} User_t;

#endif
