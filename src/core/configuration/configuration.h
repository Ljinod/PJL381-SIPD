/**
 * @file    configuration.h
 * @brief   XXX Add brief description!
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015/09)
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
 * @brief   Ask the user for hers/his configuration file.
 * @details The configuration file hold the information later used by the
 *          program. These information are:
 *          - the ip address of the user's tcell
 *          - the user's global identification
 *          - the port of the user's tcell
 *          - the location of the private key in the system
 *          - the location of the public key in the system
 *          First the program asks if it should load the default configuration
 *          file. If the users answers no (s)he can provide an alternative path
 *          for a custom configuration file.
 *          If no configuration file exists then the program creates one based
 *          on the answers given by the user. If the user does not provide a
 *          pair of public/private keys the program will create one.
 *
 *          On failure, my_info is set to NULL.
 *
 * @param user The structure that will hold the user's information.
 */
MyInfo_t* ask_user_configuration_file(void);

#endif
