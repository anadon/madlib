#!/usr/bin/make 


CPP=g++
CC=gcc
LIBS=-pthread

INCLUDE=$(shell pwd)/include
OLIB=lib/madlib.a lib/madlib.so
TEST_PROG=""

COMMON_FLAGS=-pipe -Wall -Wextra -Wconversion -march=native -I../include
CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x -I $(INCLUDE)
CFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c99 -I $(INCLUDE)
#CPPFLAGS=-O3 $(COMMON_FLAGS) -std=c++0x -march=native -I $(INCLUDE)
#CFLAGS=-O3 $(COMMON_FLAGS) -std=c99 -march=native -I $(INCLUDE)


all:$(OLIB)

test:$(OLIB) $(TEST_PROG)

debug:

install:

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
	echo "Error, build commands not completed."


