#include "assembler.h"
#include "Primitivas.h"
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

static const long hextable[] = {
    [0 ... 255] = -1, // bit aligned access into this table is considerably
    ['0'] = 0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9, // faster for most modern processors,
    ['A'] = 10,
    11,
    12,
    13,
    14,
    15, // for the space conscious, reduce to
    ['a'] = 10,
    11,
    12,
    13,
    14,
    15 // signed char.
};

void generarASM(SExpression *ast, t_cola *colaSimbolos)
{
    FILE *pArchivo;

    crearPila(&pilaIdDecisiones);
    crearPila(&pilaIdMientras);

    crearArchivo(&pArchivo);

    escribirCabecera(pArchivo);

    escribirVariables(pArchivo, colaSimbolos);

    escribirCabeceraCodeInit(pArchivo);

    escribirCodigo(pArchivo, ast);

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
            // pasar a decimal todo
            if (strcmp(itemSimbolo.tipo, "cadena") != 0)
            {
                pasarSimboloADecimal(&itemSimbolo);
            }

            fprintf(pArchivo, "\t%s\n", itemSimbolo.valor);
        }
        else
            fprintf(pArchivo, "\t%s", "?\n");
    }

    fprintf(pArchivo, "\n");
}

void pasarSimboloADecimal(QueueItem *itemSimbolo)
{
    char *endptr;

    if (strcmp(itemSimbolo->tipo, "real") == 0)
    {
        return;
    }

    if (strcmp(itemSimbolo->tipo, "entero") == 0)
    {
        sprintf(itemSimbolo->valor, "%s.0", itemSimbolo->valor);
        return;
    }

    if (strcmp(itemSimbolo->tipo, "hexadecimal") == 0)
    {
        sprintf(itemSimbolo->valor, "%ld.0", strtol(itemSimbolo->valor, &endptr, 16));
        return;
    }

    if (strcmp(itemSimbolo->tipo, "binario") == 0)
    {
        sprintf(itemSimbolo->valor, "%ld.0", strtol((itemSimbolo->valor) + 2, &endptr, 2));
        return;
    }
}

long hexdec(char *hex)
{
    hex = hex + 2;
    long ret = 0;
    while (*hex && ret >= 0)
    {
        ret = (ret << 4) | hextable[*hex++];
    }
    return ret;
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

void escribirCodigo(FILE *pArchivo, SExpression *ast)
{
    int nroIf = 0;
    int nroWhile = 0;
    int whileNroCondicion = 0;

    if (ast == NULL || esHoja(ast))
        return;

    escribirCodigo(pArchivo, ast->left);

    switch (ast->type)
    {
    case eMULTIPLICACION:
        fprintf(pArchivo, "; MULTIPLICACION\n");
        fprintf(pArchivo, "FLD _%s\n", ast->left->value);
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FMUL \n");
        fprintf(pArchivo, "FSTP @aux%d\n", contadorAux);
        fprintf(pArchivo, "ffree\n");
        sprintf(ast->value, "@aux%d", contadorAux);
        ast->left = NULL;
        ast->right = NULL;
        contadorAux++;
        break;
    case eSUMA:
        if (ast->right->type != eVALUE)
            escribirCodigo(pArchivo, ast->right);
        fprintf(pArchivo, "; SUMA\n");
        fprintf(pArchivo, "FLD _%s\n", ast->left->value);
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FADD \n");
        fprintf(pArchivo, "FSTP @aux%d\n", contadorAux);
        fprintf(pArchivo, "ffree\n");
        sprintf(ast->value, "@aux%d", contadorAux);
        ast->left = NULL;
        ast->right = NULL;
        contadorAux++;
        break;
    case eDIVISION:
        fprintf(pArchivo, "; DIVISION\n");
        fprintf(pArchivo, "FLD _%s\n", ast->left->value);
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FDIV \n");
        fprintf(pArchivo, "FSTP @aux%d\n", contadorAux);
        fprintf(pArchivo, "ffree\n");
        sprintf(ast->value, "@aux%d", contadorAux);
        ast->left = NULL;
        ast->right = NULL;
        contadorAux++;
        break;
    case eMENOS:
        if (ast->right->type != eVALUE)
            escribirCodigo(pArchivo, ast->right);
        fprintf(pArchivo, "; RESTA\n");
        fprintf(pArchivo, "FLD _%s\n", ast->left->value);
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FSUB \n");
        fprintf(pArchivo, "FSTP @aux%d\n", contadorAux);
        fprintf(pArchivo, "ffree\n");
        sprintf(ast->value, "@aux%d", contadorAux);
        ast->left = NULL;
        ast->right = NULL;
        contadorAux++;
        break;
    case eASIGNACION:
        if (ast->right->type != eVALUE)
            escribirCodigo(pArchivo, ast->right);
        fprintf(pArchivo, "; ASIGNACION\n");
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FSTP _%s\n", ast->left->value);
        fprintf(pArchivo, "ffree\n");
        ast->left = NULL;
        ast->right = NULL;
        break;
    case eLEER:
        fprintf(pArchivo, "; LEER\n");
        fprintf(pArchivo, "getString %s\n", ast->left->value);
        ast->left = NULL;
        //ast = NULL;
        break;
    case eESCRIBIR:
        fprintf(pArchivo, "; ESCRIBIR\n");
        fprintf(pArchivo, "displayString %s\n", ast->left->value);
        ast->left = NULL;
        //ast = NULL;
        break;
    case eMENOR:
        fprintf(pArchivo, "; MENOR\n");
        fprintf(pArchivo, "condicion%d:\n", contadorCondicion);
        contadorCondicion++;

        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FCOMP _%s\n", ast->left->value);
        fprintf(pArchivo, "FSTSW ax\n");
        fprintf(pArchivo, "SAHF\n");
        fprintf(pArchivo, "JNA\t");
        break;
    case eMAYOR:
        fprintf(pArchivo, "; MAYOR\n");
        fprintf(pArchivo, "condicion%d:\n", contadorCondicion);
        contadorCondicion++;

        fprintf(pArchivo, "FLD _%s\n", ast->left->value);
        fprintf(pArchivo, "FCOMP _%s\n", ast->right->value);
        fprintf(pArchivo, "FSTSW ax\n");
        fprintf(pArchivo, "SAHF\n");
        fprintf(pArchivo, "JNA\t");
        break;
    case eMENORIGUAL:
        fprintf(pArchivo, "condicion%d:\n", contadorCondicion);
        contadorCondicion++;

        fprintf(pArchivo, "; MENORIGUAL\n");
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FCOMP _%s\n", ast->left->value);
        fprintf(pArchivo, "FSTSW ax\n");
        fprintf(pArchivo, "SAHF\n");
        fprintf(pArchivo, "JNAE\t");
        break;
    case eMAYORIGUAL:
        fprintf(pArchivo, "condicion%d:\n", contadorCondicion);
        contadorCondicion++;

        fprintf(pArchivo, "; MAYORIGUAL\n");
        fprintf(pArchivo, "FLD _%s\n", ast->right->value);
        fprintf(pArchivo, "FCOMP _%s\n", ast->left->value);
        fprintf(pArchivo, "FSTSW ax\n");
        fprintf(pArchivo, "SAHF\n");
        fprintf(pArchivo, "JNAE\t");
        break;
    case eO:
        fprintf(pArchivo, "condicion%d:\n", contadorCondicion);
        contadorCondicion++;

        fprintf(pArchivo, "; OR\n");
        fprintf(pArchivo, "o_siguiente_condicion\n");
        fprintf(pArchivo, "jmp bloque_verdadero\n");
        fprintf(pArchivo, "o_siguiente_condicion:\n");
        break;
    case eY:
        fprintf(pArchivo, "condicion%d:\n", contadorCondicion);
        contadorCondicion++;

        fprintf(pArchivo, "; AND\n");
        fprintf(pArchivo, "bloque_falso\n");
        break;
    case eDECISION:
        item.id = ++contadorIf;
        nroIf = item.id;
        meterEnPila(&pilaIdDecisiones, &item);

        if (ast->right->type == eDECISIONCUERPO)
        {
            fprintf(pArchivo, "bloque_falso%d\n", nroIf);
        }
        else
        {
            fprintf(pArchivo, "endif%d\n", nroIf);
        }

        if (ast->right->type == eDECISIONCUERPO)
        {
            escribirCodigo(pArchivo, ast->right);
            ast->right = NULL;
        }
        else
        {
            escribirCodigo(pArchivo, ast->right);
            ast->right = NULL;
            fprintf(pArchivo, "endif%d:\n", nroIf);
        }

        sacarDePila(&pilaIdDecisiones, &item);
        break;
    case eDECISIONCUERPO:
        tope_de_pila(&pilaIdDecisiones, &item);
        nroIf = item.id;

        fprintf(pArchivo, "; DECISIONCUERPO\n");
        escribirCodigo(pArchivo, ast->left);
        ast->left = NULL;
        fprintf(pArchivo, "jmp endif%d\n", nroIf);

        fprintf(pArchivo, "bloque_falso%d:\n", nroIf);
        escribirCodigo(pArchivo, ast->right);
        ast->right = NULL;
        fprintf(pArchivo, "endif%d:\n", nroIf);

        break;
    case eMIENTRAS:
        nroWhile = contadorWhile++;
        whileNroCondicion = contadorCondicion - 1;

        fprintf(pArchivo, "endwhile%d\n", nroWhile);

        escribirCodigo(pArchivo, ast->right);
        fprintf(pArchivo, "jmp condicion%d\n", whileNroCondicion);
        fprintf(pArchivo, "endwhile%d:\n", nroWhile);

        // fprintf(pArchivo, "; MIENTRAS\n");
        // fprintf(pArchivo, "mientras_inicio%d:\n", nroWhile);
        // escribirCodigo(pArchivo, ast -> left);
        // ast -> left = NULL;
        // fprintf(pArchivo, "endwhile%d\n", nroWhile);
        // escribirCodigo(pArchivo, ast -> right);
        // ast -> right = NULL;
        // escribirCodigo(pArchivo, ast -> left);
        // ast -> left = NULL;
        // fprintf(pArchivo, "jmp mientras_inicio%d\n", nroWhile);
        // fprintf(pArchivo, "endwhile%d:\n", nroWhile);

        break;
    }

    escribirCodigo(pArchivo, ast->right);
}
