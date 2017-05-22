#!/usr/bin/make

##Versioning notes:
## Major.Minor.Sub
##Major -- feaure removed and backwards compatability changed
##Minor -- features added
##Sub -- bugfix

MAJOR_VERSION=1
MINOR_VERSION=0
SUB_VERSION=0
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION)


CPP=g++
CC=gcc
LIBS=-pthread

INCLUDE=$(shell pwd)/include
OLIB=lib/madlib.a lib/madlib.so

TEMPLATES=graph.tpp                                                    \
          upper-diagonal-square-matrix.tpp

HEADERS=diagnostics.hpp                                                \
        kendall-correlation-matrix.hpp                                 \
        pearson-correlation-matrix.hpp                                 \
        quickmerge.hpp                                                 \
        rank-matrix.hpp                                                \
        short-primatives.h                                             \
        simple-thread-dispatch.hpp                                     \
        spearman-correlation-matrix.hpp                                \
        statistics.h


COMMON_FLAGS=-pipe -Wall -Wextra -Wconversion -march=native -I../include

all: release

release: CPPFLAGS=-O3 $(COMMON_FLAGS) -std=c++0x -march=native -I $(INCLUDE)
release: CFLAGS=-O3 $(COMMON_FLAGS) -std=c99 -march=native -I $(INCLUDE)
release:$(OLIB)

debug: CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x -I $(INCLUDE)
debug: CFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c99 -I $(INCLUDE)
debug: $(OLIB)

test:debug $(TEST_PROG)
	cd test; make

install:$(OLOB)
	install -C include /usr/include
	install -s lib/madlib.a /usr/lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION)
	ln -s lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION) \
	      /usr/lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION)
	ln -s lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION) \
	      /usr/lib/madlib.a.$(MAJOR_VERSION)
	ln -s lib/madlib.a.$(MAJOR_VERSION) \
	      /usr/lib/madlib.a
	install -s lib/madlib.so /usr/lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION)
	ln -s lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION) \
	      /usr/lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION)
	ln -s lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION) \
	      /usr/lib/madlib.so.$(MAJOR_VERSION)
	ln -s lib/madlib.so.$(MAJOR_VERSION) \
	      /usr/lib/madlib.so

clean:
	cd src ; make clean
	cd test ; make clean
	rm -f $(OLIB)

#$(EXEC):$(OLIB) $(MAINOBJECT)
#	$(CPP) $(CPPFLAGS) -flto $(MAINOBJECT) $(LIBS) $(OLIB) -o $(EXEC)

$(OLIB):$(shell find src -type f) $(shell find include -type f)
	cd src ; make
	mv src/madlib.so lib/
	mv src/madlib.a lib/

$(TEST_PROG): $(OLIB)
	cd test ; make


