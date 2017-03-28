CPP=g++
CC=gcc
LIBS=-pthread 

COMMON_FLAGS=-Iinclude -Igoogletest/googletest/include/ -Llib    \
              -Lgoogletest/build/googlemock/gtest/ -pipe -Wall -Wextra \
              -Wconversion -march=native
CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x
CFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c99
#CPPFLAGS=-O3 $(COMMON_FLAGS) -std=c++0x
#CFLAGS=-O3 $(COMMON_FLAGS) -std=c99

EXEC=test.out

MAINSOURCE=main.cpp
MAINOBJECT=main.o
SOURCES=graph-text.cpp                                                 \
        quickmerge-test.cpp                                            \
        rank-matrix-test.cpp                                           \
        run-tests.cpp                                                  \
        statistics-test.cpp                                            \
        upper-diagonal-square-matrix-test.cpp

OBJECTS=graph-text.o                                                   \
        quickmerge-test.o                                              \
        rank-matrix-test.o                                             \
        run-tests.o                                                    \
        statistics-test.o                                              \
        upper-diagonal-square-matrix-test.o

HEADERS=diagnostics.hpp                                                \
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

all:$(EXEC)

$(EXEC): $(OBJECTS)
	$(CPP) $(CPPFLAGS) -flto $(LIBS) $(OBJECTS) -o $(EXEC)


%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJECTS) $(MAINOBJECT)
	rm -f $(EXEC) $(OLIB)
	rm -f gmon.out
	rm -f *.o

