#include <string.h>
#include <stdio.h>

#include "../connection.h"
#include "../transfer_file.h"
#include "../../beans/file_desc.h"

int main(void)
{
    fprintf(stdout, "[TEST] Trying to connect to server.\n");
    int sock_fd = connect_to("127.0.0.1", 5678);
    FileDesc_t file_desc;
    strcpy(file_desc.path, "text.txt");
    file_desc.size = 27;
    // strcpy(file_desc.path, "music.flac");
    // file_desc.size = 29506658;

    fprintf(stdout, "[TEST] Trying to send file - %s - to server.\n",
                    file_desc.path);
    send_file_to(sock_fd, &file_desc);

    fprintf(stdout, "[TEST] File - %s - sent to server!\n", file_desc.path);

    return 0;
}