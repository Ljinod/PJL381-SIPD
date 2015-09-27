/**
 * @file    tcell-daemon.c
 * @brief   The tcell daemon.
 * @author  Loudet  Julien   <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015/09)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <libgen.h>

#include "../core/beans/my_info.h"
#include "../core/configuration/configuration.h"
#include "../core/tools/utils.h"
#include "../core/message/message.h"
#include "../core/network/connection.h"
#include "../core/crypto/rsa_encryption.h"
#include "../database/database.h"
#include "tcell.h"



/* thread function that will handle incoming connections */
void *connection_handler(void *);

MyInfo_t *my_info;

/**
 * @brief XXX Add brief description!
 *
 * @param argc
 * @param argv
 *
 * @return 0 on success, -1 otherwise.
 */
int main(int argc, char **argv)
{
    int                server_sock_fd, client_sock_fd;
    struct sockaddr_in server, client;
    pthread_t          thread_client;

    /* get the user's information */
    my_info = ask_user_configuration_file();

    /* check if the database exists, if not create it */
    if(access(get_database_path(), R_OK|W_OK) != 0)
    {
        create_database();
    }

    /* Create first socket */
    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1)
    {
        print_error_info("[ERROR] tcelld: Could not create socket.");
    }

    /* Creating server socket */
    server.sin_family      = AF_INET;
    server.sin_port        = htons(atoi(my_info->my_tcell_port));
    server.sin_addr.s_addr = inet_addr(my_info->my_tcell_ip);

    /* Bind server socket to port */
    if(bind(server_sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        print_error_info("[ERROR] tcelld: Could not bind socket.");
        close(server_sock_fd);
        exit(EXIT_FAILURE);
    }

    /* Listening mode: we can queue up to 10 connections */
    listen(server_sock_fd, 10);

    /* Now we can wait for incoming connections */
    int cli_len = sizeof(struct sockaddr_in);
    while(1)
    {
        fprintf(stdout, "[INFO] tcelld: Waiting for connection...");

        /* Once a "connect" is done, we try to accept it */
        client_sock_fd = accept(server_sock_fd, (struct sockaddr *) &client,
                (socklen_t*) &cli_len);
        if(client_sock_fd < 0)
        {
            print_error_info("[ERROR] tcelld: Accept failed.");
        }

        /* If we could accept the connection then we handle it in a new thread */
        if(pthread_create(&thread_client, NULL, connection_handler,
                    (void *) &client_sock_fd) < 0)
        {
            print_error_info("[ERROR] tcelld: Could not create thread.");
        }
    }

    return 0;
}


/**
 * @brief XXX Add brief description!
 *
 * @param socket_fd
 *
 * @return
 */
void *connection_handler(void *socket_fd)
{
    /* We cast the socket into an int to be able to use it as a file
     * descriptor */
    int sock = *(int *)socket_fd;
    fprintf(stdout, "[INFO] tcelld: Connection request accepted.\n"
                    "               Socket: %i\n", sock);

    /* Receive encrypted message from client */
    char *enc_client_msg = recv_msg_from(sock);
    /* Decrypt the message */
    char *client_msg = rsa_decrypt_msg(my_info->my_private_key,
                                             enc_client_msg);
    free(enc_client_msg);

    char copy_client_msg[strlen(client_msg) + 1];
    strcpy(copy_client_msg, client_msg);

    /* Process message: what action was started */
    char *action = strtok(client_msg, MSG_SEP);
    if(strcmp(action, ACT_STORE) == 0)
    {
        tcell_store(my_info, sock, copy_client_msg);
    }
    else
    {
        print_error("[ERROR] tcelld: Unknown action requested.");
        exit(EXIT_FAILURE);
    }

    /* Free resources */
    free(client_msg);
    return 0;
}

