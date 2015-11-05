/**
 * @file    create_config.h
 * @brief   Create a configuration file
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015-10-14
 *           -- updated comments)
 */


#ifndef __CREATE_CONFIG_H__
#define __CREATE_CONFIG_H__

#include "../beans/my_info.h"

/**
 * @brief Creates a configuration file for the user.
 *
 * @param config_file_path The path where the config file will be created.
 *
 * @return The structure holding the user's information or NULL if an error
 *         occurred.
 */
MyInfo_t* create_user_configuration_file(const char *config_file_path);

#endif
