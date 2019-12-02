//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define MAX_SONS 4

#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MUL 4
#define AST_DIV 5
#define AST_EQUAL 6
#define AST_GREATER 7
#define AST_LESS 8
#define AST_OR 9
#define AST_LE 10
#define AST_GE 11
#define AST_AND 12
#define AST_IF 13
#define AST_ASS 14
#define AST_LCMD 15
#define AST_WHILE 16
#define AST_PRINT 17
#define AST_READ 18
#define AST_RETURN 19
#define AST_VAR 20
#define AST_VECTOR 21
#define AST_NOT 22
#define AST_FUN 23
#define AST_CMD 24
#define AST_TYPE_INT 25
#define AST_TYPE_FLOAT 26
#define AST_TYPE_CHAR 27
#define AST_PARAM_LIST 28
#define AST_PARAM 29
#define AST_EXPR 30
#define AST_DEC 31
#define AST_COMMA 32
#define AST_PAREN 33
#define AST_VECTOR_ATT 34
#define AST_SYMBOL_VEC 35
#define AST_SYMBOL_STRING 36


typedef struct ast_node
{
	int type;
	struct ast_node *son[MAX_SONS];
	HASH_NODE *symbol;

} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3);
void astPrint(AST *node, int level);
void codePrint(FILE *f, AST *node);

#endif