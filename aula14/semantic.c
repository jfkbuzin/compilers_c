#include "semantic.h"

int semanticErrorFlag = 0;

void setDeclaration(AST *root){
	AST *node;
	AST *dec;
	AST *type;
	AST *id;
	for(node = root;node;node = node->son[1]){
		dec = node->son[0];
		if(dec){
			switch(dec->type){
				case AST_VAR: 
					id = dec->son[1];
					if(id->symbol){
						if(id->symbol->type != SYMBOL_IDENTIFIER){
							fprintf(stderr,"Symbol %s redeclared.\n", id->symbol->text);
							semanticErrorFlag = 1;
						}
						id->symbol->type = SYMBOL_SCALAR;
						type = dec->son[0];
						switch(type->type){
							case AST_TYPE_INT: id->symbol->datatype = DATATYPE_INT; //pode ser float ou char!
							break;
							case AST_TYPE_FLOAT : id->symbol->datatype = DATATYPE_REAL; //pode ser float ou char!
							break;
							case AST_TYPE_CHAR : id->symbol->datatype = DATATYPE_CHAR; //pode ser float ou char!
							break;
							default: break;
						}
						
					}
				break;
				case AST_FUN: 
					if(dec->symbol){
						if(dec->symbol->type != SYMBOL_IDENTIFIER){
							fprintf(stderr,"Symbol %s redeclared.\n", dec->symbol->text);
							semanticErrorFlag = 1;
						}
						dec->symbol->type = SYMBOL_FUNCTION;
						dec->symbol->datatype = DATATYPE_INT;
					}
				break;
				default: break;
			}
		}
	}

}
void checkUndeclared(void){
	hashCheckUndeclared();
}