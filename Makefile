PREFIX = /usr/local
LIB_DIR = ${PREFIX}/lib
INCLUDE_DIR = ${PREFIX}/include

VERSION = 1.0.0

.PHONY: all lib install uninstall clean dist doc

all: dist doc

lib:
	make -C src/

doc:
	test -d dist || mkdir dist
	doxygen Doxyfile

dist: lib
	install -d dist
	install -m644 src/*.h dist/
	install -m644 src/libcassava.so dist/libcassava.so.${VERSION}
	install -m644 src/libcassava.a dist/

install:
	install -d ${INCLUDE_DIR}/libcassava
	install -m644 src/*.h ${INCLUDE_DIR}/libcassava/
	install -m755 src/libcassava.so ${LIB_DIR}/libcassava.so.${VERSION}
	install -m644 src/libcassava.a ${LIB_DIR}/
	ldconfig ${LIB_DIR}

uninstall:
	test -d ${INCLUDE_DIR}/libcassava && rm -r ${INCLUDE_DIR}/libcassava
	test -f ${LIB_DIR}/libcassava.so.${VERSION} && rm ${LIB_DIR}/libcassava.so.${VERSION}
	test -f ${LIB_DIR}/libcassava.a && rm ${LIB_DIR}/libcassava.a
	ldconfig ${LIB_DIR}

clean:
	make -C src/ clean
	test -d dist && rm -rf dist || true
	test -f libcassava-${VERSION}.tar.gz && rm libcassava-${VERSION}.tar.gz || true

# vim: set noet:
