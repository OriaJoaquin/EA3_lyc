%{
  #include "Primitivas.h"
  #include "expression.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "y.tab.h"

  int yystopparser = 0;
  FILE *yyin;

  char *yytext;

  SExpression* pFactor;
  SExpression* pExpresion;
  SExpression* pTermino;
  SExpression* pOperacionOPLIST;
  SExpression* pPrograma;
  SExpression* pAux;

  t_pila pilaValores;

  StackItem itemLista;

  int contadorValores = 0;
  int cantElementos = 0;
  int auxPos;
  int auxPos2;

  char auxPosStr[30];

  EOperationType operador;

  int yyerror();
  int yylex();
%}

%parse-param { SExpression **expression }
%parse-param { t_listaTercetos *lista }

%token LETRA
%token DIGITO

%token <value> ID
%token COMA
%token PYC
%token ASIG

%token <value> CONSENT
%token <value> CONSCAD

%token WRITE
%token READ
%token COLA

%token PARA
%token PARC
%token CORCA
%token CORCC

%union {
  char* value;
  SExpression *expression;
}

%type <expression> s
%type <expression> prog
%type <expression> sent
%type <expression> op_read
%type <expression> op_asig 
%type <expression> op_cola 
%type <expression> op_write
%type <expression> lista

%%
  s:
    prog {printf("\tREGLA 0: {prog} es s\n");};

  prog:
    sent {printf("\tREGLA 1: sent} es prog\n");} |
    prog sent {printf("\tREGLA 2:{prog} es sent\n");};

  sent:
    op_read {printf("\tREGLA 3.1: {op_read} es sent\n");} |
    op_write {printf("\tREGLA 3.2: {op_write} es sent\n");} |
    op_asig {printf("\tREGLA 3.3: {op_asig} es sent\n");};
  
  op_read:
    READ ID {printf("\tREGLA 4: {READ CONSENT} es op_read\n");};

  op_asig:
    ID ASIG op_cola {printf("\tREGLA 5: {ID ASIG COLA} es op_asig\n");};

  op_cola:
    COLA PARA ID PYC CORCA lista CORCC PARC {
      printf("\tREGLA 6: {COLA PARA ID PYC CORCA lista CORCC PARC} es op_cola\n");

    } |
    COLA PARA ID PYC CORCA CORCC PARC {
      printf("\tREGLA 7: {COLA PARA ID PYC CORCA lista CORCC PARC} es op_cola\n");
      yyerrorConMensaje("La lista está vacía.");
    };

  lista:
    CONSENT {
      printf("\tREGLA 8: {CONSENT} es lista\n");
      crearPila(&pilaValores);
      strcpy(itemLista.value, yytext);
      meterEnPila(&pilaValores, &itemLista);
      cantElementos++;
    }|
    lista COMA CONSENT {
      printf("\tREGLA 9: {lista CONSENT} es lista\n");
      strcpy(itemLista.value, yytext);
      meterEnPila(&pilaValores, &itemLista);
      cantElementos++;

      //Tercetos
      auxPos = insertarTerceto(lista, "CMP", "@cont", "@cantElementosSaltear");
      itoa(auxPos + 4, auxPosStr, 10);
      formatearPosicion(auxPosStr);
      insertarTerceto(lista, "BLE", auxPosStr, "");
      auxPos = insertarTerceto(lista, "+", "@acum", yytext);
      itoa(auxPos, auxPosStr, 10);
      formatearPosicion(auxPosStr);
      insertarTerceto(lista, "=", "@acum", auxPosStr);
      auxPos = insertarTerceto(lista, "+", "@cont", "1");
      itoa(auxPos, auxPosStr, 10);
      formatearPosicion(auxPosStr);
      insertarTerceto(lista, "=", "@cont", auxPosStr);
    };
  
  op_write:
    WRITE CONSCAD {printf("\tREGLA 10: {WRITE CONSCAD} es op_write\n");} |
    WRITE ID {printf("\t{REGLA 11: WRITE ID} es op_write\n");};
/*
  0.S → PROG
  1.PROG → SENT
  2.PROG → PROG SENT
  3.SENT → READ | WRITE| ASIG
  4.READ → read id
  5.ASIG →id asigna COLA
  6.COLA→ cola para id pyc ca LISTA cc parc
  7.COLA → cola para id pyc ca cc parc
  8.LISTA → cte
  9.LISTA → LISTA coma cte
  10.WRITE → write cte_s
  11.WRITE → write id
*/
%%

////////// funciones GRAPHVIZ
char* node_name(SExpression *e){
  /* return char* description of parse tree node, e.g. '+', '-', '3', etc */
  char* name = (char *)malloc(16);  /* up to 15 char descriptive node name */
  switch (e->type){
  case eVALUE:
    sprintf(name, "%s", e->value);
    return name;
  case eMULTIPLICACION:
    sprintf(name, "*");
    return name;
  case eSUMA:
    sprintf(name, "+");
    return name;
  case eRESTA:
    sprintf(name, "-");
    return name;
  case eASIGNACION:
    sprintf(name, "=");
    return name;
  case ePROGRAMA:
    sprintf(name, "PROGRAMA");
    return name;
  default:  // shouldn't get here
    sprintf(name, "???");
    return name;
  }
}

int main(int argc, char *argv[])
{
  t_listaTercetos lista;

  if(( yyin = fopen(argv[1], "rt")) == NULL)
  {
    printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
  }
  else
  {
    crearListaTercetos(&lista);

    SExpression *expression;
    yyparse(&expression, &lista);

    printf("\n\nCANTIDAD ELEMENTOS: %d", cantElementos);

    printf("\n\nPOSICIÓN TERCETO: %d", posicionTerceto);

    printf("\nTERCETOS GENERADOS: \n\n");

    imprimirListaTercetos(&lista);
  }

  fclose(yyin);
  return 0;
}

int yyerror(void)
{
  printf("Error sintáctico\n");
  exit(1);
}

int yyerrorConMensaje(char *mensaje)
{
  printf("Error sintáctico\n");
  printf("%s", mensaje);
  exit(1);
}