CPP=g++
CC=gcc
LIBS=-pthread

##These should be defined in the calling Makefile
COMMON_FLAGS=-pipe -Wall -Wextra -Wconversion -Wmissing-declarations -march=native -I$(INCLUDE) -fPIC
#CPPFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c++0x
#CFLAGS=-ggdb -pg -O0 $(COMMON_FLAGS) -std=c99
CPPFLAGS=-O3 $(COMMON_FLAGS) -std=c++0x
CFLAGS=-O3 $(COMMON_FLAGS) -std=c99

STATIC_LIB=libmadlib.a
DYNAMIC_LIB=libmadlib.so
SUBLIBS_OBJECTS=file-parsing.o


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



all:$(STATIC_LIB) $(DYNAMIC_LIB) $(SUBLIBS_OBJECTS)

$(STATIC_LIB):$(OBJECTS) $(SUBLIBS_OBJECTS)
	ar rcs $(STATIC_LIB) $(OBJECTS) $(SPEC_OBJECTS)
	ranlib $(STATIC_LIB)

$(DYNAMIC_LIB):$(OBJECTS) $(SUBLIBS_OBJECTS)
	$(CPP) $(OBJECTS) $(SPEC_OBJECTS) -shared -o $(DYNAMIC_LIB)

%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -fPIC -c $< -o $@

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(SUBLIBS_OBJECTS):
	$(eval SUBDIR := $(basename $@))
	$(MAKE) -C $(SUBDIR)
	ld -r $(SUBDIR)/*.o -o $@

clean:
	rm -f $(OBJECTS) $(SPEC_OBJECTS)
	rm -f $(STATIC_LIB) $(DYNAMIC_LIB)
	rm -f gmon.out
	rm -f *.o
	rm -f *.a
	for obj in $(basename $(SUBLIBS_OBJECTS) ) ; do \
		$(MAKE) -C $$obj clean; \
	done
