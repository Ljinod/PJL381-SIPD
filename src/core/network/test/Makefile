HEADERS = ../connection.h ../transfer_file.h ../../beans/file_desc.h \
		  ../../tools/utils.h
OBJECTS = connection.o transfer_file.o
CC = gcc
CFLAGS = -Wall -Werror -O2

all: main.exe server_test.exe

main.exe: main.c $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) main.c $(CFLAGS) -o $@

connection.o: ../connection.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

transfer_file.o: ../transfer_file.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

server_test.exe: server_test.c $(OBJECTS) $(HEADERS)
	$(CC) server_test.c $(OBJECTS) $(CFLAGS) -o $@

.PHONY: clean

clean:
	@rm -vf main.exe server_test.exe $(OBJECTS) transfered_text.txt
