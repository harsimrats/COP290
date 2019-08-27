all:
	bison -d calc.y
	flex -o calc.lex.c calc.l
	gcc -o bignumcalc code.c calc.lex.c calc.tab.c -lfl -lm