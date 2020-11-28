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

  t_pila pilaValores;

  StackItem itemLista;

  char nameID[30];

  int contadorValores = 0;
  int cantElementos = 0;
  int auxPos;
  int auxPos2;

  char auxPosStr[30];

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
%token <value> ASIG

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
}

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
    ID {strcpy(nameID, yytext);} ASIG op_cola {
      
      printf("\tREGLA 5: {ID ASIG COLA} es op_asig\n");
      insertarTerceto(lista, "=", nameID, "@acum"); 
    };

  op_cola:
    COLA PARA ID PYC CORCA lista CORCC PARC {
      printf("\tREGLA 6: {COLA PARA ID PYC CORCA lista CORCC PARC} es op_cola\n");
      printf("VALOR RETORNO: %s\n\n", $3);
      itoa(cantElementos, auxPosStr, 10);
      insertarTerceto(lista, "=", "@cantElementos", auxPosStr);
      auxPos = insertarTerceto(lista, "CMP", nameID, "@cantElementos");
      itoa(auxPos + 4, auxPosStr, 10);
      insertarTerceto(lista, "BGE", auxPosStr, "");
      insertarTerceto(lista, "PUT", "La lista tiene menos elementos que el indicado", "");
      insertarTerceto(lista, "JMP", "fin", "");
      insertarTerceto(lista, "=", "@acum", "0");
      insertarTerceto(lista, "=", "@cont", "0");

      while(!esPilaVacia(&pilaValores))
      {
        sacarDePila(&pilaValores, &itemLista);

        //Tercetos
        auxPos = insertarTerceto(lista, "CMP", "@cont", nameID);
        itoa(auxPos + 4, auxPosStr, 10);
        formatearPosicion(auxPosStr);
        insertarTerceto(lista, "BGE", auxPosStr, "");
        auxPos = insertarTerceto(lista, "+", "@acum", itemLista.value);
        itoa(auxPos, auxPosStr, 10);
        formatearPosicion(auxPosStr);
        insertarTerceto(lista, "=", "@acum", auxPosStr);
        auxPos = insertarTerceto(lista, "+", "@cont", "1");
        itoa(auxPos, auxPosStr, 10);
        formatearPosicion(auxPosStr);
        insertarTerceto(lista, "=", "@cont", auxPosStr);
      }

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
    };
  
  op_write:
    WRITE CONSCAD {printf("\tREGLA 10: {WRITE CONSCAD} es op_write\n");} |
    WRITE ID {printf("\t{REGLA 11: WRITE ID} es op_write\n");};

%%


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