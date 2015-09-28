/**
 * @file    api.h
 * @brief   The api: the actions we can perform with the tcell.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#ifndef __API_H__
#define __API_H__

#include "../beans/my_info.h"

void store_file(MyInfo_t *my_info, const char *file_path);

void list_files(MyInfo_t *my_info);

void read_file(MyInfo_t *my_info, const char *file_gid);

#endif
