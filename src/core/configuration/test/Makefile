HEADERS = ../configuration.h ../../beans/my_info.h ../../tools/utils.h \
		  ../create_config.h ../parse_config.h
OBJECTS = configuration.o create_config.o parse_config.o
LIBXML_FLAGS = `xml2-config --cflags` `xml2-config --libs`
CC = gcc
CFLAGS = -Wall -Werror -O2

main.exe: main.c $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) main.c $(LIBXML_FLAGS) $(CFLAGS) -o $@

configuration.o: ../configuration.c $(HEADERS)
	$(CC) -c ../configuration.c $(LIBXML_FLAGS) $(CFLAGS) -o $@

create_config.o: ../create_config.c $(HEADERS)
	$(CC) -c ../create_config.c $(LIBXML_FLAGS) $(CFLAGS) -o $@

parse_config.o: ../parse_config.c $(HEADERS)
	$(CC) -c ../parse_config.c $(LIBXML_FLAGS) $(CFLAGS) -o $@

.PHONY: clean

clean:
	@rm -vf main.exe $(OBJECTS)
