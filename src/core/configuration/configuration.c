/**
 * @file    configuration.c
 * @brief   Ask or create a configuration file.
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015-10-13
 *           -- updated comments
 *           -- implemented the )
 */

#include <stdio.h> /* sprintf, fprintf */
#include <stdlib.h> /* getenv */
#include <string.h> /* memset */
#include <unistd.h> /* access */

#include "configuration.h"
#include "create_config.h"
#include "parse_config.h"
#include "../tools/utils.h"


/**
 * @brief   Ask the user for hers/his configuration file.
 * @details The configuration file hold the information later used by the
 *          program.
 *
 *          First the program asks if it should load the default configuration
 *          file. If the users answers no (s)he can provide an alternative path
 *          for a custom configuration file.
 *
 *          If the user asked to use the default configuration file but it does
 *          not exist then the program creates one. It will ask questions to
 *          the user in order to fill in the required fields.
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


beginning:

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
        if(access(buffer, R_OK) != 0)
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

        /* we check that the file exists */
        if(access(buffer, R_OK) != 0)
        {
            fprintf(stderr, "[ERROR] I'm sorry the file you specified does\n"
                            "        not exist or you do not have the\n"
                            "        permission to read it. Starting over.\n");
            goto beginning;
        }

        /* file does exist, parse it! */
        my_info = parse_configuration_file(buffer);
    }

    return my_info;
}

