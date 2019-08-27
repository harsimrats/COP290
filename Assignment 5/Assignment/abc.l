
%{
	 #include "abc.tab.h"
    #include <stdio.h>
    #include <string.h>
    void yyerror(char *);
    int i = 0;
%}

%s SOURCE
%s COMPONENT
%s NETCHECK
%s NETCHECK2
%s TYPECHECK
%s UNITS

DIGIT [0-9]
ALPHANUM [A-Za-z][A-Za-z0-9_]*
POWERS (K|M|N|F|P|U|"MEG")

%%
<INITIAL>{ALPHANUM}			{yylval.str=strdup(yytext);BEGIN NETCHECK;return NAME;}
<NETCHECK>{ALPHANUM}			{yylval.str=strdup(yytext);BEGIN NETCHECK2;return NAME;}
<NETCHECK2>{ALPHANUM}			{yylval.str=strdup(yytext);BEGIN TYPECHECK;return NAME;}
<NETCHECK2>"0"					{yylval.str=strdup(yytext);BEGIN TYPECHECK;return INT;}
<TYPECHECK>{DIGIT}+			{yylval.str=strdup(yytext);BEGIN COMPONENT;return INT;}
<TYPECHECK>"SINE"	{yylval.str=strdup(yytext);BEGIN SOURCE;return SIN;}
"(" 					{return OP;}
")" 					{return CP;}
"hz"|F|H|S				{yylval.str=strdup(yytext);BEGIN UNITS;return UNIT;}
<UNITS>{POWERS}		{yylval.str=strdup(yytext);return POWER;}
{POWERS}			{yylval.str=strdup(yytext);return POWER;}
{DIGIT}+			{yylval.str=strdup(yytext);return INT;}
\.								{return DOT;}
[ \t]+ 							{}
"\n" 							{BEGIN INITIAL;return END;}
.								{yyerror("unknown chracter: syntax error");}
%%
int yywrap(void)
{
    return 1;
}