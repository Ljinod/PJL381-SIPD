/**
 * @file    tcell.h
 * @brief   The tcell's actions.
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 */

#ifndef __TCELL_H__
#define __TCELL_H__

#include "../core/beans/my_info.h"
#include "../core/tools/utils.h"

#define TCELL_FILES ((const char*) "sipd/tcell/files")


void tcell_store(MyInfo_t *my_info, int sock_fd, char *store_msg);


static inline char* get_tcell_files_path(void)
{
    char *home_path = getenv("HOME");
    char *tcell_files_path = malloc(sizeof(char) * (strlen(TCELL_FILES) +
                                    strlen(home_path) + 2));
    sprintf(tcell_files_path, "%s/%s", home_path, TCELL_FILES);

    return tcell_files_path;
}


#endif
