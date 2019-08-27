%{
    #include<stdio.h>
    #include <string.h>
    #include<stdlib.h>
    #include<stdbool.h>
    #define YYSTYPE char*
    #include "ass.tab.h"
    
    #include <ctype.h>
    #define YYSTYPE char*

%}

DIGIT [0-9a-fA-F]
EMPTY [ \t]+
EOL [ \n]+

%%

{DIGIT}{8}                  {yylval = strdup(yytext); return INSTRUCTION; }
{EOL}                       {return EOL;}
{EMPTY}                     {}
.                           {yyerror("Unknown character: syntax error");}

%%

int yywrap(void)
{
    return 1;
}