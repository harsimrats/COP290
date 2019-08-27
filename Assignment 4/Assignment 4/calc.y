%{
	#include <stdio.h>
	
    FILE* output;
	void yyerror(const char* s);
    int yylex(void);
	extern FILE* yyin;
	#include "calc.h"
	#define YYSTYPE bignum
%}
/* declare tokens */

%start Input
%token FLOAT
%token PLUS MINUS DIV TIMES
%token EOL
%token OB CB
%token SQRT
%token LOG
%token POW
%token COMMA
%right SQRT
%left LOG
%left MINUS PLUS
%left TIMES DIV
%%

Input:
|EOL expr EOL                    {printbignum($2);}
;

expr:
    FLOAT                           {$$ = $1;}
    | expr PLUS expr                {$$ = rounding(addition($1,$3));}
    | expr MINUS expr               {$$ = rounding(subtraction($1,$3)) ; }
    | expr TIMES expr               {$$ = rounding(multiplication($1,$3));}
    | expr DIV expr                 {$$ = rounding(division($1,$3)) ;}
    | MINUS expr %prec TIMES        {$$ = negation($2) ;}
    | OB expr CB                    {$$ = $2;}
    | SQRT OB expr CB               {$$ = squareRoot($3);}
    | LOG OB expr CB                {$$ = logarithm($3);}
    | POW OB expr COMMA expr CB     {$$ = power($3, $5);}
    ;

%%

int main(int argc, char *argv[])
{
yyin=fopen("in.txt","r");
output=fopen("out.txt","w");

fscanf(yyin,"%d",&max_size);

yyparse();
return 0;
}

void yyerror(const char *s)
{
	fprintf(output, "Error: %s\n", s);
}