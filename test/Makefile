CPP=g++
CC=gcc
LIBS=-pthread 

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = googletest/googletest

# Where to find user code.
USER_DIR = .

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = RANK_MATRIX_TEST SORT_LOW_TO_HIGH SORT_HIGH_TO_LOW GRAPH UpperDiagonalMatrixTest

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h


COMMON_FLAGS=-I../include -Igoogletest/googletest/include/ -pipe -Wall \
             -Wextra -Wconversion -march=native
CPP_LIBS=-Llib -Lgoogletest/build/googlemock/gtest/
CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x -isystem \
         -I$(GTEST_DIR)/include

EXEC=test.out

SOURCES=graph-test.cpp                                                 \
        quickmerge-test.cpp                                            \
        rank-matrix-test.cpp                                           \
        run-tests.cpp                                                  \
        statistics-test.cpp                                            \
        upper-diagonal-square-matrix-test.cpp

OBJECTS=graph-test.o                                                   \
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
        $(GTEST_HEADERS)



all:$(EXEC)

$(EXEC): $(OBJECTS) gtest_main.a
	$(CPP) $(CPPFLAGS) -flto $(LIBS) $(OBJECTS) lib/madlib.a gtest_main.a -o $(EXEC)

%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXEC) $(OLIB)
	rm -f gmon.out
	rm -f *.o
	rm -f $(TESTS) gtest.a gtest_main.a


# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
