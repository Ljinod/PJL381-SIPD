OBJECTS = message.o
HEADERS = ../message.h ../../beans/file_desc.h
CC = gcc
CFLAGS = -Wall -Werror -O2

all: main.exe

main.exe: main.c $(OBJECTS) $(HEADERS)
	$(CC) main.c $(OBJECTS) $(CFLAGS) -o $@

message.o: ../message.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	@rm -vf $(OBJECTS) main.exe
