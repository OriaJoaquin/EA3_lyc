%{
  #include "Primitivas.h"
  #include "expression.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include "y.tab.h"

  FILE *yyin; // funcion que hace el analisis.
  int yylvan; //Variable reservada que interactua con el analizador lexico
  char* yyltext; //varible que devuelve flex

  int yyerror(void);
%}

%option noyywrap
%option yylineno

LETRA [a-zA-Z]
DIGITO [0-9]

ID        {LETRA}({LETRA}|{DIGITO})*
COMA      ","
PYC       ";"
ASIG      "="

CONSENT   [-]?[0-9]+
CONSCAD   ["].{1,30}["]

WRITE    "write"
READ    "read"
COLA    "cola"

PARA      "("
PARC      ")"
CORCA     "["
CORCC     "]"


%%


{WRITE}     {printf("WRITE: %s\n", yytext); return WRITE;}
{READ}     {printf("READ: %s\n", yytext); return READ;}
{COLA}     {printf("COLA: %s\n", yytext); return COLA;}

{ID}      {printf("ID: %s\n", yytext); return ID;}
{COMA}      {printf("COMA: %s\n", yytext); return COMA;}
{PYC}     {printf("PYC: %s\n", yytext); return PYC;}
{ASIG}     {printf("ASIG: %s\n", yytext); return ASIG;}

{CONSENT}     {printf("CONSENT: %s\n", yytext); return CONSENT;}
{CONSCAD}     {printf("CONSCAD: %s\n", yytext); return CONSCAD;}

{PARA}      {printf("PARA: %s\n", yytext); return PARA;}
{PARC}      {printf("PARC: %s\n", yytext); return PARC;}
{CORCA}     {printf("CORCA: %s\n", yytext); return CORCA;}
{CORCC}     {printf("CORCC: %s\n", yytext); return CORCC;}



"\t"
"\n"
"\n\t"
"\r\n"
" "
.   yyerror();
%%

