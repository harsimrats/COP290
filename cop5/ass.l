%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include<stdbool.h>
    #include "ass.tab.h"
%}

DIGIT [0-9a-fA-F]
EMPTY [ \t]+
EOL [ \n]+

%%

{DIGIT}{8}                  {yylval.str = yytext; return INSTRUCTION; }
{EOL}                       {return EOL;}
{EMPTY}                     {}
.                           {yyerror("Unknown character: syntax error");}

%%

int yywrap(void)
{return 1;}