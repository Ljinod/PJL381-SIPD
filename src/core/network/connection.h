/**
 * @file    connection.h
 * @brief   Connection header.
 * @author  Loudet  Julien   <loudet.julien@gmail.com>
 * @author  Maire   Stéphane <stephane.maire@telecom-paristech.org>
 * @author  Sabbagh Cyril    <cyril.sabbagh@telecom-paristech.org>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015/09)
 */

int connect_to(const char *ip_addr, const char *port);


void send_msg_to(int sock_fd, const char* msg);


const char* recv_msg_from(int sock_fd);
