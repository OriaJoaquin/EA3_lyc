#include "assembler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int contadorAux = 0;
int contadorIf = 0;
int contadorWhile = 0;
int contadorCondicion = 0;
int ifLayer = 0;
t_pila pilaIdDecisiones;
t_pila pilaIdMientras;
StackItem item;

void generarASM(t_listaTercetos *lista, t_cola *colaSimbolos)
{
    FILE *pArchivo;

    crearPila(&pilaIdDecisiones);
    crearPila(&pilaIdMientras);

    crearArchivo(&pArchivo);

    escribirCabecera(pArchivo);

    escribirVariables(pArchivo, colaSimbolos);

    escribirCabeceraCodeInit(pArchivo);

    escribirCodigo(pArchivo, lista);

    escribirFinal(pArchivo);

    fclose(pArchivo);

    printf("\n///////////// Archivo de assembler generado. /////////////\n");
}

void crearArchivo(FILE **pArchivo)
{
    *pArchivo = fopen("Final.asm", "wb");

    if (*pArchivo == NULL)
    {
        printf("\nNo se pudo crear el archivo de assembler.\n");
        exit(1);
    }
}

void escribirCabecera(FILE *pArchivo)
{
    //fprintf(pArchivo, "include macros2.asm\n");
    //fprintf(pArchivo, "include numbers.asm\n\n");
    fprintf(pArchivo, ".MODEL LARGE\n.386\n.STACK 200h\n\n.DATA\n\n");
}

void escribirCabeceraCodeInit(FILE *pArchivo)
{
    fprintf(pArchivo, ".CODE\nMOV ax, @data\nMOV ds, ax\nFINIT; Inicializa el coprocesador\n\n");
}

void escribirVariables(FILE *pArchivo, t_cola *colaSimbolos)
{
    QueueItem itemSimbolo;

    while (!colaVacia(colaSimbolos))
    {
        desacolar(colaSimbolos, &itemSimbolo);
        //printf("ESTE ES EL VALOR PADRE: %s\n", itemSimbolo.nombre);
        fprintf(pArchivo, "%-30s\tdd", itemSimbolo.nombre);

        if (strlen(itemSimbolo.valor) > 0)
        {
            fprintf(pArchivo, "\t%s\n", itemSimbolo.valor);
        }
        else
        {
            fprintf(pArchivo, "\t%s", "?\n");
        }
    }

    fprintf(pArchivo, "\n");
}

void escribirFinal(FILE *pArchivo)
{
    fprintf(pArchivo, "\n\nFINAL:\n");
    fprintf(pArchivo, "\tmov ah, 1 ; pausa, espera que oprima una tecla:\n");
    fprintf(pArchivo, "\tint 21h ; AH=1 es el servicio de lectura\n");
    fprintf(pArchivo, "\tMOV AX, 4C00h ; Sale del Dos\n");
    fprintf(pArchivo, "\tINT 21h ; Enviamos la interripcion 21h\n");
    fprintf(pArchivo, "END ; final del archivo.\n");
}

void escribirCodigo(FILE *pArchivo, t_listaTercetos *lista)
{
}
