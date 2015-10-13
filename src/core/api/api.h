/**
 * @file    api.h
 * @brief   The api: the actions we can perform with the tcell.
 * @author  Loudet Julien    <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- added comments)
 */

#ifndef __API_H__
#define __API_H__

#include "../beans/my_info.h"

/**
 * @brief Store a file on the tcell.
 *
 * @param my_info The user's information as defined in the xml configuration
 *                file.
 * @param file_path The path of the file to store.
 */
void store_file(MyInfo_t *my_info, const char *file_path);


/**
 * @brief Asks the tcell the list of all the files stored.
 *
 * @param my_info The user's information as defined in the xml configuration
 *                file.
 */
void list_files(MyInfo_t *my_info);


/**
 * @brief Asks the tcell for a file.
 *
 * @param my_info The user's information as defined in the xml configuration
 *                file.
 * @param file_gid The file's unique identifier. One can get this id after a
 *                 call to the function list_file.
 */
void read_file(MyInfo_t *my_info, const char *file_gid);

#endif
