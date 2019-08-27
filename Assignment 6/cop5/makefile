make:
	bison -d ass.y
	flex ass.l
	gcc -w -std=c99 ass.tab.c lex.yy.c mips.h -g -lm -o processor -lpthread
	
clear:
	rm ass.tab.c ass.tab.h processor lex.yy.c a.svg