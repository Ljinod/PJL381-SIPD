#include <stdio.h>

#include "../aes_encryption.h"
#include "../../beans/file_desc.h"

int main(void)
{
    FileDesc_t *f_desc = aes_encrypt_file("text.txt");

    if(f_desc == NULL)
    {
        fprintf(stdout, "File desc is NULL.\n");
    }
    return 0;
}
