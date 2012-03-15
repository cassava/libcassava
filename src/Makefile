CC = gcc
CFLAGS = -g -c -O2 -Wall -Wextra -pedantic -fpic
LFLAGS = -shared -fpic -Wl,-export-dynamic,-soname,libcassava.so.1

objects = config_kv.o list.o list_str.o string.o util.o

.PHONY: all clean

all: libcassava.so libcassava.a

libcassava.so: ${objects}
	${CC} ${LFLAGS} -o libcassava.so ${objects} -lc

libcassava.a: ${objects}
	ar rcs libcassava.a ${objects}

config_kv.o: config_kv.h config_kv.c
	${CC} ${CFLAGS} config_kv.c

list.o: list.h list.c
	${CC} ${CFLAGS} list.c

list_str.o: list.h string.h list_str.h list_str.c
	${CC} ${CFLAGS} list_str.c

string.o: string.h string.c
	${CC} ${CFLAGS} string.c

util.o: list.h list_str.h string.h util.h util.c
	${CC} ${CFLAGS} util.c

clean:
	rm ${objects}
	rm libcassava.so
	rm libcassava.a

# vim: set noet:
