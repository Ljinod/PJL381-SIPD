/**
 * @file    connection.c
 * @brief   This file deals with connections between sockets and servers.
 * @author  Loudet  Julien   <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-04
 *
 * @details (last edited by Loudet Julien on 2015/09)
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> /* close */
#include <string.h> /* strlen */
#include <stdlib.h>  /* exit, atoi */

#include "connection.h"
#include "../tools/utils.h"

/**
 * @brief This function tries to connect to a server/daemon/tcell.
 *
 * @param ip_addr the address of the server, expected format is 192.168.0.0
 * @param port    the port on which the server is listening
 *
 * @return if successful the function returns the corresponding socket file
 *         descriptor.
 */
int connect_to(const char *ip_addr, const char *port)
{
    int                sock_fd;
    struct sockaddr_in server;

    /* Create client socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1)
    {
        print_error_info("[ERROR] Could not create socket.");
    }

    /* Server address */
    server.sin_family      = AF_INET;
    server.sin_port        = htons(atoi(port));
    server.sin_addr.s_addr = inet_addr(ip_addr);

    /* Connect to server */
    if(connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        print_error_info("[ERROR] Cannot connect to server.");
        close(sock_fd);
        exit(-1);
    }

    return sock_fd;
}


void send_msg_to(int sock_fd, const char* msg)
{
    int bytes_sent, msg_len = strlen(msg);
    if(msg_len > MSG_SIZE)
    {
        fprintf(stderr, "[ERROR] Ohoh, it seems that you have reached the\n"
                        "        limits of my program. The following message\n"
                        "        was too long to be handled:\n%s\n", msg);
        exit(-1);
    }

    if((bytes_sent = send(sock_fd, (const void*) msg, msg_len, 0)) != msg_len)
    {
        fprintf(stderr, "[ERROR] Could not send the message.\n"
                        "        Message's length was: %i\n"
                        "        Bytes sent          : %i\n",
                        msg_len, bytes_sent);
        exit(-1);
    }
}

char* recv_msg_from(int sock_fd)
{
    int bytes_recv;
    char buffer[MSG_SIZE], *msg_recv;
    memset(buffer, '\0', MSG_SIZE);

    if((bytes_recv = recv(sock_fd, (void *)buffer, MSG_SIZE, 0)) == -1)
    {
        fprintf(stderr, "[ERROR] Could not receive the message sent.\n");
        exit(-1);
    }

    /* Allocate correct amount of memory for the message, the "+ 1" is because
     * does not count the final '\0' character thus when copying if we don't
     * add one the receiving array would be too short. */
    msg_recv = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(msg_recv, buffer);

    return msg_recv;
}
