/*
 * expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "expression.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Allocates space for expression
 * @return The expression or NULL if not enough memory
 */
static SExpression *allocateExpression()
{
  SExpression *b = (SExpression *)malloc(sizeof(SExpression));
  if (b == NULL)
    return NULL;
  b->type = eVALUE;
  b->left = NULL;
  b->right = NULL;
  return b;
}

SExpression *crearHoja(char *value)
{
  printf("(hoja creada: %s)\n", value);
  SExpression *b = allocateExpression();
  if (b == NULL)
    return NULL;
  b->type = eVALUE;
  strcpy(b->value, value);

  return b;
}

SExpression *crearNodo(EOperationType type,
                       SExpression *left, SExpression *right)
{
  SExpression *b = allocateExpression();
  if (b == NULL)
    return NULL;
  b->type = type;
  b->left = left;
  b->right = right;
  return b;
}

void deleteExpression(SExpression *b)
{
  if (b == NULL)
    return;
  deleteExpression(b->left);
  deleteExpression(b->right);
  free(b);
}

EOperationType convertirLexemaEnOperacion(char *lexema)
{
  if (!strcmp("+", lexema))
    return eSUMA;

  if (!strcmp("*", lexema))
    return eMULTIPLICACION;
}

char *convertirOperacionEnLexema(EOperationType operacion)
{
  if (operacion == eSUMA)
    return "+";

  if (operacion == eMULTIPLICACION)
    return "*";
}

void imprimirArbolPosOrden(SExpression *arbol)
{
  if (arbol)
  {
    imprimirArbolPosOrden(arbol->left);
    imprimirArbolPosOrden(arbol->right);
    if (arbol->type == eVALUE)
      printf("%s ", arbol->value);
    else
    {
      switch (arbol->type)
      {
      case eMULTIPLICACION:
        printf("* ");
        break;
      case eSUMA:
        printf("+ ");
        break;
      case eRESTA:
        printf("- ");
        break;
      case eASIGNACION:
        printf("= ");
        break;
      case ePROGRAMA:
        printf("PROGRAMA ");
        break;
      default: // shouldn't get here
        printf("? ");
        break;
      }
    }
  }
}