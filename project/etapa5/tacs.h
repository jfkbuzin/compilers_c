//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#ifndef TACS_HEADER
#define TACS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_GREATER 6
#define TAC_LESS 7
#define TAC_OR 8
#define TAC_LE 9
#define TAC_GE 10
#define TAC_AND 11
#define TAC_EQUAL 12
#define TAC_WHILE 13
#define TAC_IF 14
#define TAC_LABEL 15
#define TAC_COPY 16
#define TAC_BEGIN_FUN 17
#define TAC_END_FUN 18
#define TAC_READ 19
#define TAC_PRINT 20
#define TAC_RETURN 21
#define TAC_CALL_FUN 22
#define TAC_ARG 23
#define TAC_IF_ELSE 24
#define TAC_VEC_READ 25
#define TAC_VEC_WRITE 26
#define TAC_JUMP 27
#define TAC_CONCAT 28

typedef struct tac_struct
{
	int type;
	HASH_NODE *res;
	HASH_NODE *op1;
	HASH_NODE *op2;
	struct tac_struct *prev;
	struct tac_struct *next;
	int argOrder;

} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2, int argOrder);
void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC *tacJoin(TAC *l1, TAC *l2);
TAC *reverse(TAC *tac);
void tacPrintForward(TAC *tac);

//code generation

TAC *tacGenerate(AST *node, HASH_NODE *currentFun, HASH_NODE *previousFun, int argOrder);

#endif
