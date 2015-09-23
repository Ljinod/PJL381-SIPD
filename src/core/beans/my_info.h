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

typedef struct MyInfo
{
    char my_gid[256];
    char my_tcell_ip[256];
    int  my_tcell_port;
    char my_public_key[256];
    char my_private_key[256];
} MyInfo_t;

#endif
