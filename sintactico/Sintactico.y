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

  t_cola colaSimbolos;
  QueueItem itemSimbolo;

  t_pila pilaPosiciones;

  StackItem itemPosicion;

  char nameID[30];

  int cantElementos = 0;
  int auxPos;

  char auxPosStr[30];
  char auxPosStr2[30];
  char error[50];

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

%token <value> WRITE
%token <value> READ
%token COLA

%token PARA
%token PARC
%token CORCA
%token CORCC

%union {
  char* value;
  int numeroTerceto;
}

%type <numeroTerceto> sent
%type <numeroTerceto> op_asig
%type <numeroTerceto> op_read
%type <numeroTerceto> op_write
%type <numeroTerceto> prog

%%
  s:
    prog {
      printf("\tREGLA 0: {prog} es s\n");
      generarASM(lista, &colaSimbolos);
    };

  prog:
    sent {
      printf("\tREGLA 1: sent} es prog\n");
      $$ = $1;
    } |
    prog sent {
      printf("\tREGLA 2:{prog} es sent\n");
      formatearPosicion($1, auxPosStr);
      formatearPosicion($2, auxPosStr2);
      $$ = insertarTerceto(lista, "PROGRAMA", auxPosStr, auxPosStr2);
    };

  sent:
    op_read {
      printf("\tREGLA 3.1: {op_read} es sent\n");
      $$ = $1;
    } |
    op_write {
      printf("\tREGLA 3.2: {op_write} es sent\n");
      $$ = $1;
    } |
    op_asig {
      printf("\tREGLA 3.3: {op_asig} es sent\n");
      $$=$1;
    };
  
  op_read:
    READ ID {
      printf("\tREGLA 4: {READ ID} es op_read\n");
      
      //Cargo simbolo ID
      cargarItemSimboloVariable(&itemSimbolo, $2, "Entero");
      acolar(&colaSimbolos, &itemSimbolo);
      strcpy(nameID, itemSimbolo.nombre);
      
      $$ = insertarTerceto(lista, $1, itemSimbolo.nombre, "");
    };

  op_asig:
    ID ASIG op_cola {
      printf("\tREGLA 5: {ID ASIG COLA} es op_asig\n");

      //Cargo simbolo ID
      cargarItemSimboloVariable(&itemSimbolo, $1, "Entero");
      acolar(&colaSimbolos, &itemSimbolo);
      
      $$ = insertarTerceto(lista, "=", itemSimbolo.nombre, "@acum"); 
    };

  op_cola:
    COLA PARA ID PYC CORCA {
      //Cargar tabla simbolos 1
      cargarItemSimboloEntero(&itemSimbolo, "1");//1 para la comparación con la cantidad pedida de elementos
      acolar(&colaSimbolos, &itemSimbolo);  
      //Validacion ID > 1
      auxPos = insertarTerceto(lista, "CMP", nameID, itemSimbolo.nombre);
      formatearPosicion(auxPos + 4, auxPosStr);
      insertarTerceto(lista, "BGE", auxPosStr, "");
      //Apilo posicion para la etiqueta
      strcpy(itemPosicion.value, auxPosStr);
      meterEnPila(&pilaPosiciones, &itemPosicion);

      //Cargo simbolo cadena mensaje de error
      cargarItemSimboloCadena(&itemSimbolo, "\"El valor debe ser >=1\"");
      acolar(&colaSimbolos, &itemSimbolo);

      insertarTerceto(lista, "WRITE", itemSimbolo.nombre, "");
      insertarTerceto(lista, "EXIT", "", "");

      //Desapilo posicion
      sacarDePila(&pilaPosiciones, &itemPosicion);
      printf("POSICION: %s\n", itemPosicion.value);
      insertarTerceto(lista, "ETIQ", itemPosicion.value, "");
      //Calculo saltos
      auxPos =insertarTerceto(lista, "-","@cantElementos", nameID);
      formatearPosicion(auxPos, auxPosStr);
      insertarTerceto(lista,"=","@cantSaltos",auxPosStr);

      //Validacion ID < CantidadElementos
      auxPos = insertarTerceto(lista, "CMP", nameID, "@cantElementos");
      formatearPosicion(auxPos + 4, auxPosStr);
      insertarTerceto(lista, "BLE", auxPosStr, "");
      //Apilo posicion para la etiqueta
      strcpy(itemPosicion.value, auxPosStr);
      meterEnPila(&pilaPosiciones, &itemPosicion);

      //Cargo simbolo cadena mensaje de error
      cargarItemSimboloCadena(&itemSimbolo, "\"La lista tiene menos elementos que el indicado\"");
      acolar(&colaSimbolos, &itemSimbolo);

      insertarTerceto(lista, "WRITE", itemSimbolo.nombre, "");
      insertarTerceto(lista, "EXIT", "", "");



      //Desapilo posicion
      sacarDePila(&pilaPosiciones, &itemPosicion);
      printf("POSICION: %s\n", itemPosicion.value);
      insertarTerceto(lista, "ETIQ", itemPosicion.value, "");
      //Seteo acumuladores y contadores
      //insertarTerceto(lista, "=", "@acum", "_0");
     //insertarTerceto(lista, "=", "@cont", "_0");
    } lista CORCC PARC {

      printf("\tREGLA 6: {COLA PARA ID PYC CORCA lista CORCC PARC} es op_cola\n");
      
      //Cargar tabla simbolos 
      cargarItemSimboloVariableConValor(&itemSimbolo, "@cont", "0", "Entero");
      acolar(&colaSimbolos, &itemSimbolo);

      cargarItemSimboloVariableConValor(&itemSimbolo, "@acum", "0", "Entero");
      acolar(&colaSimbolos, &itemSimbolo);

      cargarItemSimboloVariableConValor(&itemSimbolo, "@cantSaltos", "0", "Entero");
      acolar(&colaSimbolos, &itemSimbolo); 

      itoa(cantElementos, auxPosStr, 10);
      cargarItemSimboloVariableConValor(&itemSimbolo, "@cantElementos", auxPosStr, "Entero");
      acolar(&colaSimbolos, &itemSimbolo); 

    } |
    COLA PARA ID PYC CORCA CORCC PARC {
      printf("\tREGLA 7: {COLA PARA ID PYC CORCA lista CORCC PARC} es op_cola\n");
      yyerrorConMensaje("La lista está vacía.");
    };

  lista:
    CONSENT {
      printf("\tREGLA 8: {CONSENT} es lista\n");
      cantElementos++;

      //Cargar tabla simbolos
      cargarItemSimboloEntero(&itemSimbolo, $1);
      acolar(&colaSimbolos, &itemSimbolo); 

      //Tercetos
        auxPos = insertarTerceto(lista, "CMP", "@cont", "@cantSaltos");
        formatearPosicion(auxPos + 4, auxPosStr);
        insertarTerceto(lista, "BLT", auxPosStr, "");

        //Apilo posicion para la etiqueta
        strcpy(itemPosicion.value, auxPosStr);
        meterEnPila(&pilaPosiciones, &itemPosicion);

        auxPos = insertarTerceto(lista, "+", "@acum", itemSimbolo.nombre);
        formatearPosicion(auxPos, auxPosStr);
        insertarTerceto(lista, "=", "@acum", auxPosStr);
        
        //Desapilo posicion para la etiqueta
        sacarDePila(&pilaPosiciones, &itemPosicion);
        
        insertarTerceto(lista, "ETIQ", itemPosicion.value, "");
        auxPos = insertarTerceto(lista, "+", "@cont", "_1");
        formatearPosicion(auxPos, auxPosStr);
        insertarTerceto(lista, "=", "@cont", auxPosStr);
    }|
    lista COMA CONSENT {
      printf("\tREGLA 9: {lista CONSENT} es lista\n");

      cantElementos++;
      //Cargar tabla simbolos
      cargarItemSimboloEntero(&itemSimbolo, $3);
      acolar(&colaSimbolos, &itemSimbolo);

      //Tercetos
        auxPos = insertarTerceto(lista, "CMP", "@cont", "@cantSaltos");
        formatearPosicion(auxPos + 4, auxPosStr);
        insertarTerceto(lista, "BLT", auxPosStr, "");
        
        //Apilo posicion para la etiqueta
        strcpy(itemPosicion.value, auxPosStr);
        meterEnPila(&pilaPosiciones, &itemPosicion);
        
        auxPos = insertarTerceto(lista, "+", "@acum", itemSimbolo.nombre);
        formatearPosicion(auxPos, auxPosStr);
        insertarTerceto(lista, "=", "@acum", auxPosStr);
        
        //Desapilo posicion para la etiqueta
        sacarDePila(&pilaPosiciones, &itemPosicion);
        
        insertarTerceto(lista, "ETIQ", itemPosicion.value, "");
        auxPos = insertarTerceto(lista, "+", "@cont", "_1");
        formatearPosicion(auxPos, auxPosStr);
        insertarTerceto(lista, "=", "@cont", auxPosStr);
    };
  
  op_write:
    WRITE CONSCAD {
      printf("\tREGLA 10: {WRITE CONSCAD} es op_write\n");

      //Cargar tabla simbolos
      cargarItemSimboloCadena(&itemSimbolo, $2);
      acolar(&colaSimbolos, &itemSimbolo);  
      
      $$ = insertarTerceto(lista, $1, itemSimbolo.nombre, "");

    } |
    WRITE ID {
      printf("\t{REGLA 11: WRITE ID} es op_write\n");
      $$ = insertarTerceto(lista, $1, $2, "");
    };

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
    crearCola(&colaSimbolos);
    crearPila(&pilaPosiciones);
    crearListaTercetos(&lista);

    SExpression *expression;
    yyparse(&expression, &lista);

    printf("\n\nCANTIDAD ELEMENTOS: %d", cantElementos);

    printf("\n\nPOSICIÓN TERCETO: %d", posicionTerceto);

    printf("\nTERCETOS GENERADOS: \n\n");

    //imprimirListaTercetos(&lista);
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