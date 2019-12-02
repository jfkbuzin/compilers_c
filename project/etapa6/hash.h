//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_REAL 2
#define SYMBOL_LIT_CHAR 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_IDENTIFIER 5
#define SYMBOL_SCALAR 6 
#define SYMBOL_VECTOR 7
#define SYMBOL_FUNCTION 8
#define SYMBOL_LABEL 9

#define DATATYPE_INT 10
#define DATATYPE_REAL 11
#define DATATYPE_CHAR 12

typedef struct hash_node
{
	int type;
	int datatype;
	int asttype;
	int astid;
	char *text;
	char *paramfun;
	int paramorder;
	struct hash_node *next;

} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashInsert(int type, char *text);
HASH_NODE *hashFind(char *text);
void hashPrint(void);
void hashCheckUndeclared(void);
HASH_NODE *makeTemp();
HASH_NODE *makeLabel();

#endif