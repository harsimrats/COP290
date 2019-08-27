%{
	#include<stdio.h>
	int flag=0;
%}

%%

\/\*					flag = 1;

\*/\					flag = 0;

(\")(.|\n)*(\")			{ ECHO ;}

(\').*(\')				{ ECHO ;}	
							
\/\/.*\n

\/\*[^"*/"]*\*\/		

.						printf("%c",yytext[0]);

\n						printf("%c",yytext[0]);

%%

int main()
{
	yylex();
	if (flag == 1)
	{
		fprintf(stderr, "Error");
		return EXIT_FAILURE;
	}
	return EXIT_FAILURE;
}

int yywrap()
{
	return 1;
}