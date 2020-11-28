/*
 * assembler.h
 
 */
#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include "expression.h"
#include "Primitivas.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void generarASM(t_listaTercetos *, t_cola *);
void crearArchivo(FILE **);
void escribirCabecera(FILE *);
void escribirCabeceraCodeInit(FILE *);
void escribirVariables(FILE *, t_cola *);
void escribirCodigo(FILE *, SExpression *);
void escribirFinal(FILE *);
void pasarSimboloADecimal(QueueItem *);
//void imprimirArbolInOrden(SExpression *ast);

extern int contadorAux;
#endif // __ASSEMBLER_H__
