// ============================================================================
//
//       Filename:  utils.h
//         Author:  Loudet  Julien
//                  Maire   Stéphane
//                  Sabbagh Cyril
//          Email:  julien.loudet@telecom-paristech.fr
//                  stephane.maire@telecom-paristech.fr
//                  cyril.sabbagh@telecom-paristech.fr
//
//    Description:
//
//
// ============================================================================

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#define DAEMON_ADDRESS       ((const char *) "127.0.0.1"   )
//#define DAEMON_ADDRESS       ((const char *) "192.168.43.164"   )
#define DAEMON_PORT          ((int)          6789              )

#define DAEMON_SIZE_RESPONSE                 2000

#define MSG_OK               ((const char *) "OK"              )
#define MSG_ERROR            ((const char *) "ERROR"           )

#define TCELL_FILES_DIR      ((const char *) "TCell/files/"    )
#define TCELL_KEYS_DIR       ((const char *) "TCell/keys/"     )
#define TCELL_MYINFO         ((const char *) "TCell/myinfo"    )
#define APP_FILES_DIR        ((const char *) "app/files/"      )
#define TMP_TCELL_PUBKEY     ((const char *) "app/keys/tcells_keys/public.pem")

#define MSG_SEPARATOR        ((const char *) "|"               )
#define MSG_STORE            ((const char *) "store"           )
#define MSG_SHARE            ((const char *) "share"           )
#define MSG_READ_FILE        ((const char *) "read"            )
#define MSG_LIST_FILES       ((const char *) "list"            )
#define MSG_FILE             ((const char *) "file"            )

#define DB_NAME             ((const char *) "Tcell.db"         )

#define AUTO_DETER_SIZE      ((int)          -1            )

#define DEBUG_LINE fprintf(stdout, "DEBUG: %i\n", __LINE__)
#define MSG_ACK    ((const char*) "ACK")
#define ARR_SIZE   (256)
#define CMD_SIZE   (512)
#define MSG_SIZE   (2048)
/* We create keys of 2048bits hence an array of 512 bytes should be enough
 * (512 * 8 = 4096bits) */
#define RSA_PUB_SIZE (512)

static inline void print_error_info(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  fprintf(stderr, "ERROR INFO: %s\n", strerror(errno));
}

static inline void print_error(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
}

static inline void get_user_input(char *buffer, int buffer_size,
                                  const char* msg)
{
    /* Clear the buffer entirely */
    memset(buffer, '\0', buffer_size);

    /* Display message */
    fprintf(stdout, "%s", msg);

    /* Ask for user's input until its valid */
    while(fgets(buffer, buffer_size, stdin) == NULL || buffer[0] == '\n')
    {
        /* User's input was incorrect, wipe it */
        memset(buffer, '\0', buffer_size);
        fprintf(stdout, "Sorry, I did not understand your input.\n"
                        "Please re-try:\n%s", msg);
    }

    /* Remove the newline at the end */
    buffer[strcspn(buffer, "\n")] = '\0';
}

#endif
