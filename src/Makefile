CPP=g++
CC=gcc
LIBS=-pthread

##These should be defined in the calling Makefile
COMMON_FLAGS=-pipe -Wall -Wextra -Wconversion -Wmissing-declarations -march=native -I../include
CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x
CFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c99
#CPPFLAGS=-O3 $(COMMON_FLAGS) -std=c++0x
#CFLAGS=-O3 $(COMMON_FLAGS) -std=c99

STATIC_LIB=libmadlib.a
DYNAMIC_LIB=libmadlib.so


SOURCES=diagnostics.cpp                                                \
        kendall-correlation-matrix.cpp                                 \
        pearson-correlation-matrix.cpp                                 \
        quickmerge.cpp                                                 \
        rank-matrix.cpp                                                \
        simple-thread-dispatch.cpp                                     \
        spearman-correlation-matrix.cpp                                \
        statistics.c                                                   \
        sparse-bitpacked-array.c

OBJECTS= diagnostics.o                                                 \
         kendall-correlation-matrix.o                                  \
         pearson-correlation-matrix.o                                  \
         quickmerge.o                                                  \
         rank-matrix.o                                                 \
         simple-thread-dispatch.o                                      \
         spearman-correlation-matrix.o                                 \
         statistics.o                                                  \
         sparse-bitpacked-array.o


all:$(STATIC_LIB) $(DYNAMIC_LIB)

$(STATIC_LIB):$(OBJECTS)
	ar rcs $(STATIC_LIB) $(OBJECTS)
	ranlib $(STATIC_LIB)

$(DYNAMIC_LIB):$(OBJECTS)
	gcc $(OBJECTS) -shared -o $(DYNAMIC_LIB)

%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -fPIC -c $< -o $@

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@


clean:
	rm -f $(OBJECTS)
	rm -f $(STATIC_LIB) $(DYNAMIC_LIB)
	rm -f gmon.out
	rm -f *.o
	rm -f *.a
