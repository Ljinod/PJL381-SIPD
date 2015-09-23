/**
 * @file    parse_config.h
 * @brief   XXX Add brief description!
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015/09)
 *
 *          Parses a configuration file. The configuration file, for now, has
 *          to follow a certain structure that is why it is STRONGLY advised to
 *          first generate a default configuration file. Then one can copy
 *          and/or modify it according to ones needs so that the structure
 *          stays intact.
 *
 *          The structure is as following:
 *
 *          <MY_INFO>
 *              <tcell_ip>127.0.0.1</tcell_ip>
 *              <tcell_port>5678</tcell_port>
 *              <id>julien</id>
 *              <priv_key_path>$HOME/.config/sipd/keys/priv_key</priv_key_path>
 *              <pub_key_path>$HOME/.config/sipd/keys/pub_key</pub_key_path>
 *          </MY_INFO>
 *
 *          NOTES: (*) The order of the parameters does not matter.
 *                 (*) The spelling and case of the parameters MUST be the
 *                     SAME as above. See the header file "configuration.h".
 *                 (*) There MUST be NO SPACES in the values you provide.
 *                 (*) Other parameters will not be processed. Thus you can add
 *                     whatever information you want (for instance other path
 *                     for other RSA keys), only the parameter above will be
 *                     taken into account.
 */
#ifndef __PARSE_CONFIG_H__
#define __PARSE_CONFIG_H__

#include "../beans/my_info.h"

MyInfo_t* parse_configuration_file(const char *config_file_path);

#endif
