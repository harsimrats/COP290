%{

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "calc.h"
#define YYSTYPE bignum
#include "calc.tab.h"
int i=0;
int max_size;
%}
%%
"+"					{ return PLUS; }
"-"					{ return MINUS; }
"*" 				{ return TIMES; }
"/" 				{ return DIV; }
"("					{ return OB;                       }
")" 				{ return CB; }
","					{return COMMA;}
"SQRT"				{ return SQRT; }
"POW"				{ return POW;}
"LOG"				{ return LOG;}

[0-9]+|[0-9]+"."[0-9]+		{ yylval=string_to_bignum(yytext);return FLOAT;}
[\n]+          		{ return EOL; }
[ \t] 				{ /* ignore whitespace */ }
.					{ printf("SYNTAX ERROR %s", yytext); }
%%

int yywrap(void){
    return 1;
}
