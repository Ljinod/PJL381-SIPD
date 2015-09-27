#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include "../transfer_file.h"
#include "../connection.h"

#define PORT_NUMBER     5678
#define SERVER_ADDRESS  "127.0.0.1"

int main(int argc, char **argv)
{
    int server_socket;
    int peer_socket;
    socklen_t       sock_len;
    struct sockaddr_in      server_addr;
    struct sockaddr_in      peer_addr;

    /* Create server socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        fprintf(stderr, "Error creating socket --> %s", strerror(errno));

        exit(EXIT_FAILURE);
    }

    /* Zeroing server_addr struct */
    memset(&server_addr, 0, sizeof(server_addr));
    /* Construct server_addr struct */
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
    server_addr.sin_port = htons(PORT_NUMBER);

    /* Bind */
    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
    {
        fprintf(stderr, "Error on bind --> %s", strerror(errno));

        exit(EXIT_FAILURE);
    }

    /* Listening to incoming connections */
    if ((listen(server_socket, 5)) == -1)
    {
        fprintf(stderr, "Error on listen --> %s", strerror(errno));

        exit(EXIT_FAILURE);
    }

    sock_len = sizeof(struct sockaddr_in);
    /* Accepting incoming peers */
    peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
    if (peer_socket == -1)
    {
        fprintf(stderr, "Error on accept --> %s", strerror(errno));

        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));

    const char *msg_recv = recv_msg_from(peer_socket);
    fprintf(stdout, "Message received: %s\n", msg_recv);

    send_msg_to(peer_socket, "OK");

    //recv_file_from(peer_socket, "27", "transfered_text.txt");
    recv_file_from(peer_socket, "29506658", "files/music.flac");

    close(peer_socket);
    close(server_socket);

    return 0;
}
