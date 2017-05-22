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
	install -D $(TEMPLATES) $(HEADERS) -t $(DESTDIR)/usr/include
	install -D -s lib/madlib.a \
	           $(DESTDIR)/usr/lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION)
	ln -s lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION) \
	      $(DESTDIR)/usr/lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION)
	ln -s lib/madlib.a.$(MAJOR_VERSION).$(MINOR_VERSION) \
	      $(DESTDIR)/usr/lib/madlib.a.$(MAJOR_VERSION)
	ln -s lib/madlib.a.$(MAJOR_VERSION) \
	      $(DESTDIR)/usr/lib/madlib.a
	install -D -s lib/madlib.so \
	           $(DESTDIR)/usr/lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION)
	ln -s lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION).$(SUB_VERSION) \
	      $(DESTDIR)/usr/lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION)
	ln -s lib/madlib.so.$(MAJOR_VERSION).$(MINOR_VERSION) \
	      $(DESTDIR)/usr/lib/madlib.so.$(MAJOR_VERSION)
	ln -s lib/madlib.so.$(MAJOR_VERSION) \
	      $(DESTDIR)/usr/lib/madlib.so

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


