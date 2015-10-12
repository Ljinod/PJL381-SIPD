HEADERS = ../../core/beans/my_info.h ../../core/configuration/configuration.h \
	../../core/tools/utils.h ../../core/network/connection.h \
	../../core/crypto/rsa_encryption.h ../../core/message/message.h \
	../../core/configuration/create_config.h ../tcell.h \
	../../core/configuration/parse_config.h ../../database/database.h

OBJECTS = configuration.o create_config.o parse_config.o connection.o \
	rsa_encryption.o tcell_store.o database_operations.o \
	string_operations.o transfer_file.o create_database.o \
	tcell_list_files.o message.o tcell_read_file.o

CC           = gcc
EXEC         = tcelld.exe
POSIX_DEF    = -D_POSIX_C_SOURCE=2
SQL_FLAGS    = -lsqlite3
THREAD_FLAG  = -lpthread
CFLAGS       = -std=gnu99 -Wall -Werror -O2
LIBXML_FLAGS = `xml2-config --cflags` `xml2-config --libs`


all: $(EXEC)

$(EXEC): ../tcelld.c $(OBJECTS) $(HEADERS)
	$(CC) $< $(OBJECTS) $(THREAD_FLAG) $(LIBXML_FLAGS) $(POSIX_DEF) \
		$(SQL_FLAGS) $(CFLAGS) -o $@

configuration.o: ../../core/configuration/configuration.c $(HEADERS)
	$(CC) -c $< $(LIBXML_FLAGS) $(CFLAGS) -o $@

create_config.o: ../../core/configuration/create_config.c $(HEADERS)
	$(CC) -c $< $(LIBXML_FLAGS) $(CFLAGS) -o $@

parse_config.o: ../../core/configuration/parse_config.c $(HEADERS)
	$(CC) -c $< $(LIBXML_FLAGS) $(CFLAGS) -o $@

connection.o: ../../core/network/connection.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

message.o: ../../core/message/message.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

transfer_file.o: ../../core/network/transfer_file.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

rsa_encryption.o: ../../core/crypto/rsa_encryption.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(POSIX_DEF) -o $@

tcell_store.o: ../tcell_store.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(POSIX_DEF) -o $@

tcell_list_files.o: ../tcell_list_files.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(POSIX_DEF) -o $@

tcell_read_file.o: ../tcell_read_file.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(POSIX_DEF) -o $@

database_operations.o: ../../database/database_operations.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(SQL_FLAGS) -o $@

string_operations.o: ../../database/string_operations.c $(HEADERS)
	$(CC) -c $< $(CFLGAS) $(SQL_FLAGS) -o $@

create_database.o: ../../database/create_database.c $(HEADERS)
	$(CC) -c $< $(CFLGAS) $(SQL_FLAGS) -o $@

clean:
	@rm -vf $(OBJECTS) $(EXEC)

.PHONY: clean
