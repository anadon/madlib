
all: geo-soft.yy geo-soft.l
	bison geo-soft.yy
	flex geo-soft.l
	g++ -c geo-soft.tab.cc
	g++ -c lex.yy.c

clean:
	rm -f *.cc *.hh *.c *.h
