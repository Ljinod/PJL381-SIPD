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
int connect_to(const char *ip_addr, int port)
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
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = inet_addr(ip_addr);

    /* Connect to server */
    if(connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        print_error_info("[ERROR] Cannot connect to server");
        close(sock_fd);
        exit(-1);
    }

    return sock_fd;
}
