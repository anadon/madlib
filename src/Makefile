
LIBS=-pthread

STATIC_LIB=libmadlib.a
DYNAMIC_LIB=libmadlib.so
SUBLIBS_OBJECTS=file-parsing.o


CPPSOURCES=diagnostics.cpp                                                    \
           kendall-correlation-matrix.cpp                                     \
           pearson-correlation-matrix.cpp                                     \
           rank-matrix.cpp                                                    \
           simple-thread-dispatch.cpp                                         \
           spearman-correlation-matrix.cpp                                    \
           statistics.cpp

CSOURCES=sparse-bitpacked-array.c

OBJECTS=diagnostics.o                                                         \
        kendall-correlation-matrix.o                                          \
        pearson-correlation-matrix.o                                          \
        rank-matrix.o                                                         \
        simple-thread-dispatch.o                                              \
        spearman-correlation-matrix.o                                         \
        statistics.o                                                          \
        sparse-bitpacked-array.o



all:$(STATIC_LIB) $(DYNAMIC_LIB) $(SUBLIBS_OBJECTS)

$(STATIC_LIB):$(OBJECTS) $(SUBLIBS_OBJECTS)
	ar rcs $(STATIC_LIB) $(OBJECTS) $(SPEC_OBJECTS)
	ranlib $(STATIC_LIB)

$(DYNAMIC_LIB):$(OBJECTS) $(SUBLIBS_OBJECTS)
	$(CPP) $(OBJECTS) $(SPEC_OBJECTS) -shared -o $(DYNAMIC_LIB)

%.o:%.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

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
