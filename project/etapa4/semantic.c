#include "semantic.h"

int semanticErrorFlag = 0;
int correctReturn = 0;
int idIsParameter = 0;
int paramCountCall = 0;
int paramCountFun = 0;
int idType;

//etra funcions prototypes
void checkSymbolParam(AST *paramNode, char * id);
void findParamCountOnFun(AST *root, char * function);
void findParamCountOnCall(AST *node); 
void findIdType(AST *root, char * id);
void findParamTypeOnFun(AST *root, char * function, int paramOrder, int lineNum);

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
						if(id->type == AST_ASS) id->symbol->type = SYMBOL_SCALAR;
						else id->symbol->type = SYMBOL_VECTOR;

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

void checkOperands(AST *node, AST *root){

	 int i;
	 if(node == 0) return;

	 for(i=0; i < MAX_SONS;++i)
	 	checkOperands(node->son[i],root);

	 
	 HASH_NODE *var;
	 AST *search;
	 if(node->type){

		 switch(node->type){
		 	case AST_SYMBOL:
		 	case AST_CALL:
		 	case AST_SYMBOL_VEC:
		 		if(node->symbol)
		 		{
			 		if(node->symbol->type != SYMBOL_SCALAR
			 			&& node->symbol->type != SYMBOL_VECTOR
			 			&& node->symbol->type != SYMBOL_FUNCTION
			 			&& node->symbol->type != SYMBOL_LIT_INT
			 			&& node->symbol->type != SYMBOL_LIT_REAL
			 			&& node->symbol->type != SYMBOL_LIT_CHAR)
			 		{
			 			//if symbol nao existe num parâmetro da função
			 			search = root;
			 			checkSymbolParam(search, node->symbol->text);
			 			if(!idIsParameter)
			 			{
				 			fprintf(stderr,"Identifier %s must be scalar in arithmetic expression, error on line %d.\n", 
				 				node->symbol->text, node->linenumber);
							semanticErrorFlag = 1;	
								 				
			 			}
			 			
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
			 				//variables declared in functions will always be int
			 				if(idIsParameter) node->datatype = AST_DATATYPEINT;
			 				else node->datatype = AST_DATATYPEFLOAT;
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
			 		idIsParameter = 0;

			 		//verificando call de funcoes
			 		if(node->type == AST_CALL)
			 		{
				 		//1: se é quantidade correta de parametros(0,1,2 ...)

			 			search = root;
			 			findParamCountOnFun(search, node->symbol->text);
			 			if(node->son[0])
				 			findParamCountOnCall(node); 

				 		if(paramCountCall != paramCountFun)
				 		{
				 			fprintf(stderr,"Number of parameters on call does not match function, error on line %d.\n", node->linenumber);
							semanticErrorFlag = 1;				 			
				 		} 
				 		else
				 		{
					 		if(paramCountCall > 0){
						 		//2: se os tipos estao certos
						 		if(node->son[0]->type == AST_SYMBOL || node->son[0]->type == AST_SYMBOL_VEC)
						 		{
						 			//somente 1 parametro
					 				if(node->son[0]->symbol->type != SYMBOL_LIT_INT 
					 					&& node->son[0]->symbol->type != SYMBOL_LIT_REAL
					 					&& node->son[0]->symbol->type != SYMBOL_LIT_CHAR)
					 					findIdType(root, node->son[0]->symbol->text);
					 				else
					 				{
					 					if(node->son[0]->symbol->type == SYMBOL_LIT_INT) idType = AST_TYPE_INT;
					 					if(node->son[0]->symbol->type == SYMBOL_LIT_CHAR) idType = AST_TYPE_CHAR;
					 					if(node->son[0]->symbol->type == SYMBOL_LIT_REAL) idType = AST_TYPE_FLOAT;
					 				}
						 			findParamTypeOnFun(root, node->symbol->text, 1, node->linenumber);
						 		}
						 		else
						 		{
							 		if(node->son[0]->type == AST_COMMA)
							 		{
							 			//mais de 1 parametro
							 			search = node->son[0];
							 			int order = 0;
							 			while(search->type == AST_COMMA)
							 			{
							 				order++;
							 				if(search->son[0]->symbol->type != SYMBOL_LIT_INT 
							 					&& search->son[0]->symbol->type != SYMBOL_LIT_REAL
							 					&& search->son[0]->symbol->type != SYMBOL_LIT_CHAR)
							 					findIdType(root, search->son[0]->symbol->text);
							 				else
							 				{
							 					if(search->son[0]->symbol->type == SYMBOL_LIT_INT) idType = AST_TYPE_INT;
							 					if(search->son[0]->symbol->type == SYMBOL_LIT_CHAR) idType = AST_TYPE_CHAR;
							 					if(search->son[0]->symbol->type == SYMBOL_LIT_REAL) idType = AST_TYPE_FLOAT;
							 				}
						 					findParamTypeOnFun(root, node->symbol->text, order, node->linenumber);

						 					if(search->son[1]->type == AST_SYMBOL || search->son[1]->type == AST_SYMBOL_VEC )
						 					{
						 						order++;

						 						if(search->son[1]->symbol->type != SYMBOL_LIT_INT 
							 					&& search->son[1]->symbol->type != SYMBOL_LIT_REAL
							 					&& search->son[1]->symbol->type != SYMBOL_LIT_CHAR)
							 					findIdType(root, search->son[1]->symbol->text);
								 				else
								 				{
								 					if(search->son[1]->symbol->type == SYMBOL_LIT_INT) idType = AST_TYPE_INT;
								 					if(search->son[1]->symbol->type == SYMBOL_LIT_CHAR) idType = AST_TYPE_CHAR;
								 					if(search->son[1]->symbol->type == SYMBOL_LIT_REAL) idType = AST_TYPE_FLOAT;
								 				}
						 						findParamTypeOnFun(root, node->symbol->text, order, node->linenumber);
						 					}
						 					search = search->son[1];
							 			}
							 		}						 			
						 		}						 		
						 	}
			 			
				 		}

		 				paramCountCall = 0;
						paramCountFun = 0;
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
		 			if(node->son[0]->datatype == AST_DATATYPEFLOAT){
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
					fprintf(stderr,"Warning: return command not found inside function, return missing on function that ends on line %d.\n", node->linenumber);					
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

void checkSymbolParam(AST *paramNode, char * id)
{
	int i;
	if(paramNode == 0) return;

	for(i=0; i < MAX_SONS;++i)
	 	checkSymbolParam(paramNode->son[i],id);


	if(paramNode->type){
		switch(paramNode->type){
			case AST_PARAM: if(!strcmp(paramNode->symbol->text, id)) idIsParameter = 1;
			break;
		 	default: break;			
		}
	}

}
void findParamCountOnFun(AST *root, char * function)
{
	int i;
	if(root == 0) return;

	for(i=0; i < MAX_SONS;++i)
	 	findParamCountOnFun(root->son[i],function);

	if(root->type){
		switch(root->type){
			case AST_FUN: 
				if(!strcmp(root->symbol->text, function))
				{
					root = root->son[1]->son[0];
					while(root != 0)
					{
						paramCountFun = root->symbol->paramorder;
						root = root->son[1];
					}
				}
			 
			break;
		 	default: break;			
		}
	}
}
void findParamCountOnCall(AST *node)
{
	if(node->son[0]->type == AST_SYMBOL || node->son[0]->type == AST_SYMBOL_VEC)
	{
		paramCountCall = 1;
	}
	else
	{
		if(node->son[0]->type == AST_COMMA)
		{
			node = node->son[0];
			int count = 0;
			while(node->type == AST_COMMA)
			{
				if(node->son[0] != 0 && (node->son[0]->type == AST_SYMBOL || node->son[0]->type == AST_SYMBOL_VEC))
				{
					count++;
				}
				if(node->son[1] != 0 && (node->son[1]->type == AST_SYMBOL || node->son[1]->type == AST_SYMBOL_VEC))
				{
					count++;
				}
				node = node->son[1];
			}
			paramCountCall = count;
		}
	}

}

void findIdType(AST *root, char * id)
{
	int i;
	if(root == 0) return;

	for(i=0; i < MAX_SONS;++i)
	 	findIdType(root->son[i],id);

	if(root->type){
		switch(root->type){
			case AST_VAR: 
				if(!strcmp(root->son[1]->symbol->text, id))
				{

					if(root->son[0]->type == AST_TYPE_INT) idType = AST_TYPE_INT;
						
					if(root->son[0]->type == AST_TYPE_FLOAT) idType = AST_TYPE_FLOAT;
							
					if(root->son[0]->type == AST_TYPE_CHAR) idType = AST_TYPE_CHAR;

					//fprintf(stderr, "id type: %d.\n", idType);
				}
			 
			break;
		 	default: break;			
		}
	}
}
void findParamTypeOnFun(AST *root, char * function, int paramOrder, int lineNum)
{
	int i;
	if(root == 0) return;

	for(i=0; i < MAX_SONS;++i)
	 	findParamTypeOnFun(root->son[i],function,paramOrder, lineNum);

	if(root->type){
		switch(root->type){
			case AST_FUN: 
				if(!strcmp(root->symbol->text, function))
				{
					root = root->son[1]->son[0];
					while(root != 0)
					{
						if(root->symbol->paramorder == paramOrder)
						{
							//fprintf(stderr, "paramorder %d.\n", root->symbol->paramorder);
							if(idType != root->son[0]->type)
							{
					 			if(!(idType == AST_TYPE_INT && root->son[0]->type == AST_TYPE_CHAR)
					 				&& !(idType == AST_TYPE_CHAR && root->son[0]->type == AST_TYPE_INT))
					 			{
						 			fprintf(stderr,"Parameter type mismatch on function call, error on line %d.\n", lineNum);
									semanticErrorFlag = 1;						 				
					 			}
									
							}
						}
						root = root->son[1];
					}
				}
			 
			break;
		 	default: break;			
		}
	}
}