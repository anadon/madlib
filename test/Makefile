CPP=g++
CC=gcc
LIBS=-pthread

CPPFLAGS=-ggdb -pg -O0 -pipe -Wall -Wextra -Wconversion -std=c++0x -march=native
CFLAGS=-ggdb -pg -O0 -pipe -Wall -Wextra -Wconversion -std=c99 -march=native
#CPPFLAGS=-O3 -pipe -Wall -Wextra -Wconversion -std=c++0x -march=native
#CFLAGS=-O3 -pipe -Wall -Wextra -Wconversion -std=c99 -march=native

EXEC=test.out
OLIB=correlation-matrix.a

MAINSOURCE=main.cpp
MAINOBJECT=main.o
SOURCES=correlation-matrix.cpp                                         \
        diagnostics.cpp                                                \
        kendall-correlation-matrix.cpp                                 \
        pearson-correlation-matrix.cpp                                 \
        quickmerge.cpp                                                 \
        rank-matrix.cpp                                                \
        simple-thread-dispatch.cpp                                     \
        spearman-correlation-matrix.cpp                                \
        statistics.c

OBJECTS= correlation-matrix.o                                          \
         diagnostics.o                                                 \
         kendall-correlation-matrix.o                                  \
         pearson-correlation-matrix.o                                  \
         quickmerge.o                                                  \
         rank-matrix.o                                                 \
         simple-thread-dispatch.o                                      \
         spearman-correlation-matrix.o                                 \
         statistics.o

HEADERS=correlation-matrix.hpp                                         \
        diagnostics.hpp                                                \
        kendall-correlation-matrix.hpp                                 \
        pearson-correlation-matrix.hpp                                 \
        quickmerge.hpp                                                 \
        rank-matrix.hpp                                                \
        short-primatives.h                                             \
        simple-thread-dispatch.hpp                                     \
        spearman-correlation-matrix.hpp                                \
        statistics.h                                                   \
        edge.tpp                                                       \
        graph.tpp                                                      \
        upper-diagonal-square-matrix.tpp                               \
        vertex.tpp                                                     \

all:$(EXEC) $(OLIB)

$(EXEC):$(OLIB) $(MAINOBJECT)
	$(CPP) $(CPPFLAGS) -flto $(MAINOBJECT) $(LIBS) $(OLIB) -o $(EXEC)

$(OLIB):$(OBJECTS)
	ar rcs $(OLIB) $(OBJECTS)

%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
	

clean:
	rm -f $(OBJECTS) $(MAINOBJECT)
	rm -f $(EXEC) $(OLIB)
	rm -f gmon.out
	rm -f *.o

