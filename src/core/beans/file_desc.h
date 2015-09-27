// ============================================================================
//
//       Filename:  file_desc.h
//         Author:  Loudet  Julien
//          Email:  julien.loudet@telecom-paristech.fr
//                  loudet.julien@gmail.com
//
//    Last edited:  2015/09 - Loudet Julien
//    Description:  File creation
//
//
// ============================================================================

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
