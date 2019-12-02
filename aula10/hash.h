//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997


typedef struct hash_node
{
	int type;
	char *text;
	struct hash_node *next;

} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashInsert(int type, char *text);
HASH_NODE *hashFind(char *text);
void hashPrint(void);

#endif