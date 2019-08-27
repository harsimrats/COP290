%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>
    #include "mips.h"
    
    void yyerror(char *);
    int yylex(void);
    
    #define YYSTYPE char*
    FILE *output;
    extern FILE *yyin;
%}

%token INSTRUCTION 
%token EOL
%start exp

%%

exp: line EOL exp       {}
    | line              {}
;

line:
    INSTRUCTION         { hex_to_binary($1); lines++;}
;
%%

int main(int argc, char* argv[])
{
    char ch[10];
    
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
    //printf("aaaaaaaaaaaaaaaaaaaaaaa\n");
    start();
    
    return 0;
}

