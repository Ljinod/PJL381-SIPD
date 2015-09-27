/**
 * @file    client.c
 * @brief   The client daemon.
 * @author  Loudet  Julien   <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien - 2015/09)
 *          The client daemon can be called through the command line through
 *          "sipdcd" - SIPD Client Daemon - and accepts the following
 *          arguments:
 *          TODO create the sipdcd
 *          TODO parse the arguments...!
 */

#include "../core/network/connection.h"
#include "../core/configuration/configuration.h"
#include "../core/beans/my_info.h"
#include "../core/api/api.h"

MyInfo_t *my_info;

int main(int argc, char **argv)
{
    /* First we determine who we are! */
    my_info = ask_user_configuration_file();

    /* Now we try to send a file to the tcell */
    store_file(my_info, "/home/julien/dev/OpenSSL/music.flac");

    return 0;
}
