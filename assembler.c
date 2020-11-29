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
    FILE *pArchivoIntermedia;

    crearPila(&pilaIdDecisiones);
    crearPila(&pilaIdMientras);

    crearArchivo(&pArchivo);
    crearArchivoIntermedia(&pArchivoIntermedia);

    escribirCabecera(pArchivo);

    escribirVariables(pArchivo, colaSimbolos);

    escribirCabeceraCodeInit(pArchivo);

    escribirCodigo(pArchivo, pArchivoIntermedia, lista);

    escribirFinal(pArchivo);

    fclose(pArchivo);
    fclose(pArchivoIntermedia);

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

void crearArchivoIntermedia(FILE **pArchivo)
{
    *pArchivo = fopen("intermedia.txt", "wb");

    if (*pArchivo == NULL)
    {
        printf("\nNo se pudo crear el archivo de assembler.\n");
        exit(1);
    }
}

void escribirCabecera(FILE *pArchivo)
{
    fprintf(pArchivo, "include macros2.asm\n");
    fprintf(pArchivo, "include number.asm\n\n");
    fprintf(pArchivo, ".model large\n.386\n.stack 200h\n\n.DATA\n\n");
}

void escribirCabeceraCodeInit(FILE *pArchivo)
{
    fprintf(pArchivo, ".CODE\nSTART:\n\tmov AX,@DATA\n\tmov DS,AX\n\tmov ES,AX\n\n");
}

void escribirVariables(FILE *pArchivo, t_cola *colaSimbolos)
{
    QueueItem itemSimbolo;

    while (!colaVacia(colaSimbolos))
    {
        desacolar(colaSimbolos, &itemSimbolo);
        //printf("ESTE ES EL VALOR PADRE: %s\n", itemSimbolo.nombre);
        if (itemSimbolo.longitud > 0) //Es una cadena
        {
            fprintf(pArchivo, "%-30s\tdb", itemSimbolo.nombre);
        }
        else
        {
            fprintf(pArchivo, "%-30s\tdd", itemSimbolo.nombre);
        }

        if (strlen(itemSimbolo.valor) > 0)
        {
            fprintf(pArchivo, "\t%s", itemSimbolo.valor);

            if (itemSimbolo.longitud > 0) //Es una cadena
            {
                fprintf(pArchivo, ",'$', %d dup (?)", itemSimbolo.longitud);
            }
            fprintf(pArchivo, "\n");
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
    fprintf(pArchivo, "END START; final del archivo.\n");
}

void escribirCodigo(FILE *pArchivo, FILE *pArchivoIntermdia, t_listaTercetos *lista)
{
    t_nodoTerceto *nuevoNodo;

    while (*lista)
    {
        nuevoNodo = *lista;

        printf("[%d](%s,%s,%s)\n", nuevoNodo->info.posicion, nuevoNodo->info.pos1, nuevoNodo->info.pos2, nuevoNodo->info.pos3);
        fprintf(pArchivoIntermdia, "[%d](%s,%s,%s)\n", nuevoNodo->info.posicion, nuevoNodo->info.pos1, nuevoNodo->info.pos2, nuevoNodo->info.pos3);

        //printf("NODO: %s\n", nuevoNodo->info.pos1);

        if (!strcmp(nuevoNodo->info.pos1, "READ"))
        {
            fprintf(pArchivo, "; LEER\n");
            fprintf(pArchivo, "GetInteger %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "newLine 1\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "WRITECAD"))
        {
            fprintf(pArchivo, "; ESCRIBIR CAD\n");
            fprintf(pArchivo, "displayString %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "newLine 1\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "WRITEID"))
        {
            fprintf(pArchivo, "; ESCRIBIR ID\n");
            fprintf(pArchivo, "displayInteger %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "newLine 1\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "CMP"))
        {
            fprintf(pArchivo, "; COMPARACION\n");
            fprintf(pArchivo, "FLD %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "FCOMP %s\n", nuevoNodo->info.pos3);
            fprintf(pArchivo, "FSTSW ax\n");
            fprintf(pArchivo, "SAHF\n");
            fprintf(pArchivo, "FFREE\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "BGE"))
        {
            fprintf(pArchivo, "JAE _etiq%s\n", obtenerPosicion(nuevoNodo->info.pos2));
        }
        else if (!strcmp(nuevoNodo->info.pos1, "BLE"))
        {
            fprintf(pArchivo, "JNA _etiq%s\n", obtenerPosicion(nuevoNodo->info.pos2));
        }
        else if (!strcmp(nuevoNodo->info.pos1, "BLT"))
        {
            fprintf(pArchivo, "JB _etiq%s\n", obtenerPosicion(nuevoNodo->info.pos2));
        }
        else if (!strcmp(nuevoNodo->info.pos1, "EXIT"))
        {
            fprintf(pArchivo, "JMP FINAL\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "ETIQ"))
        {
            fprintf(pArchivo, "_etiq%s:\n", obtenerPosicion(nuevoNodo->info.pos2));
        }
        else if (!strcmp(nuevoNodo->info.pos1, "+"))
        {
            transformarPosicionEnAux(nuevoNodo->info.pos2);
            transformarPosicionEnAux(nuevoNodo->info.pos3);
            fprintf(pArchivo, "; SUMA\n");
            fprintf(pArchivo, "FLD %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "FLD %s\n", nuevoNodo->info.pos3);
            fprintf(pArchivo, "FADD \n");
            fprintf(pArchivo, "FSTP @aux%d\n", nuevoNodo->info.posicion);
            fprintf(pArchivo, "FFREE\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "-"))
        {
            transformarPosicionEnAux(nuevoNodo->info.pos2);
            transformarPosicionEnAux(nuevoNodo->info.pos3);
            fprintf(pArchivo, "; RESTA\n");
            fprintf(pArchivo, "FLD %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "FLD %s\n", nuevoNodo->info.pos3);
            fprintf(pArchivo, "FSUB \n");
            fprintf(pArchivo, "FSTP @aux%d\n", nuevoNodo->info.posicion);
            fprintf(pArchivo, "FFREE\n");
        }
        else if (!strcmp(nuevoNodo->info.pos1, "="))
        {
            transformarPosicionEnAux(nuevoNodo->info.pos2);
            transformarPosicionEnAux(nuevoNodo->info.pos3);
            fprintf(pArchivo, "; ASIGNACION\n");
            fprintf(pArchivo, "FLD %s\n", nuevoNodo->info.pos3);
            fprintf(pArchivo, "FSTP %s\n", nuevoNodo->info.pos2);
            fprintf(pArchivo, "FFREE\n");
        }

        lista = &(*lista)->psig;
        free(nuevoNodo);
    }
}
