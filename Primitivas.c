#include "Primitivas.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int posicionTerceto = 0;
int contadorCadena = 0;

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

////////////////////////////////////////////////////////////
//Funciones cola
void crearCola(t_cola *pc)
{
    pc->pri = NULL;
    pc->ult = NULL;
}

int acolar(t_cola *pc, const QueueItem *pd)
{
    t_nodo *nue = (t_nodo *)malloc(sizeof(t_nodo));

    if (!nue)
        return 0;

    nue->dato = *pd;
    nue->sig = NULL;

    if (!pc->pri)
        pc->pri = nue;
    else
        pc->ult->sig = nue;

    pc->ult = nue;

    return 1;
}

int desacolar(t_cola *pc, QueueItem *pd)
{
    t_nodo *aux;

    if (!pc->pri)
        return 0;

    aux = pc->pri;
    *pd = aux->dato;
    pc->pri = aux->sig;

    if (!aux->sig)
        pc->ult = NULL;

    free(aux);

    return 1;
}

int colaLlena(const t_cola *pc)
{
    void *aux = malloc(sizeof(QueueItem));

    free(aux);

    return aux == NULL;
}

int colaVacia(const t_cola *pc)
{
    return pc->pri == NULL;
}

int verPrimero(const t_cola *pc, QueueItem *pd)
{
    if (!pc->pri)
        return 0;

    *pd = pc->pri->dato;
    return 1;
}
void vaciarCola(t_cola *pc)
{
    t_nodo *aux;

    while (!pc->pri)
    {
        aux = pc->pri;
        pc->pri = aux->sig;
        free(aux);
    }

    if (!pc->pri)
        pc->ult = NULL;
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

void formatearPosicion(int pos, char *posStr)
{
    char aux[30];
    itoa(pos, posStr, 10);
    aux[0] = '\0';
    strcat(aux, "[");
    strcat(aux, posStr);
    strcat(aux, "]");

    strcpy(posStr, aux);
}

char *obtenerPosicion(char *posStr)
{
    char auxNumero[6];
    int cont = 0;

    posStr++;
    while (*posStr != ']')
    {
        auxNumero[cont] = *posStr;
        posStr++;
        cont++;
    }
    auxNumero[cont] = '\0';

    strcpy(posStr, auxNumero);

    return posStr;
}

char *transformarPosicionEnAux(char *valor)
{
    char aux[30];
    if (*valor == '[')
    {
        strcpy(aux, "@aux");
        strcat(aux, obtenerPosicion(valor));
        strcpy(valor, aux);
    }
    return valor;
};

char *crearNombreVariableAux(char *nombre, int pos)
{
    char aux[30];
    itoa(pos, aux, 10);
    strcpy(nombre, "@aux");
    strcat(nombre, aux);

    return nombre;
}

void cargarItemSimboloEntero(QueueItem *item, char *value)
{
    char cadenaAuxiliar[33];

    sprintf(cadenaAuxiliar, "_%s", value);

    strcpy(item->nombre, cadenaAuxiliar);
    strcpy(item->tipo, "Entero");
    strcpy(item->valor, value);

    item->longitud = 0;
}

void cargarItemSimboloCadena(QueueItem *item, char *value)
{
    char cadenaAuxiliar[33];

    item->longitud = strlen(value);

    sprintf(cadenaAuxiliar, "_cad%d", contadorCadena);
    strcpy(item->nombre, cadenaAuxiliar);
    strcpy(item->tipo, "cadena");
    strcpy(item->valor, value);

    contadorCadena++;
}

void cargarItemSimboloVariableConValor(QueueItem *item, char *nombre, char *value, char *tipo)
{
    char cadenaAuxiliar[50];
    item->longitud = 0;

    strcpy(item->nombre, nombre);
    strcpy(item->tipo, tipo);
    strcpy(item->valor, value);
}

void cargarItemSimboloVariable(QueueItem *item, char *nombre, char *tipo)
{
    char cadenaAuxiliar[50];

    item->longitud = 0;
    strcpy(item->nombre, nombre);
    strcpy(item->tipo, tipo);
    strcpy(item->valor, "\0");
}