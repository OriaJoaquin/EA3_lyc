%{
  #include "Primitivas.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "y.tab.h"

  FILE *yyin; // funcion que hace el analisis.
  int yylvan; //Variable reservada que interactua con el analizador lexico
  char* yyltext; //varible que devuelve flex

  int auxInt;

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

WRITE    "WRITE"
READ    "READ"
COLA    "cola"

CONSENT   [-]?[0-9]+
CONSCAD   ["].{1,50}["]

PARA      "("
PARC      ")"
CORCA     "["
CORCC     "]"


%%


{WRITE}     {
              yylval.value = strdup(yytext);
              printf("WRITE: %s\n", yytext);
              return WRITE;
            }

{READ}      {
              yylval.value = strdup(yytext);
              printf("READ: %s\n", yytext);
              return READ;
            }

{COLA}      {
              yylval.value = strdup(yytext);
              printf("COLA: %s\n", yytext);
              return COLA;
            }

{ID}        {
              yylval.value = strdup(yytext);
              printf("ID: %s\n", yytext); 
              return ID;
            }

{COMA}      {
              yylval.value = strdup(yytext);
              printf("COMA: %s\n", yytext); 
              return COMA;
            }

{PYC}       {
              yylval.value = strdup(yytext);
              printf("PYC: %s\n", yytext);
              return PYC;
            }

{ASIG}      {
              yylval.value = strdup(yytext);
              printf("ASIG: %s\n", yytext); 
              return ASIG;
            }

{CONSENT}   {
              auxInt = atoi(yytext); 
              if(auxInt > 32767 || auxInt < 0)
              { 
                printf("La constante entera supera los limites.\n"); 
                yyerror(); 
              }
              yylval.value = strdup(yytext);
              printf("CONSENT: %s\n", yytext);
              return CONSENT;
            }

{CONSCAD}   {
              if(strlen(yytext) > 50)
              { 
                printf("La cadena supera el limite.\n"); 
                yyerror();
              }
              yylval.value = strdup(yytext);
              printf("CONSCAD: %s\n", yytext); 
              return CONSCAD;
            }

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

