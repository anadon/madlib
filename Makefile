#!/usr/bin/make

##Versioning notes:
## Major.Minor.Sub
##Major -- feaure removed and backwards compatability changed
##Minor -- features added
##Sub -- bugfix

MAJOR_VERSION=1
MINOR_VERSION=0
SUB_VERSION=1
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION)


CPP=g++
CC=gcc
LIBS=-pthread

INCLUDE=$(shell pwd)/include
OLIB=lib/libmadlib.a lib/libmadlib.so

TEMPLATES=include/graph.tpp                                                    \
          include/upper-diagonal-square-matrix.tpp

HEADERS=include/diagnostics.hpp                                                \
        include/kendall-correlation-matrix.hpp                                 \
        include/pearson-correlation-matrix.hpp                                 \
        include/quickmerge.hpp                                                 \
        include/rank-matrix.hpp                                                \
        include/short-primatives.h                                             \
        include/simple-thread-dispatch.hpp                                     \
        include/spearman-correlation-matrix.hpp                                \
        include/statistics.h


COMMON_FLAGS=-pipe -Wall -Wextra -Wconversion -march=native -I../include -fPIC

export

all: release

release: CPPFLAGS=-O3 $(COMMON_FLAGS) -std=c++0x -march=native -I $(INCLUDE)
release: CFLAGS=-O3 $(COMMON_FLAGS) -std=c99 -march=native -I $(INCLUDE)
release: $(OLIB)

debug: CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x -I $(INCLUDE)
debug: CFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c99 -I $(INCLUDE)
debug: $(OLIB)

test:debug $(TEST_PROG)
	#@make -f test/Makefile
	cd test; make

install:$(OLOB)
	install -D $(TEMPLATES) $(HEADERS) -t $(DESTDIR)/usr/include
	#install -D -s lib/libmadlib.a \
	#           $(DESTDIR)/usr/lib/libmadlib.a
	#install -D -s lib/libmadlib.so \
	#           $(DESTDIR)/usr/lib/libmadlib.so
	cp lib/libmadlib.a /usr/lib/libmadlib.a
	cp lib/libmadlib.so /usr/lib/libmadlib.so

clean:
	#@make -f src/Makefile clean
	cd src ; make clean
	#@make -f test/Makefile clean
	cd test ; make clean
	rm -f $(OLIB)
	rm -rf lib

#$(EXEC):$(OLIB) $(MAINOBJECT)
#	$(CPP) $(CPPFLAGS) -flto $(MAINOBJECT) $(LIBS) $(OLIB) -o $(EXEC)

$(OLIB):$(shell find src -type f) $(shell find include -type f)
	#@make -f src/Makefile
	cd src ; make
	mkdir -p lib
	mv src/libmadlib.so lib/
	mv src/libmadlib.a lib/

$(TEST_PROG): $(OLIB)
	#@make test/Makefile
	cd test ; make
