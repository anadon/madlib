#CPP=g++
#CC=gcc
LIBS=-pthread

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = googletest/googletest

#Rapidcheck source and build directories needed for automated coverage
#testing.
RC_SRC_DIR = rapidcheck
RC_BLD_DIR = rapidcheck_build

# Where to find user code.
USER_DIR = .

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = RANK_MATRIX_TEST SORT_LOW_TO_HIGH SORT_HIGH_TO_LOW GRAPH               \
        UpperDiagonalMatrixTest DoubleSidedStack

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h                                 \
                $(GTEST_DIR)/include/gtest/internal/*.h

RC_HEADERS = $(RC_SRC_DIR)/include/*.h                                         \
             $(RC_SRC_DIR)/extras/gtest/include/rapidcheck/gtest.h


COMMON_FLAGS = -I../include -Igoogletest/googletest/include/ -pipe -Wall       \
             -Wextra -Wconversion -march=native -DMADLIB_DEBUG
CPP_LIBS = -Llib -Lgoogletest/build/googlemock/gtest/                          \
         -Lrapidcheck_build/
CPPFLAGS += -ggdb -O0 $(COMMON_FLAGS) -isystem -I$(GTEST_DIR)/include          \
            -I$(RC_SRC_DIR)/include                                            \
            -I$(RC_SRC_DIR)/extras/gtest/include/rapidcheck

EXEC = test.out

GTEST_LIB = gtest_main.a

RC_LIB = $(RC_BLD_DIR)/librapidcheck.a

SOURCES=graph-test.cpp                                                         \
        timsort-test.cpp                                                       \
        rank-matrix-test.cpp                                                   \
        statistics-test.cpp                                                    \
        upper-diagonal-square-matrix-test.cpp                                  \
        double-sided-stack-test.cpp                                            \
        alphabet-sort-test.cpp

OBJECTS=graph-test.o                                                           \
        timsort-test.o                                                         \
        rank-matrix-test.o                                                     \
        statistics-test.o                                                      \
        upper-diagonal-square-matrix-test.o                                    \
        double-sided-stack-test.o                                              \
        alphabet-sort-test.o

HEADERS=include/diagnostics.hpp                                                \
        include/correlation-matrix.hpp                                         \
        include/timsort.hpp                                                    \
        include/rank-matrix.hpp                                                \
        include/short-primatives.h                                             \
        include/simple-thread-dispatch.hpp                                     \
        include/statistics.h                                                   \
        include/graph.hpp                                                      \
        include/upper-diagonal-square-matrix.hpp                               \
        include/double-sided-stack.hpp                                         \
        include/alphabet_sort.hpp                                              \
        $(GTEST_HEADERS)                                                       \
	$(RC_HEADERS)


all:$(EXEC) $(HEADERS)

$(EXEC): $(OBJECTS) $(GTEST_LIB) $(RC_LIB) $(HEADERS)
	$(CPP) $(CPPFLAGS) -flto $(LIBS) $(OBJECTS) ../lib/libmadlib.a $(GTEST_LIB) -o $(EXEC)

%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXEC) $(OLIB)
	rm -f gmon.out
	rm -f *.o
	rm -f $(TESTS) gtest.a gtest_main.a
	rm -f *.a
	rm -rf $(RC_BLD_DIR)


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

$(GTEST_LIB) : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(RC_LIB) : $(RC_BLD_DIR)

$(RC_BLD_DIR) : $(RC_SRC_DIR)
	mkdir -p $(RC_BLD_DIR)
	cd $(RC_BLD_DIR) ; cmake $(RC_SRC_DIR) ; make
