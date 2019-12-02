//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "ast.h"

void setDeclaration(AST *root);
void checkUndeclared(void);
void findReturn(AST *funNode, int funType);
void checkOperands(AST *node, AST *root);
void semanticError();

#endif