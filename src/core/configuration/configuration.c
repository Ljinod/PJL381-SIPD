/**
 * @file    configuration.c
 * @brief   XXX Add brief description!
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015/09)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /**< For the "access" function */

#include "configuration.h"
#include "create_config.h" /**< "create_user_configuration_file" function" */
#include "parse_config.h" /**< "parse_configuration_file" function */
#include "../tools/utils.h" /**< "get_user_input", "ARR_SIZE" */


/**
 * @brief   Ask the user for hers/his configuration file.
 * @details The configuration file hold the information later used by the
 *          program. These information are:
 *          - the ip address of the user's tcell
 *          - the port of the user's tcell
 *          - the user's global identification
 *          - the location of the private key in the system
 *          - the location of the public key in the system
 *          First the program asks if it should load the default configuration
 *          file. If the users answers no (s)he can provide an alternative path
 *          for a custom configuration file.
 *          If no configuration file exists then the program creates one based
 *          on the answers given by the user. If the user does not provide a
 *          pair of public/private keys the program will create one.
 *
 *          XXX What happens if the user deletes the folders used for storing
 *          the configuration files?
 *
 * @return MyInfo_t The structure that will hold the user's information or NULL
 *         on failure.
 */
MyInfo_t* ask_user_configuration_file(void)
{
    /* User's information will be stored here */
    MyInfo_t *my_info;

    /* To store the user's input */
    char buffer[ARR_SIZE];

    /* Default config file path */
    memset(buffer, '\0', ARR_SIZE);
    sprintf(buffer, "%s/.config/sipd/my_info.xml", getenv("HOME"));

    fprintf(stdout, "Use default configuration file? "
                    "($HOME/.config/sipd/my_info.xml)\n"
                    "If it does not exist it will be created and you will "
                    "be asked to fill in some information.\n[Y/n]: ");
    int ans = fgetc(stdin);

    /* User wants to load the default configuration file */
    if(ans == 'Y' || ans == 'y' || ans == '\n')
    {
        /* Test if my_info.xml exists, if the return value is not 0 then the
         * file does not exist */
        if(access(buffer, F_OK) != 0)
        {
            /* File does not exist, we create it */
            my_info = create_user_configuration_file(buffer);
        }
        else
        {
            /* If it does exist we parse it */
            my_info = parse_configuration_file(buffer);
        }
    }
    /* User wants to load a custom configuration file */
    else
    {
        get_user_input(buffer, ARR_SIZE, "Enter the path of the custom "
                                          "configuration file: ");
        /* XXX parse the custom configuration file if it exists */
        my_info = NULL;
    }

    return my_info;
}

