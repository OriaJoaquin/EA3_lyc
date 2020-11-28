/*
 * expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

/**
 * @brief The operation type
 */
typedef enum tagEOperationType
{
  eVALUE,
  eASIGNACION,
  eMULTIPLICACION,
  eSUMA,
  eRESTA,
  eOPLIST,
  ePROGRAMA
} EOperationType;

/**
 * @brief The expression structure
 */
typedef struct tagSExpression
{
  EOperationType type;          ///< type of operation
  char value[30];               ///< valid only when type is eVALUE
  struct tagSExpression *left;  ///< left side of the tree
  struct tagSExpression *right; ///< right side of the tree
} SExpression;

/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
SExpression *crearHoja(char *value);

/**
 * @brief It creates an operation
 * @param type The operation type
 * @param left The left operand
 * @param right The right operand
 * @return The expression or NULL in case of no memory
 */
SExpression *crearNodo(EOperationType type,
                       SExpression *left, SExpression *right);

/**
 * @brief Deletes a expression
 * @param b The expression
 */
void imprimirArbolPosOrden(SExpression *);
EOperationType convertirLexemaEnOperacion(char *);
char *convertirOperacionEnLexema(EOperationType);
void deleteExpression(SExpression *b);
#endif // __EXPRESSION_H__
