/**
 * @file    my_info.h
 * @brief   The user's information
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- added comments)
 *
 *          This header contains all the fields the program - be it the client
 *          or the tcell - needs to know about its owner.
 *          The fields correspond to:
 *
 *          (*) my_gid: the unique global identifier of the owner. For now it
 *          corresponds to the name the owner gives when first running the
 *          program.
 *
 *          (*) my_tcell_ip: the ip address of the tcell. The expected format
 *          is: "192.168.0.1".
 *
 *          (*) my_tcell_port: the port the tcell is listening on.
 *
 *          (*) my_public_key: the path where the public key is stored.
 *
 *          (*) my_private_key: the path where the private key is stored.
 *
 *
 *          IMPORTANT INFORMATION
 *          -------------------------------------------------------------------
 *          (*) The user's information are stored in a xml file that is
 *          generated when running for the first time the client daemon and
 *          the tcell daemon. The information to both the client and the tcell
 *          MUST be IDENTICAL otherwise the two will not be able to communicate
 *          as intended.
 *          IDENTICAL means here: the tcell ip and port must be the same, the
 *          global id must be the same, the paths must point to the same files
 *          (same in the sense of: same content - the name and path can be
 *          different for as long as the content is strictly identical).
 *          To achieve what is explained above one could copy the file produced
 *          after a first run of one of the two ends, tcell or client.
 *
 *          (*) Both the client and the tcell look for the default
 *          configuration file in one specific place (see configuration.c). It
 *          is possible to provide an alternative path but it is advised to
 *          use the default one. As the process of getting the user's
 *          information could be automated in future versions.
 *
 *
 *          ===================================================================
 *          TODO
 *          ===================================================================
 *          - optimise memory usage, allocating ARR bytes for each field could
 *          be a potential overkill...
 */

#ifndef __MY_INFO_H__
#define __MY_INFO_H__

#include "../tools/utils.h"

typedef struct MyInfo
{
    char my_gid[ARR_SIZE];
    char my_tcell_ip[ARR_SIZE];
    char my_tcell_port[ARR_SIZE];
    char my_public_key[ARR_SIZE];
    char my_private_key[ARR_SIZE];
} MyInfo_t;

#endif
