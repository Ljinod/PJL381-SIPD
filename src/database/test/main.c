#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "../database.h"


int main(int argc, char *argv[])
{
    char tricky_str[256];
    sprintf(tricky_str, "I\'m\b\\0ring.\tha\t\"sFo\rSureMe\n\\!");
    fprintf(stdout, "[TEST] Checking safe_string_for_db with:\n"
                    "\"\" %s \"\"\n", tricky_str);
    safe_string_for_db(tricky_str);
    fprintf(stdout, "[TEST] string after processing:\n\"\" %s \"\"\n",
            tricky_str);


    if(access(get_database_path(), R_OK|W_OK) != 0)
    {
        fprintf(stdout, "[TEST] Creating database.\n");
        create_database();
        fprintf(stdout, "[TEST] Database created!\n");
    }
    else
    {
        fprintf(stdout, "[TEST] Database already exists.\n");
    }

    FileDesc_t fdesc;
    strcpy(fdesc.file_gid             , "julientestfile.txt");
    strcpy(fdesc.file_id              , "testfile.txt");
    strcpy(fdesc.size                 , "9001");
    strcpy(fdesc.path                 , "/dev/null");
    strcpy(fdesc.type                 , "ouioui|,c'estuntype");
    strcpy(fdesc.description          , "jejejejejejejejejejejejejejejejeje");
    strcpy(fdesc.symmetric_key        , "asyyyyymmmetrickeybutjustsymmetric");
    strcpy(fdesc.initialisation_vector, "iniiiiiitvectoooooor");


    fprintf(stdout, "[TEST] Inserting file desc: (gid) %s\n", fdesc.file_gid);
    insertFile(&fdesc);
    fprintf(stdout, "[TEST] File desc inserted!\n");

    FileDesc_t fdesc_bis;
    strcpy(fdesc_bis.file_gid             , "testfile2");
    strcpy(fdesc_bis.file_id              , "testfile.txt");
    strcpy(fdesc_bis.size                 , "9001");
    strcpy(fdesc_bis.path                 , "/dev/null");
    strcpy(fdesc_bis.type                 , "ouioui|,c'estuntype");
    strcpy(fdesc_bis.description          , "jejejejejejejejejejejejejejejejeje");
    strcpy(fdesc_bis.symmetric_key        , "asyyyyymmmetrickeybutjustsymmetric");
    strcpy(fdesc_bis.initialisation_vector, "iniiiiiitvectoooooor");

    fprintf(stdout, "[TEST] Inserting file desc: (gid) %s\n", fdesc_bis.file_gid);
    insertFile(&fdesc_bis);
    fprintf(stdout, "[TEST] File desc inserted!\n");


    fprintf(stdout, "[TEST] Getting the file desc < %s > back...", fdesc.file_gid);

    FileDesc_t *fdesc2 = getFileDesc(fdesc.file_gid);
    if(strcmp(fdesc2->type, fdesc.type) == 0)
    {
        fprintf(stdout, "Ok\n");
    }
    else
    {
        fprintf(stdout, "FAILED\n");
    }

    fprintf(stdout, "[TEST] Listing all files...\n");

    FileDesc_t *all_files = getAllFileDesc();

    for(int i=0; i< getFileDescNumber(); i++)
    {
        fprintf(stdout, "[ALL FILES] file_gid: %s\n", all_files[i].file_gid);
    }

    fprintf(stdout, "[TEST] Done!\n");

   return 0;
}
