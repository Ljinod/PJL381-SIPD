// ============================================================================
//
//       Filename:  user.h
//         Author:  Loudet  Julien
//          Email:  julien.loudet@telecom-paristech.fr
//                  loudet.julien@gmail.com
//
//    Last edited:  2015/09 - Loudet Julien
//    Description:  File creation
//
//
// ============================================================================

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
