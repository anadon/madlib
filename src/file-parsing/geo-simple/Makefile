
all: geo-soft.yy geo-soft.l
	bison geo-soft.yy
	flex geo-soft.l
	#mv geo-soft.tab.c geo-soft.tab.cc
	g++ -c geo-soft.tab.cc
	mv lex.yy.c lex.yy.cc
	g++ -c lex.yy.cc

clean:
	rm -f geo-soft.tab.cc geo-soft.tab.hh lex.yy.cc lex.yy.c
