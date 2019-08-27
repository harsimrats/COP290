make:
	bison -d abc.y
	flex abc.l
	gcc abc.tab.c lex.yy.c -lm -o ac_circuit_solver

clear:
	rm abc.tab.c abc.tab.h ac_circuit_solver lex.yy.c