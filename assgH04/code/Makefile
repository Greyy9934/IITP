CC=gcc
DEBUG=-g

a.out: main.o lex.yy.o y.tab.o
	${CC} -I. main.o lex.yy.o y.tab.o -o a.out -ll ${DEBUG}

lex.yy.o: mylex.l y.tab.h
	lex mylex.l
	${CC} -c lex.yy.c ${DEBUG}

y.tab.o y.tab.h: myyacc.y
	yacc -d myyacc.y
	${CC} -c y.tab.c -I. ${DEBUG}

main.o: main.c header.h
	${CC} -c main.c -I. ${DEBUG}

clean:
	rm -f *.o lex.yy.* y.tab.* a.out