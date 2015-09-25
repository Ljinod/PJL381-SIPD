#include <stdio.h>

#include "../aes_encryption.h"
#include "../../beans/file_desc.h"
#include "../rsa_encryption.h"

int main(void)
{
    FileDesc_t *f_desc = aes_encrypt_file("text.txt");

    if(f_desc == NULL)
    {
        fprintf(stderr, "Encrypt failed.\n");
    }
    else if(aes_decrypt_file(f_desc) == NULL)
    {
        fprintf(stderr, "Decrypt failed.\n");
    }

    char *msg = "Bonjour, j'aimerais être chiffré en RSA.";
    fprintf(stdout, "MSG TO ENCRYPT: %s\n", msg);
    const char *enc_msg =
        rsa_encrypt_msg(msg, "/home/julien/.config/sipd/keys/julien-sipd-rsa.pub");

    const char *dec_msg =
        rsa_decrypt_msg(enc_msg, "/home/julien/.config/sipd/keys/julien-sipd-rsa");

    fprintf(stdout, "\nDecrypted message: %s\n", dec_msg);

    return 0;
}
