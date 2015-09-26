#include <stdio.h>

#include "../message.h"
#include "../../beans/file_desc.h"

int main(void)
{
    /* Fake file descriptor */
    FileDesc_t file_desc;
    strcpy(file_desc.file_gid, "fakeGID");
    strcpy(file_desc.file_id, "fakeID");
    file_desc.size = 9001;
    strcpy(file_desc.type, "fakefile!");
    strcpy(file_desc.description, "A fake file which size is over 9000!");
    strcpy(file_desc.symmetric_key, "atotallyfakesymmetrickey");
    strcpy(file_desc.initialisation_vector,
           "anincrediblyfakeinitialisationvector");

    fprintf(stdout, "[TEST] Create store message:\n%s\n",
            create_store_message(&file_desc));

    fprintf(stdout, "[TEST] Create read message:\n%s\n",
            create_read_message("GimmeAFile.txt"));

    fprintf(stdout, "[TEST] Create list files message:\n%s\n",
            create_list_files_message());

    return 0;
}
