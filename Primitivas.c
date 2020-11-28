#include "Primitivas.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int posicionTerceto = 0;

//////////////////////////////////////////////////////////////////////////////////
///PRIMITIVAS
//////////////////////////////////////////////////////////////////////////////////
void crearPila(t_pila *pp)
{
    *pp = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int meterEnPila(t_pila *pp, StackItem *pi)
{
    t_nodo_pila *pn = (t_nodo_pila *)malloc(sizeof(t_nodo_pila));
    if (!pn)
        return 0;
    pn->dato = *pi;
    pn->psig = *pp;
    *pp = pn;
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int sacarDePila(t_pila *pp, StackItem *pi)
{
    t_nodo_pila *pn;
    if (!*pp)
        return 0;
    pn = *pp;
    *pi = (*pp)->dato; //Para que guarda este dato si lo va a sacar?//Es para mostrar en un printf "se saco este dato"??
    *pp = (*pp)->psig;
    free(pn);
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int esPilaVacia(const t_pila *pp)
{
    return *pp == NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int esPilaLlena(const t_pila *pp)
{
    t_nodo_pila *pn = (t_nodo_pila *)malloc(sizeof(t_nodo_pila)); //Crea un nodo, el cual hace que sea apuntado por pn.
    free(pn);                                                     //Si hay memoria, el puntero pn va a existir, por ende libera la memoria para no desperdiciarla.
    return pn == NULL;                                            //Pregunta si el puntero pn se creo.
}
//////////////////////////////////////////////////////////////////////////////////
int tope_de_pila(const t_pila *pp, StackItem *pi)
{
    if (!*pp)
        return 0;
    *pi = (*pp)->dato;
    return 0;
}

void vaciarPila(t_pila *pp)
{
    t_nodo_pila *pn;
    while (*pp)
    {
        pn = *pp;
        *pp = (*pp)->psig;
        printf("\n%s\n", pn->dato.value);
        free(pn);
    }
}

//FUNCIONES LISTA
void crearListaTercetos(t_listaTercetos *pTerceto)
{

    *pTerceto = NULL;
}

int insertarTerceto(t_listaTercetos *pTerceto, char *contenido1, char *contenido2, char *contenido3)
{
    t_nodoTerceto *nuevoNodo = (t_nodoTerceto *)malloc(sizeof(t_nodoTerceto));

    if (!nuevoNodo)
    {
        return 0; //SIN MEMORIA
    }

    strcpy(nuevoNodo->info.pos1, contenido1);
    strcpy(nuevoNodo->info.pos2, contenido2);
    strcpy(nuevoNodo->info.pos3, contenido3);
    nuevoNodo->info.posicion = posicionTerceto++;
    nuevoNodo->psig = NULL;

    while (*pTerceto)
    {
        pTerceto = &(*pTerceto)->psig;
    }

    *pTerceto = nuevoNodo;
    return nuevoNodo->info.posicion;
}

void imprimirListaTercetos(t_listaTercetos *pTerceto)
{
    t_nodoTerceto *nuevoNodo;

    while (*pTerceto)
    {
        nuevoNodo = *pTerceto;
        printf("[%d](%s,%s,%s)\n", nuevoNodo->info.posicion, nuevoNodo->info.pos1, nuevoNodo->info.pos2, nuevoNodo->info.pos3);
        pTerceto = &(*pTerceto)->psig;
        free(nuevoNodo);
    }
}

void formatearPosicion(char *posStr)
{
    char aux[30];
    aux[0] = '\0';
    strcat(aux, "[");
    strcat(aux, posStr);
    strcat(aux, "]");

    strcpy(posStr, aux);
}