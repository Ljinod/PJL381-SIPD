#include <stdio.h>

#include "../configuration.h"
#include "../../beans/my_info.h"

int main(void)
{
    MyInfo_t *my_info = ask_user_configuration_file();
    printf("%s\n%s\n%i\n%s\n%s\n", my_info->my_gid,
                                   my_info->my_tcell_ip,
                                   my_info->my_tcell_port,
                                   my_info->my_public_key,
                                   my_info->my_private_key);


    return 0;
}
