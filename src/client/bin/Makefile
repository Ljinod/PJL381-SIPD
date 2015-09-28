HEADERS = ../../core/network/connection.h ../../core/network/transfer_file.h \
		  ../../core/configuration/configuration.h ../../core/beans/my_info.h \
		  ../../core/api/api.h ../../core/configuration/create_config.h \
		  ../../core/configuration/parse_config.h \
		  ../../core/crypto/aes_encryption.h \
		  ../../core/crypto/rsa_encryption.h ../../core/message/message.h \
		  ../../core/tools/utils.h

OBJECTS = connection.o transfer_file.o configuration.o store_file.o \
		  create_config.o parse_config.o aes_encryption.o rsa_encryption.o \
		  message.o list_files.o read_file.o

CC           = gcc
EXEC         = sipdc.exe
POSIX_DEF    = -D_POSIX_C_SOURCE=2
CFLAGS       = -std=gnu99 -Wall -Werror -O2
LIBXML_FLAGS = `xml2-config --cflags` `xml2-config --libs`

all: $(EXEC)

$(EXEC): ../client.c $(OBJECTS) $(HEADERS)
	$(CC) $< $(OBJECTS) $(CFLAGS) $(LIBXML_FLAGS) -o $@

connection.o: ../../core/network/connection.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

transfer_file.o: ../../core/network/transfer_file.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

configuration.o: ../../core/configuration/configuration.c $(HEADERS)
	$(CC) -c $< $(LIBXML_FLAGS) $(CFLAGS) -o $@

create_config.o: ../../core/configuration/create_config.c $(HEADERS)
	$(CC) -c $< $(LIBXML_FLAGS) $(CFLAGS) -o $@

parse_config.o: ../../core/configuration/parse_config.c $(HEADERS)
	$(CC) -c $< $(LIBXML_FLAGS) $(CFLAGS) -o $@

rsa_encryption.o: ../../core/crypto/rsa_encryption.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) $(POSIX_DEF) -o $@

aes_encryption.o: ../../core/crypto/aes_encryption.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

message.o: ../../core/message/message.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

store_file.o: ../../core/api/store_file.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

list_files.o: ../../core/api/list_files.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

read_file.o: ../../core/api/read_file.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	@rm -vf $(OBJECTS) $(EXEC)

.PHONY: clean
