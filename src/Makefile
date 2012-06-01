CC = gcc -DNDEBUG
CFLAGS = --std=c99 -g -Wall -Wextra -pedantic -fpic
LFLAGS = -shared -fpic -Wl,-export-dynamic,-soname,libcassava.so.1

objects = config_kv.o list.o list_str.o string.o util.o system.o bitset.o

.PHONY: all clean check library

library: libcassava.so libcassava.a

all: tags check libcassava.so libcassava.a test

tags:
	ctags --exclude=test.c *.c *.h

check:
	cppcheck -q --enable=all *.c *.h

test: test.c libcassava.a
	${CC} ${CFLAGS} -lm -o test test.c libcassava.a

libcassava.so: ${objects}
	${CC} ${LFLAGS} -lm -lc -o libcassava.so ${objects}

libcassava.a: ${objects}
	ar rcs libcassava.a ${objects}

config_kv.o: config_kv.h config_kv.c
	${CC} ${CFLAGS} -c config_kv.c

list.o: list.h list.c
	${CC} ${CFLAGS} -c list.c

list_str.o: list.h string.h list_str.h list_str.c
	${CC} ${CFLAGS} -c list_str.c

string.o: string.h string.c
	${CC} ${CFLAGS} -c string.c

util.o: list.h list_str.h string.h util.h util.c
	${CC} ${CFLAGS}  -c util.c

system.o: system.h system.c
	${CC} ${CFLAGS} -c system.c

bitset.o: bitset.h bitset.c
	${CC} ${CFLAGS} -c bitset.c

clean:
	for file in ${objects} tags libcassava.a libcassava.so test; do \
		test -f $$file && echo "rm $$file" && rm $$file || continue; \
	done

# vim: set noet:
