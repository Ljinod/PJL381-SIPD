OBJECTS_AES = aes_encryption.o
HEADERS_AES = ../aes_encryption.h ../../beans/file_desc.h ../../tools/utils.h

EXEC = main.exe
CC = gcc
CFLAGS = -Wall -Werror -O2

$(EXEC): main.c $(OBJECTS_AES) $(HEADERS_AES)
	$(CC) main.c $(OBJECTS_AES) $(CFLAGS) -o $@

aes_encryption.o: ../aes_encryption.c $(HEADERS_AES)
	$(CC) -c ../aes_encryption.c $(CFLAGS) -o $@


.PHONY: clean

clean:
	@rm -vf $(OBJECTS_AES) $(EXEC) *.enc
