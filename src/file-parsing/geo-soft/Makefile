
all: geo-soft.yy geo-soft.l
	bison geo-soft.yy
	flex geo-soft.l
	$(CPP) $(CPPFLAGS) -c geo-soft.tab.cc
	mv lex.yy.c lex.yy.cc
	$(CPP) $(CPPFLAGS) -c lex.yy.cc
	$(CPP) $(CPPFLAGS) -c geo-soft.cpp

clean:
	rm -f geo-soft.tab.cc geo-soft.tab.hh lex.yy.cc lex.yy.c geo-soft.o geo-soft.tab.o  lex.yy.h  lex.yy.o
