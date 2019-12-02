#include "semantic.h"

int semanticErrorFlag = 0;
int correctReturn = 0;

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
							fprintf(stderr,"Symbol %s redeclared, error on line %d.\n", id->symbol->text, id->linenumber);
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
							fprintf(stderr,"Symbol %s redeclared, error on line %d.\n", dec->symbol->text, dec->linenumber);
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

void findReturn(AST *funNode, int funType){
	int i;
	if(funNode == 0) return;

	for(i=0; i < MAX_SONS;++i)
	 	findReturn(funNode->son[i], funType);
	 if(funNode->type){
	 	if(funNode->type == AST_RETURN){
	 		correctReturn = 1;
	 		if(funNode->son[0]->symbol){
		 		if(funNode->son[0]->symbol->type == SYMBOL_SCALAR){
		 			//returns a variable
		 			if(funType == AST_TYPE_FLOAT && funNode->son[0]->symbol->datatype == DATATYPE_INT){
			 			
			 			fprintf(stderr,"Cannot return non float value in float function, error on line %d.\n", funNode->linenumber);
						semanticErrorFlag = 1;	
			 		} else {
			 			if((funType == AST_TYPE_INT
				 			|| funType == AST_TYPE_CHAR) && 
				 				funNode->son[0]->symbol->datatype == DATATYPE_REAL)
				 				{
				 					fprintf(stderr,"Cannot return float value in non float function, error on line %d.\n", funNode->linenumber);
									semanticErrorFlag = 1;				
				 				}
			 		}
		 		} else {
		 			//returns a literal
			 		if(funType == AST_TYPE_FLOAT && funNode->son[0]->symbol->type != SYMBOL_LIT_REAL){
			 			
			 			fprintf(stderr,"Cannot return non float value in float function, error on line %d.\n", funNode->linenumber);
						semanticErrorFlag = 1;	
			 		}
			 		else{

			 			if((funType == AST_TYPE_INT
			 			|| funType == AST_TYPE_CHAR) && 
			 				funNode->son[0]->symbol->type == SYMBOL_LIT_REAL)
			 				{
			 					fprintf(stderr,"Cannot return float value in non float function, error on line %d.\n", funNode->linenumber);
								semanticErrorFlag = 1;				
			 				}

			 		}
		 		}
	 		}
	 	}
	 }
}
/*
//tem que rodar depois do checkOperands
void checkParams(AST *paramNode, char * function){
	int i;
	int order = 1;
	if(node == 0) return;

	for(i=0; i < MAX_SONS;++i)
	 	checkParams(paramNode->son[i], symbol);

	HASH_NODE *var;
	if(paramNode->type == AST_SYMBOL
	 	|| paramNode->type == AST_SYMBOL_VEC){

		if(paramNode->datatype == AST_DATATYPEINT)
	 		var = hashFind(paramNode->symbol->text);
	 		if(var->astid == AST_PARAM){
	 			if(strcmp(var->paramfun,function) == 0)
	 			//conferir lista de parametros da funcao
	 			//conferir se funcao retorna o tipo compativel com a atribuicao
	 			
	 		}	
	}
}*/

void checkOperands(AST *node){

	 int i;
	 if(node == 0) return;

	 for(i=0; i < MAX_SONS;++i)
	 	checkOperands(node->son[i]);

	 HASH_NODE *var;
	 if(node->type){

		 switch(node->type){
		 	case AST_SYMBOL:
		 	case AST_CALL:
		 	case AST_SYMBOL_VEC:
		 		if(node->symbol)
		 		{
			 		if(node->symbol->type != SYMBOL_SCALAR
			 			&& node->symbol->type != SYMBOL_FUNCTION
			 			&& node->symbol->type != SYMBOL_LIT_INT
			 			&& node->symbol->type != SYMBOL_LIT_REAL
			 			&& node->symbol->type != SYMBOL_LIT_CHAR)
			 		{
			 			fprintf(stderr,"Identifier %s must be scalar in arithmetic expression, error on line %d.\n", 
			 				node->symbol->text, node->linenumber);
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
			 				//undeclared variables will also have type float, which may lead to more than one semantic error
			 				node->datatype = AST_DATATYPEFLOAT;
			 			}


			 		if(node->type == AST_SYMBOL_VEC){
			 			if(node->son[0]->symbol->type == SYMBOL_LIT_REAL){
			 				fprintf(stderr,"index cannot be of float type, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
			 			}
			 		}	
			 		var = hashFind(node->symbol->text);	
			 		if(var != NULL){
			 			if((node->type == AST_SYMBOL || node->type == AST_CALL) && var->astid == AST_VECTOR){
				 			fprintf(stderr,"Vector must have index set at call, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}
				 		if(node->type == AST_SYMBOL_VEC && var->astid == AST_VAR){
				 			fprintf(stderr,"This variable is not a vector, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}
			 		
			 		} else {
			 			//should not get here
			 			fprintf(stderr,"Var is undeclared.\n");
			 		}

		 		}
		 	break;
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
				 			|| node->son[0]->type == AST_LESS
				 			|| node->son[1]->type == AST_LESS
				 			|| node->son[1]->type == AST_GREATER
				 			|| node->son[1]->type == AST_OR
				 			|| node->son[1]->type == AST_LE
				 			|| node->son[1]->type == AST_GE
				 			|| node->son[1]->type == AST_AND
				 			|| node->son[1]->type == AST_OR
				 			|| node->son[1]->type == AST_COMMA_EXPR)
				 		{

				 			fprintf(stderr,"Invalid operand in arithmetic expression, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}
				 				 			
		 			}
		 			if(node->son[0]->datatype != node->son[1]->datatype){
			 			fprintf(stderr,"Arithmetic operation cannot mix int and float data, error on line %d.\n", node->linenumber);
						semanticErrorFlag = 1;
			 		}
			 		else{
			 			node->datatype = node->son[0]->datatype;
			 		}
		 		}
		 	break;

		 	case AST_ASS:
		 	case AST_ASS_ATT:
		 	case AST_VECTOR:


		 		if(node->type == AST_VECTOR){
		 			if(node->son[0]->symbol->type == SYMBOL_LIT_REAL){
		 				fprintf(stderr,"index cannot be of float type, error on line %d.\n", node->linenumber);
						semanticErrorFlag = 1;
		 			}
		 		}	
		 		var = hashFind(node->symbol->text);	
			 	if(var != NULL){

			 		if((node->type == AST_ASS || node->type == AST_ASS_ATT) && var->astid == AST_VECTOR){
			 			fprintf(stderr,"Vector must have index set at call, error on line %d.\n", node->linenumber);
						semanticErrorFlag = 1;
			 		}

			 		if(node->type == AST_VECTOR && var->astid == AST_VAR){
			 			fprintf(stderr,"Vector must have index set at call, error on line %d.\n", node->linenumber);
						semanticErrorFlag = 1;
			 		}

			 		if(node->type == AST_ASS || node->type == AST_ASS_ATT){
			 			if((var->asttype == AST_TYPE_INT || var->asttype == AST_TYPE_CHAR) 
				 			&& node->son[0]->datatype != AST_DATATYPEINT){
				 			
				 			fprintf(stderr,"Cannot assign non integer value to integer variable, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}
				 		if(var->asttype == AST_TYPE_FLOAT 
				 			&& node->son[0]->datatype != AST_DATATYPEFLOAT){
				 			
				 			fprintf(stderr,"Cannot assign non float value to float variable, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}


				 	}
			 		if(node->type == AST_VECTOR && node->son[1]){
			 			if((var->asttype == AST_TYPE_INT || var->asttype == AST_TYPE_CHAR) 
				 			&& node->son[1]->datatype != AST_DATATYPEINT){
				 			
				 			fprintf(stderr,"Cannot assign non integer value to integer variable, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}
				 		if(var->asttype == AST_TYPE_FLOAT 
				 			&& node->son[1]->datatype != AST_DATATYPEFLOAT){
				 			
				 			fprintf(stderr,"Cannot assign non float value to float variable, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;
				 		}
			 		}

			 	}
			 	if(node->son[0] && node->son[0]->symbol){
				 	var = hashFind(node->son[0]->symbol->text);
			 		if(var->astid == AST_FUN){
			 			//conferir lista de parametros da funcao
			 			//conferir se funcao retorna o tipo compativel com a atribuicao
			 			
			 		}			 		
			 	}
			 	if(node->son[1] && node->son[1]->symbol){
				 	var = hashFind(node->son[1]->symbol->text);
			 		if(var->astid == AST_FUN){
			 			//conferir lista de parametros da funcao
			 			//conferir se funcao retorna o tipo compativel com a atribuicao
			 			
			 		}			 		
			 	}
	

			break;
			case AST_VECTOR_ATT:
				if(node->son[0]->symbol->type == SYMBOL_LIT_REAL){
	 				fprintf(stderr,"index cannot be of float type, error on line %d.\n", node->linenumber);
					semanticErrorFlag = 1;
		 		}
			break;
			case AST_IF:
			case AST_WHILE:
			case AST_RETURN:
				if(node->son[0]->type == AST_SYMBOL_STRING){
					fprintf(stderr,"Cannot assign string to this command, error on line %d.\n", node->son[0]->linenumber);
					semanticErrorFlag = 1;
				}
			break;

			case AST_FUN: findReturn(node, node->symbol->asttype);
				if(!correctReturn){
					fprintf(stderr,"Return command not found inside function, return missing on function that ends on line %d.\n", node->linenumber);
					semanticErrorFlag = 1;					
				}
				correctReturn = 0;
			break;
		 	default: break;
		 }	 	
	 }

	 
}

void semanticError(){
	if(semanticErrorFlag){
		fprintf(stderr, "Semantic errors found.\n");
		exit(4); 
	} 
}
