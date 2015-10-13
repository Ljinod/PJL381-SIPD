/**
 * @file    configuration.h
 * @brief   Ask or create a configuration file.
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- updated comments)
 */

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "../beans/my_info.h"

#define XML_ENCODING  "UTF-8"
#define TCELL_IP      "tcell_ip"
#define TCELL_PORT    "tcell_port"
#define USER_ID       "id"
#define PRIV_KEY_PATH "priv_key_path"
#define PUB_KEY_PATH  "pub_key_path"

/**
 * @brief Ask the user for hers/his configuration file.
 *
 * @return MyInfo_t The structure that will hold the user's information or NULL
 *         on failure.
 */
MyInfo_t* ask_user_configuration_file(void);

#endif
