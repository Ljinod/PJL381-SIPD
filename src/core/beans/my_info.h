// ============================================================================
//
//       Filename:  my_info.h
//         Author:  Loudet  Julien
//          Email:  julien.loudet@telecom-paristech.fr
//                  loudet.julien@gmail.com
//
//    Last edited:  2015/09 - Loudet Julien
//    Description:  File creation
//
//
// ============================================================================

#ifndef __MY_INFO_H__
#define __MY_INFO_H__

#include "../tools/utils.h"

typedef struct MyInfo
{
    char my_gid[ARR_SIZE];
    char my_tcell_ip[ARR_SIZE];
    char my_tcell_port[ARR_SIZE];
    char my_public_key[ARR_SIZE];
    char my_private_key[ARR_SIZE];
} MyInfo_t;

#endif
