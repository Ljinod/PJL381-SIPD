OBJECTS = database_operations.o create_database.o string_operations.o
HEADERS = ../../core/beans/file_desc.h ../../core/beans/user.h \
		  ../../core/beans/my_info.h ../database.h
CC = gcc
CFLAGS = -std=c99 -Wall -Werror -O2
SQL_FLAGS = -lsqlite3

all: main.exe

main.exe: main.c $(OBJECTS) $(HEADERS)
	$(CC) $< $(OBJECTS) $(SQL_FLAGS) $(CFLAGS) -o $@

database_operations.o: ../database_operations.c $(HEADERS)
	$(CC) -c $< $(SQL_FLAGS) $(CFLAGS) -o $@

create_database.o: ../create_database.c $(HEADERS)
	$(CC) -c $< $(SQL_FLAGS) $(CFLAGS) -o $@

string_operations.o: ../string_operations.c $(HEADERS)
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	@rm -vf $(OBJECTS) main.exe
