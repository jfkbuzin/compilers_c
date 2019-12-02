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

						if(type->type == AST_TYPE_INT
							|| type->type == AST_TYPE_CHAR)
						{
							id->symbol->datatype = DATATYPE_INT;
						}
						else {
							id->symbol->datatype = DATATYPE_REAL;
						}

						/*
						switch(type->type){
							case AST_TYPE_INT: id->symbol->datatype = DATATYPE_INT; //pode ser float ou char!
							break;
							case AST_TYPE_FLOAT : id->symbol->datatype = DATATYPE_REAL; //pode ser float ou char!
							break;
							case AST_TYPE_CHAR : id->symbol->datatype = DATATYPE_CHAR; //pode ser float ou char!
							break;
							default: break;
						}*/
						
					}
				break;
				case AST_FUN: 
					if(dec->symbol){
						if(dec->symbol->type != SYMBOL_IDENTIFIER){
							fprintf(stderr,"Symbol %s redeclared.\n", dec->symbol->text);
							semanticErrorFlag = 1;
						}
						dec->symbol->type = SYMBOL_FUNCTION;

						type = dec->son[0];

						if(type->type == AST_TYPE_INT
							|| type->type == AST_TYPE_CHAR)
						{
							dec->symbol->datatype = DATATYPE_INT;
						}
						else {
							dec->symbol->datatype = DATATYPE_REAL;
						}

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

void checkOperands(AST *node){

	 int i;
	 if(node == 0) return;

	 for(i=0; i < MAX_SONS;++i)
	 	checkOperands(node->son[i]);

	 if(node->type){
		 switch(node->type){
		 	case AST_SYMBOL:
		 		if(node->symbol)
		 		{
			 		if(node->symbol->type != SYMBOL_SCALAR
			 			&& node->symbol->type != SYMBOL_LIT_INT
			 			&& node->symbol->type != SYMBOL_LIT_REAL
			 			&& node->symbol->type != SYMBOL_LIT_CHAR)
			 		{
			 			fprintf(stderr,"Identifier %s must be scalar in arithmetic expression.\n", node->symbol->text);
						semanticErrorFlag = 1;
			 		}

			 		if(node->symbol->datatype == DATATYPE_INT
			 			|| node->symbol->datatype == DATATYPE_CHAR
			 			|| node->symbol->type == SYMBOL_LIT_INT
			 			|| node->symbol->type == SYMBOL_LIT_CHAR)
			 			{
			 				node->datatype = AST_DATATYPEINT;
			 			}	 			
			 			else
			 			{
			 				node->datatype = AST_DATATYPEFLOAT;
			 			}
		 		}

		 	case AST_ADD:
		 	case AST_SUB:
		 	case AST_MUL:
		 	case AST_DIV:
		 		if(node->son[0] && node->son[1]){
		 			if(node->son[0]->type && node->son[1]->type){
				 		if(node->son[0]->type == AST_LESS
				 			|| node->son[0]->type == AST_GREATER
				 			|| node->son[0]->type == AST_OR
				 			|| node->son[0]->type == AST_LE
				 			|| node->son[0]->type == AST_GE
				 			|| node->son[0]->type == AST_AND
				 			|| node->son[0]->type == AST_OR
				 			|| node->son[1]->type == AST_LESS
				 			|| node->son[1]->type == AST_GREATER
				 			|| node->son[1]->type == AST_OR
				 			|| node->son[1]->type == AST_LE
				 			|| node->son[1]->type == AST_GE
				 			|| node->son[1]->type == AST_AND
				 			|| node->son[1]->type == AST_OR)
				 		{

				 			fprintf(stderr,"Invalid operand in arithmetic expression.\n");
							semanticErrorFlag = 1;
				 		}
				 				 			
		 			}
		 			if(node->son[0]->datatype != node->son[1]->datatype){
			 			fprintf(stderr,"Arithmetic operation cannot mix int and float data.\n");
						semanticErrorFlag = 1;
			 		}
			 		else{
			 			node->datatype = node->son[0]->datatype;
			 		}
		 		}

	 
		 	break;

		 	default: break;
		 }	 	
	 }

}