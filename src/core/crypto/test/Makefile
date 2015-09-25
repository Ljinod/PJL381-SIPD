OBJECTS = aes_encryption.o rsa_encryption.o
HEADERS = ../aes_encryption.h ../../beans/file_desc.h ../../tools/utils.h \
		  ../rsa_encryption.h

EXEC = main.exe
CC = gcc
CFLAGS = -Wall -Werror -O2

$(EXEC): main.c $(OBJECTS) $(HEADERS)
	$(CC) main.c $(OBJECTS) $(CFLAGS) -o $@

aes_encryption.o: ../aes_encryption.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

rsa_encryption.o: ../rsa_encryption.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	@rm -vf $(OBJECTS) $(EXEC) *.enc
