/**
 * @file    transfer_file.h
 * @brief   XXX Add brief description!
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 */


#ifndef __TRANSFER_FILE_H__
#define __TRANSFER_FILE_H__

#include "../beans/file_desc.h"

void send_file_to(int sock_fd, FileDesc_t* file_desc);


/**
 * @brief Receive a file through a socket.
 *
 * @param sock_fd   The socket through which the file is sent.
 * @param file_path The path of the file that will be created.
 */
void recv_file_from(int sock_fd, int file_size, const char *file_path);

#endif

