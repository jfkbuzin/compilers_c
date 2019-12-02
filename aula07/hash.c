//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include "hash.h"

HASH_NODE *Table[HASH_SIZE];

void hashInit(void){
	int i;
	for(i=0;i<HASH_SIZE;++i){
		Table[i] = 0;
	}
}

int hashAddress(char *text){
	int i;
	int address = 1;
	for(i=0;i<strlen(text);++i){
		address = (address*text[i])%HASH_SIZE+1;
	}
	return address-1;

}


HASH_NODE *hashInsert(int type, char *text){
	HASH_NODE *newNode;
	int address;
	address = hashAddress(text);

	newNode = hashFind(text);
	if(newNode != NULL) return newNode;
	//if(newNode = hashFind(text)) return newNode;

	newNode = (HASH_NODE*)calloc(1,sizeof(HASH_NODE));
	newNode->type = type;

	newNode->text = calloc(strlen(text)+1,sizeof(char));

	strcpy(newNode->text,text);
	newNode->next = Table[address];
	Table[address] = newNode;
	return newNode;

}
HASH_NODE *hashFind(char *text){
	HASH_NODE *node;
	int i;

	for(i=0;i<HASH_SIZE;++i){
		for(node=Table[i];node;node = node->next){
			if(!strcmp(node->text,text)){
				return node;
			}
		}
	}
	return NULL;
}
void hashPrint(void){
	HASH_NODE *node;
	int i;

	for(i=0;i<HASH_SIZE;++i){
		for(node=Table[i];node;node = node->next){
			fprintf(stderr,"Table[%d] has %s\n",i,node->text);
		}
	}
}