/**
 * @file    transfer_file.c
 * @brief   Send or receive a file through a socket.
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details
 *
 *          ===================================================================
 *          TO DO
 *          ===================================================================
 *          (*) Add brief descriptions
 *          (*) See if there is a way to display the advancement of the file
 *              transfer on both side
 *          (*) <send_file_to>: give the prerequisites of the function
 */

#include "transfer_file.h"

#include <sys/sendfile.h> /* sendfile */
#include <unistd.h>       /* close */
#include <sys/types.h>    /* open, recv */
#include <sys/stat.h>     /* open */
#include <fcntl.h>        /* open */
#include <stdio.h>        /* fprintf, fwrite, fopen */
#include <sys/socket.h>   /* recv */
#include <string.h>       /* memset */
#include <stdlib.h>       /* atoi */



#define BUFFER_SIZE   (512)

/**
 * @brief Send a file through a socket.
 *
 * @details XXX What are the prerequisites of this function?
 *
 * @param sock_fd   The socket through which the file will be sent.
 * @param file_desc The file to send.
 */
void send_file_to(int sock_fd, FileDesc_t *file_desc)
{
    int bytes_sent, file_size, file_fd;
    off_t offset = 0;

    /* Open the file in read only mode */
    if((file_fd = open(file_desc->path, O_RDONLY)) == -1)
    {
        fprintf(stderr, "[ERROR] An error occurred while trying to open"
                        "        the file: %s\n", file_desc->path);
        exit(-1);
    }

    /* Get the file size */
    file_size = atoi(file_desc->size);

    /* Send it */
    while(file_size > 0  &&
          (bytes_sent = sendfile(sock_fd, file_fd, &offset, BUFFER_SIZE)) > 0)
    {
        /* XXX Can we display a % showing the advancement of the upload? */
        file_size -= bytes_sent;
        fprintf(stdout, "[INFO] %d bytes sent - %d remaining\n", bytes_sent,
                                                                 file_size);
        if(file_size == 0)
            break;
    }

    /* Check that we did send every last bytes of it. */
    if(file_size > 0)
    {
        fprintf(stderr, "[WARNING] Maybe something happened while transfering:"
                        "          %s\n"
                        "          Apparently the transfer was not completed."
                        "\n", file_desc->path);
    }

    /* Close the file descriptor */
    if(close(file_fd) != 0)
    {
        fprintf(stderr, "[ERROR] An error occurred while trying to close"
                        "        the file: %s\n", file_desc->path);
        exit(-1);
    }

    fprintf(stdout, "[DEBUG] Send over!\n");
}


/**
 * @brief Receive a file through a socket.
 *
 * @details XXX What are the prerequisits of this function?
 *
 * @param sock_fd   The socket through which the file is sent.
 * @param file_size The size of the file.
 * @param file_path The path of the file to create.
 */
void recv_file_from(int sock_fd, const char *file_size, const char *file_path)
{
    int   bytes_recv, i_file_size = atoi(file_size);
    char  buffer[BUFFER_SIZE];
    FILE *file_stream;

    /* Create the file */
    file_stream = fopen(file_path, "w");
    if(file_stream == NULL)
    {
        fprintf(stderr, "[ERROR] Could not create file: %s\n", file_path);
        exit(-1);
    }

    /* Precautions precautions, clean the buffer */
    memset(buffer, '\0', BUFFER_SIZE);

    /* Receive the file, by blocks of BUFFER_SIZE */
    while(i_file_size > 0 &&
          (bytes_recv = recv(sock_fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        /* write what we received */
        fwrite(buffer, sizeof(char), bytes_recv, file_stream);
        i_file_size -= bytes_recv;
        fprintf(stdout, "[INFO] %d bytes received - %d remaining\n",
                        bytes_recv, i_file_size);

        /* Precautions precautions, clean the buffer */
        memset(buffer, '\0', BUFFER_SIZE);
    }

    /* Check that we received everything */
    if(i_file_size > 0)
    {
        fprintf(stderr, "[WARNING] Maybe something happened while transfering:"
                        " %s\n"
                        "          Apparently the transfer was not completed: "
                        "%i bytes missing.\n", file_path, i_file_size);
    }

    /* Close the file */
    if(fclose(file_stream) != 0)
    {
        fprintf(stderr, "[ERROR] An error occurred while trying to close"
                        "        the file: %s\n", file_path);
        exit(-1);
    }

    fprintf(stdout, "[DEBUG] Recv over!\n");
}

