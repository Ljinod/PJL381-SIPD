/**
 * @file    message.h
 * @brief   The messages controlling the tcell.
 * @author  Loudet Julien
 * @version 1.1
 * @date    2015-09
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define MSG_SEP         ((const char*) "|")
#define FIELD_VALUE_SEP ((const char*) "::")

#define FIELD_FILE_GID ((const char*) "filegid")
#define FIELD_FILE_ID  ((const char*) "fileid")
#define FIELD_SIZE     ((const char*) "size")
#define FIELD_TYPE     ((const char*) "type")
#define FIELD_DESC     ((const char*) "description")
#define FIELD_SALT     ((const char*) "salt")
#define FIELD_SYM_KEY  ((const char*) "symkey")
#define FIELD_IV       ((const char*) "initvect")

#define ACT_STORE      ((const char*) "store")
#define ACT_READ       ((const char*) "read")
#define ACT_LIST_FILES ((const char*) "listfiles")
#define ACT_SHARE      ((const char*) "share")

#include "../beans/file_desc.h"

char* create_store_message(const FileDesc_t* file_desc);

char* create_read_message(const char* file_gid);

char* create_list_files_message(void);

char* create_share_message(void);

#endif
