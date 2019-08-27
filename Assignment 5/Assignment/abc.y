
%{
//innermost
    #include <stdio.h>
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>
    #include "circuit.h"
    void yyerror(char *);
    int yylex(void);
    int total_nets=0;
    int MAX_SIZE;
    extern FILE* yyin;
    char* concat(char *s1,char *s2)
    {
        char *result;
        result = malloc(strlen(s1)+strlen(s2));
        strcpy(result, s1);
        strcat(result, s2);
        return result;
    }
%}
%union{
  char* str;
  double num;
  int x;
  //int n[2];
  char * terms[2];
}
%token NAME SIN OP CP UNIT POWER END DOT ENDOFFILE
%token INT
%start exprn
%%
exprn:
    line END exprn
    | line
    | 
    ;

line:
    NAME nets decimal pow units   {
                            
                            component *com = (component*)malloc(sizeof(component));
                            com->name = $<str>1;
                            com->value= $<num>3 * checkValue($<str>4);
                            com->power= $<str>4;                                        //change this
                            com->Net_Connected_to_terminal = $<terms>2[0];
                            com->Net_Connected_to_other_terminal = $<terms>2[1];
                            //printf("value of comp: %.10f\n", com->value);
                            if(!strcmp(com->power,"O"))com->toprint="";
                            else com->toprint = com->power;
                            if(checkUnitsComponent($<str>5))
                            {
                                com->type = setType($<str>5);
                            }
                            else
                            {
                                yyerror("semantic error: please check the units.");
                                return 0;
                            }
                            
                            node *n = (node*)malloc(sizeof(node));
                            n->c= com;
                            n->s = NULL;
                            n->Net_Connected_to_terminal = com->Net_Connected_to_terminal;
                            n->Net_Connected_to_other_terminal = com->Net_Connected_to_other_terminal;
                            adds(n);
                            
    }
    | NAME nets SIN OP decimal pow decimal pow decimal pow units decimal pow units damping CP {

                            source *s = (source*)malloc(sizeof(source));

                            if(strcmp($<str>11,"hz") && strcmp($<str>14,"S"))
                            {
                                yyerror("semantic error: please check the units.");
                                return 0;
                            }

                            if($<str>1[0]!='V' && $<str>1[0]!='I')
                            {
                                yyerror("semantic error: source can only be named S or V.");
                                return 0;
                            }

                            s->name=$<str>1;
                            s->type = $<str>1[0];
                            s->Net_Connected_to_terminal = $<terms>2[0];
                            s->Net_Connected_to_other_terminal = $<terms>2[1];
                            s->dcoffset= $<num>5 * checkValue($<str>6);
                            s->amplitude= $<num>7 * checkValue($<str>8);
                            s->frequency= $<num>9 * checkValue($<str>10);
                            s->delay= $<num>12 * checkValue($<str>13);
                            //printf("yes\n");
                            s->damping= $<num>15;
                            s->freq_power = $<str>10;                                    //change these
                            s->delay_power = $<str>13;                                  // change these
                            s->sin_type = $<str>3;
                           // printf("value of source: %lf\n", s->frequency);

                            if(!strcmp(s->freq_power,"O"))s->toprint[0]="";
                            else s->toprint[0] = $<str>10;
                            if(!strcmp(s->delay_power,"O"))s->toprint[1]="";
                            else s->toprint[1] = $<str>13;

                            node *n=(node*)malloc(sizeof(node));
                            n->s= s;
                            n->c = NULL;
                            n->Net_Connected_to_terminal = s->Net_Connected_to_terminal;
                            n->Net_Connected_to_other_terminal = s->Net_Connected_to_other_terminal;
                            adds(n);
                            //printf("checked source\n\n");
    }
    |END
    |error                  {/*printf("%s\n",$<str>1);*/yyerror("please provide all the components\n");return 0;}
    ;

pow:
    POWER                   {$<str>$ = $<str>1;}
    |                       {$<str>$ = "O";}
    ;
units:
    UNIT                    {$<str>$ = $<str>1;}
    |                       {$<str>$ = "O";}
    ;

nets:
    NAME NAME                {$<terms>$[0]= $<str>1; $<terms>$[1] = $<str>2;}
    | NAME INT               {
                                 if(strcmp($<str>2,"0"))
                                 {
                                    yyerror("symantic error: which net?\n");
                                    return 0;
                                }
                                 $<terms>$[0] = $<str>1; $<terms>$[1] = "0";
                            }
    | INT NAME               {
                                 if(strcmp($<str>1,"0"))
                                 {
                                    yyerror("symantic error: which net?\n");
                                    return 0;
                                }
                                 $<terms>$[1] = $<str>2; $<terms>$[0] = "0";
                            }
    ;

damping:                    {$<num>$=0;}
    | decimal               {$<num>$=$<num>1;}

decimal:
    INT                     {$<num>$=atoi($<str>1);}
    | INT DOT INT           {
                                $<num>$= atoi($<str>1) + ((double)atoi($<str>3)/(pow(10,strlen($<str>3))));
                            }
%%
void yyerror(char * s)
{
    fprintf(stderr,"%s\n",s);
}
int main(int argc, char* argv[])
{
    yyin = fopen(argv[1], "r");
    out=fopen(argv[2], "w");
    yyparse();
    fnc();
    fclose(yyin);
    return 0;
}
