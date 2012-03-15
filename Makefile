PREFIX = /usr/local
LIB_DIR = ${PREFIX}/lib
INCLUDE_DIR = ${PREFIX}/include

VERSION = 0.1.0

.PHONY: all install uninstall clean dist

all:
	make -C src/

install:
	install -d ${INCLUDE_DIR}/libcassava
	install -m644 src/*.h ${INCLUDE_DIR}/libcassava/
	install -m755 src/libcassava.so ${LIB_DIR}/libcassava.so.${VERSION}
	install -m644 src/libcassava.a ${LIB_DIR}/
	ldconfig ${LIB_DIR}

uninstall:
	rm -r ${INCLUDE_DIR}/libcassava
	rm ${LIB_DIR}/libcassava.so.${VERSION}
	rm ${LIB_DIR}/libcassava.a
	ldconfig ${LIB_DIR}

clean:
	make -C src/ clean
	rm libcassava-${VERSION}.tar.gz

# vim: set noet:
