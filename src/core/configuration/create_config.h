/**
 * @file    create_config.h
 * @brief
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 */
#ifndef __CREATE_CONFIG_H__
#define __CREATE_CONFIG_H__

#include "../beans/my_info.h"

/**
 * @brief   Creates a configuration file for the user.
 * @details This function will ask the user the required information in order
 *          to use the SIPD (see ask_user_configuration_file for a detailed
 *          list of those information).
 *
 *          If the user cannot provide a public/private RSA pair of keys then
 *          a new pair is created and stored here: $HOME/.config/sipd/keys/
 *
 *          The configuration file is stored here: 
 *          $HOME/.config/sipd/my_info.xml
 *
 *          At the same time the MyInfo_t structure is filled with the
 *          information provided.
 *
 * @param config_file_path The path were the config file will be created.
 *
 * @return The structure holding the user's information.
 */
MyInfo_t* create_user_configuration_file(const char *config_file_path);

#endif
