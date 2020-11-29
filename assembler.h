/*
 * assembler.h
 
 */
#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include "Primitivas.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void generarASM(t_listaTercetos *, t_cola *);
void crearArchivo(FILE **);
void crearArchivoIntermedia(FILE **);
void escribirCabecera(FILE *);
void escribirCabeceraCodeInit(FILE *);
void escribirVariables(FILE *, t_cola *);
void escribirCodigo(FILE *, FILE *, t_listaTercetos *);
void escribirFinal(FILE *);

extern int contadorAux;
#endif // __ASSEMBLER_H__
