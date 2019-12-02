#include "ast.h"

AST *astCreate(int type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3){
	AST *newNode;
	newNode = (AST*)calloc(1,sizeof(AST));
	newNode->type = type;
	newNode->symbol = symbol;
	newNode->son[0] = son0;
	newNode->son[1] = son1;
	newNode->son[2] = son2;
	newNode->son[3] = son3;
	return newNode;

}
void astPrint(AST *node, int level){
	int i;
	if(node == 0) return;

	for(i=0;i<level;++i){
		fprintf(stderr," ");
	}
	fprintf(stderr,"AST(");

	switch(node->type){
		case AST_SYMBOL: fprintf(stderr,"AST_SYMBOL");
		break;
		case AST_SYMBOL_VEC: fprintf(stderr,"AST_SYMBOL_VEC");
		break;
		case AST_ADD: fprintf(stderr,"AST_ADD");
		break;
		case AST_SUB: fprintf(stderr,"AST_SUB");
		break;
		case AST_MUL: fprintf(stderr,"AST_MUL");
		break;
		case AST_DIV: fprintf(stderr,"AST_DIV");
		break;
		case AST_EQUAL: fprintf(stderr,"AST_EQUAL");
		break;
		case AST_GREATER: fprintf(stderr,"AST_GREATER");
		break;
		case AST_LESS: fprintf(stderr,"AST_LESS");
		break;
		case AST_OR: fprintf(stderr,"AST_OR");
		break;
		case AST_LE: fprintf(stderr,"AST_LE");
		break;
		case AST_GE: fprintf(stderr,"AST_GE");
		break;
		case AST_AND: fprintf(stderr,"AST_AND");
		break;
		case AST_IF: fprintf(stderr,"AST_IF");
		break;
		case AST_ASS: fprintf(stderr,"AST_ASS");
		break;
		case AST_LCMD: fprintf(stderr,"AST_LCMD");
		break;
		case AST_WHILE: fprintf(stderr,"AST_WHILE");
		break;
		case AST_PRINT: fprintf(stderr,"AST_PRINT");
		break;
		case AST_READ: fprintf(stderr,"AST_READ");
		break;
		case AST_RETURN: fprintf(stderr,"AST_RETURN");
		break;
		case AST_VAR: fprintf(stderr,"AST_VAR");
		break;
		case AST_VECTOR: fprintf(stderr,"AST_VECTOR");
		break;		
		case AST_NOT: fprintf(stderr,"AST_NOT");
		break;
		case AST_FUN: fprintf(stderr,"AST_FUN");
		break;
		case AST_CMD: fprintf(stderr,"AST_CMD");
		break;
		case AST_TYPE_INT: fprintf(stderr,"AST_TYPE_INT");
		break;									
		case AST_TYPE_FLOAT: fprintf(stderr,"AST_TYPE_FLOAT");
		break;									
		case AST_TYPE_CHAR: fprintf(stderr,"AST_TYPE_CHAR");
		break;	
		case AST_PARAM_LIST: fprintf(stderr,"AST_PARAM_LIST");
		break;									
		case AST_PARAM: fprintf(stderr,"AST_PARAM");
		break;
		case AST_EXPR: fprintf(stderr,"AST_EXPR");
		break;	
		case AST_DEC: fprintf(stderr,"AST_DEC");
		break;	
		case AST_COMMA: fprintf(stderr,"AST_COMMA");
		break;	 	
		case AST_PAREN: fprintf(stderr,"AST_PAREN");
		break;
		case AST_VECTOR_ATT: fprintf(stderr,"AST_VECTOR_ATT");
		break;	 		 												
		default: fprintf(stderr,"AST_UNKNOWN");
		break;
	}

	if(node->symbol != 0){
		fprintf(stderr,",%s)\n", node->symbol->text);
	} else {
		fprintf(stderr,")\n");
	}

	for(i=0;i<MAX_SONS;++i){
		astPrint(node->son[i], level+1);
	}

	//for(i=0;i<level;++i){
	//	fprintf(stderr," ");
	//}
	//fprintf(stderr,")");

}

void codePrint(FILE *f, AST *node){
	if(node == 0) return;



	switch(node->type){
		case AST_SYMBOL: fprintf(f,"%s", node->symbol->text); 
						if(node->son[0] != 0){
							fprintf(f," d");
							codePrint(f,node->son[0]);
							fprintf(f,"b");
						}
		break;
		case AST_SYMBOL_VEC: fprintf(f,"%s", node->symbol->text); 
							fprintf(f," q");
							codePrint(f,node->son[0]);
							fprintf(f,"p");
		break;			
		case AST_ADD: codePrint(f,node->son[0]);
						fprintf(f," + ");
						codePrint(f,node->son[1]);
		break;
		case AST_SUB: codePrint(f,node->son[0]);
						fprintf(f," - ");
						codePrint(f,node->son[1]);
		break;
		case AST_MUL: codePrint(f,node->son[0]);
						fprintf(f," * ");
						codePrint(f,node->son[1]);
		break;
		case AST_DIV: codePrint(f,node->son[0]);
						fprintf(f," / ");
						codePrint(f,node->son[1]);
		break;
		case AST_EQUAL: codePrint(f,node->son[0]);
						fprintf(f," == ");
						codePrint(f,node->son[1]);
		break;
		case AST_GREATER: codePrint(f,node->son[0]);
						fprintf(f," > ");
						codePrint(f,node->son[1]);
		break;
		case AST_LESS: codePrint(f,node->son[0]);
						fprintf(f," < ");
						codePrint(f,node->son[1]);
		break;
		case AST_OR: codePrint(f,node->son[0]);
						fprintf(f," or ");
						codePrint(f,node->son[1]);
		break;
		case AST_LE: codePrint(f,node->son[0]);
						fprintf(f," <= ");
						codePrint(f,node->son[1]);
		break;
		case AST_GE: codePrint(f,node->son[0]);
						fprintf(f," >= ");
						codePrint(f,node->son[1]);
		break;
		case AST_AND: codePrint(f,node->son[0]);
						fprintf(f," and ");
						codePrint(f,node->son[1]);
		break;
		case AST_IF:  fprintf(f,"if ");
						codePrint(f,node->son[0]);
						fprintf(f," then\n");
						codePrint(f,node->son[1]);

						if(node->son[2] != 0){
							fprintf(f," else\n");
							codePrint(f,node->son[2]);
						}
						
		break;
		case AST_ASS: fprintf(f,"%s", node->symbol->text); 
						fprintf(f," = ");
						codePrint(f,node->son[0]);
		break;
		case AST_LCMD: fprintf(f,"{\n");
						codePrint(f,node->son[0]);
						fprintf(f,"}\n");
		break;
		case AST_WHILE: fprintf(f,"while ");
						codePrint(f,node->son[0]);
						fprintf(f,"\n");
						codePrint(f,node->son[1]);
		break;
		case AST_PRINT: fprintf(f,"print \"");
						codePrint(f,node->son[0]);
						fprintf(f,"\"");
		break;
		case AST_READ: fprintf(f,"read ");
						codePrint(f,node->son[0]);
		break;
		case AST_RETURN: fprintf(f,"return ");
						codePrint(f,node->son[0]);
		break;
		case AST_VAR:  codePrint(f,node->son[0]);
						codePrint(f,node->son[1]);
						fprintf(f,";");
		break;
		case AST_VECTOR: fprintf(f,"%s", node->symbol->text); 
						fprintf(f," q");
						codePrint(f,node->son[0]);
						fprintf(f,"p ");

						if(node->son[1] != 0){
							fprintf(f," = ");
							codePrint(f,node->son[1]);
						}
		break;	
		case AST_VECTOR_ATT: fprintf(f,"%s", node->symbol->text); 
						fprintf(f," q");
						codePrint(f,node->son[0]);
						fprintf(f,"p: ");
						codePrint(f,node->son[1]);
						
		break;			
		case AST_NOT: fprintf(f,"not ");
						codePrint(f,node->son[0]);
		break;
		case AST_FUN: codePrint(f,node->son[0]);
					fprintf(f,"%s", node->symbol->text); 
					fprintf(f," d");
					codePrint(f,node->son[1]);
					fprintf(f,"b");
					fprintf(f,"\n");
					codePrint(f,node->son[2]);
		break;
		case AST_CMD: codePrint(f,node->son[0]);
					fprintf(f,";\n");
					codePrint(f,node->son[1]);
		break;
		case AST_TYPE_INT: fprintf(f,"int ");
		break;									
		case AST_TYPE_FLOAT: fprintf(f,"float ");
		break;									
		case AST_TYPE_CHAR: fprintf(f,"char ");
		break;	
		case AST_PARAM_LIST: codePrint(f,node->son[0]);
		break;									
		case AST_PARAM: codePrint(f,node->son[0]); //type
						fprintf(f,"%s", node->symbol->text); //id
						if(node->son[1] != 0){
							fprintf(f,", ");
							codePrint(f,node->son[1]); //next param
						}	
		break;
		case AST_EXPR: codePrint(f,node->son[0]);
						if(node->son[1] != 0){
							fprintf(f," ");
							codePrint(f,node->son[1]);
						}
		break;	
		case AST_DEC: codePrint(f,node->son[0]);
						fprintf(f,"\n");
						codePrint(f,node->son[1]);
		break;	
		case AST_COMMA: codePrint(f,node->son[0]);
						fprintf(f,", ");
						codePrint(f,node->son[1]);
		break;	 	
		case AST_PAREN: fprintf(f,"d ");
						codePrint(f,node->son[0]);
						fprintf(f,"b ");
		break;	 												
		default:
		break;
	}


}
