//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include "tacs.h"

//code generation prototypes

TAC *makeBinOp(int type, TAC *result0, TAC *result1);
TAC *makeVecRead(HASH_NODE *symbol, TAC *result0);
TAC *makeVecWrite(HASH_NODE *symbol, TAC *result0, TAC *result1);
TAC *makeIf(TAC *result0, TAC *result1, TAC *result2);
TAC *makeWhile(TAC *result0, TAC *result1);
TAC *makeAss(AST *node, TAC *result0, HASH_NODE *currentFun);
TAC *makeFun(HASH_NODE *funId, TAC *result0, TAC *result1, TAC *result2);
TAC *makeSingle(int type, TAC *result0);
TAC *makeCall(HASH_NODE *funId, TAC *result0);
TAC *makeArg(AST *node, HASH_NODE *currentFun, TAC *result0, TAC *result1, int argOrder);

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2, int argOrder)
{
	TAC *newTac;
	newTac = (TAC *) calloc(1,sizeof(TAC));
	newTac->type = type;
	newTac->res = res;
	newTac->op1 = op1;
	newTac->op2 = op2;
	newTac->prev = 0;
	newTac->next = 0;
	newTac->argOrder = argOrder;
	return newTac;
}

void tacPrintSingle(TAC *tac)
{
	if(!tac) return;
	if(tac->type == TAC_SYMBOL) return;
	fprintf(stderr,"TAC(");

	switch(tac->type)
	{
		case TAC_SYMBOL: fprintf(stderr,"TAC_SYMBOL");
		break;
		case TAC_ADD: fprintf(stderr,"TAC_ADD");
		break;
		case TAC_SUB: fprintf(stderr,"TAC_SUB");
		break;
		case TAC_MUL: fprintf(stderr,"TAC_MUL");
		break;		
		case TAC_DIV: fprintf(stderr,"TAC_DIV");
		break;
		case TAC_GREATER: fprintf(stderr,"TAC_GREATER");
		break;
		case TAC_LESS: fprintf(stderr,"TAC_LESS");
		break;
		case TAC_OR: fprintf(stderr,"TAC_OR");
		break;
		case TAC_LE: fprintf(stderr,"TAC_LE");
		break;
		case TAC_GE: fprintf(stderr,"TAC_GE");
		break;
		case TAC_AND: fprintf(stderr,"TAC_AND");
		break;
		case TAC_EQUAL: fprintf(stderr,"TAC_EQUAL");
		break;
		case TAC_WHILE: fprintf(stderr,"TAC_WHILE");
		break;
		case TAC_IF: fprintf(stderr,"TAC_IF");
		break;
		case TAC_LABEL: fprintf(stderr,"TAC_LABEL");
		break;
		case TAC_COPY: fprintf(stderr,"TAC_COPY");
		break;		
		case TAC_BEGIN_FUN: fprintf(stderr,"TAC_BEGIN_FUN");
		break;
		case TAC_END_FUN: fprintf(stderr,"TAC_END_FUN");
		break;
		case TAC_READ: fprintf(stderr,"TAC_READ");
		break;
		case TAC_PRINT: fprintf(stderr,"TAC_PRINT");
		break;
		case TAC_RETURN: fprintf(stderr,"TAC_RETURN");
		break;
		case TAC_CALL_FUN: fprintf(stderr,"TAC_CALL_FUN");
		break;
		case TAC_ARG: fprintf(stderr,"TAC_ARG");
		break;
		case TAC_IF_ELSE: fprintf(stderr,"TAC_IF_ELSE");
		break;
		case TAC_VEC_READ: fprintf(stderr,"TAC_VEC_READ");
		break;
		case TAC_VEC_WRITE: fprintf(stderr,"TAC_VEC_WRITE");
		break;
		case TAC_JUMP: fprintf(stderr,"TAC_JUMP");
		break;
		case TAC_CONCAT: fprintf(stderr,"TAC_CONCAT");
		break;
		default: fprintf(stderr,"TAC_UNKNOWN");
		break;
	}

	if(tac->res) fprintf(stderr,", %s", tac->res->text);
	else fprintf(stderr,", 0");

	if(tac->op1) fprintf(stderr,", %s", tac->op1->text);
	else fprintf(stderr,", 0");

	if(tac->op2) fprintf(stderr,", %s", tac->op2->text);
	else fprintf(stderr,", 0");

	if(tac->argOrder != 0) fprintf(stderr,", argument order: %d", tac->argOrder);

	fprintf(stderr,") \n");
}

void tacPrintBackwards(TAC *tac)
{
	if(!tac) return;
	tacPrintSingle(tac);
	tacPrintBackwards(tac->prev);
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
	TAC *aux;
	if(!l1) return l2;
	if(!l2) return l1;

	for(aux = l2;aux->prev; aux = aux->prev) ;

	aux->prev = l1;

	return l2;
}

TAC *reverse(TAC *tac)
{
	if(!tac) return 0;
	for(;tac->prev;tac = tac->prev){
		tac->prev->next = tac;
	}
	return tac;
}

void tacPrintForward(TAC *tac)
{
	if(!tac) return;
	tacPrintSingle(tac);
	tacPrintForward(tac->next);
}

//code generation

TAC *tacGenerate(AST *node, HASH_NODE *currentFun, HASH_NODE *previousFun, int argOrder)
{

	if(currentFun != 0)
	{
		if(currentFun != previousFun)
		{
			previousFun = currentFun;
			argOrder = 0;
		}
	} 

	if(!node) return 0;

	switch(node->type){
		case AST_ASS: if(node->son[0]->son[0] != 0) currentFun = node->son[0]->symbol;
		break;
		case AST_SYMBOL: if(node->son[0] != 0) currentFun = node->symbol;
		break;
		case AST_COMMA: argOrder = argOrder + 1;
		break;
		default:
		break;			
	}

	int i;
	TAC *result[MAX_SONS];

	for(i=0;i<MAX_SONS;i++){
		result[i] = tacGenerate(node->son[i],currentFun,previousFun,argOrder);
	}
	switch(node->type){
		case AST_SYMBOL: 
		case AST_SYMBOL_STRING: return tacCreate(TAC_SYMBOL,node->symbol,result[0] ? result[0]->res : 0,0,0);
		break;
		//case AST_CALL: return makeCall(node->symbol, result[0]);
		//break;
		case AST_SYMBOL_VEC: return makeVecRead(node->symbol,result[0]);
		break;
		case AST_ADD: return makeBinOp(TAC_ADD,result[0],result[1]);
		break;
		case AST_SUB: return makeBinOp(TAC_SUB,result[0],result[1]);
		break;
		case AST_MUL: return makeBinOp(TAC_MUL,result[0],result[1]);
		break;
		case AST_DIV: return makeBinOp(TAC_DIV,result[0],result[1]);	
		break;
		case AST_GREATER: return makeBinOp(TAC_GREATER,result[0],result[1]);
		break;
		case AST_LESS: return makeBinOp(TAC_LESS,result[0],result[1]);
		break;
		case AST_OR: return makeBinOp(TAC_OR,result[0],result[1]);
		break;
		case AST_LE: return makeBinOp(TAC_LE,result[0],result[1]);
		break;
		case AST_GE: return makeBinOp(TAC_GE,result[0],result[1]);
		break;
		case AST_AND: return makeBinOp(TAC_AND,result[0],result[1]);
		break;
		case AST_EQUAL: return makeBinOp(TAC_EQUAL,result[0],result[1]);
		break;
		case AST_COMMA_EXPR: return makeBinOp(TAC_CONCAT,result[0],result[1]);
		break;
		case AST_IF: return makeIf(result[0],result[1],result[2]);
		break;
		case AST_WHILE: return makeWhile(result[0],result[1]);
		break;
		case AST_ASS: return makeAss(node, result[0],currentFun);
		break;
		case AST_FUN: return makeFun(node->symbol, result[0],result[1],result[2]);
		break;
		case AST_READ:
		case AST_PRINT:
		case AST_RETURN: return makeSingle(node->type, result[0]);
		break;
		case AST_COMMA: return makeArg(node, currentFun, result[0], result[1], argOrder);
		break;
		case AST_VECTOR: if(result[1]) 
							return makeVecWrite(node->symbol, result[0], result[1]);
						else 
							return tacJoin(tacJoin(tacJoin(result[0],result[1]), result[2]), result[3]);
		break;
		default: return tacJoin(tacJoin(tacJoin(result[0],result[1]), result[2]), result[3]);
		break;	
	}
	return 0;
}

TAC *makeBinOp(int type, TAC *result0, TAC *result1)
{
	return tacJoin(tacJoin(result0,result1), tacCreate(type, makeTemp(), 
			result0 ? result0->res : 0, result1 ? result1->res : 0,0));
}

TAC *makeVecRead(HASH_NODE *symbol, TAC *result0)
{
	return tacJoin(tacJoin(result0,tacCreate(TAC_SYMBOL,symbol,result0 ? result0->res : 0,0,0)), 
		tacCreate(TAC_VEC_READ, makeTemp(),symbol, result0 ? result0->res : 0,0));	
}

TAC *makeVecWrite(HASH_NODE *symbol, TAC *result0, TAC *result1)
{		
	
	TAC *newVecWriteTac = 0;

	newVecWriteTac = tacCreate(TAC_VEC_WRITE, symbol, result0 ? result0->res : 0,result1 ? result1->res : 0,0);

	return tacJoin(tacJoin(result0,result1), newVecWriteTac);
}


TAC *makeIf(TAC *result0, TAC *result1, TAC *result2)
{
	TAC *newIfTac = 0;
	TAC *newLabelTac = 0;
	HASH_NODE *newLabel = 0;

	newLabel = makeLabel();

	newIfTac = tacCreate(TAC_IF,newLabel,result0 ? result0->res : 0,0,0);
	newLabelTac = tacCreate(TAC_LABEL,newLabel,0,0,0);

	if(!result2) //if sem else
		return tacJoin(tacJoin(tacJoin(result0,newIfTac), result1),newLabelTac);
	else //if com else(ta certo isso?)
	{
		TAC *newJumpTac = 0;
		TAC *newElseLabelTac = 0;
		HASH_NODE *newElseLabel = 0;

		newElseLabel = makeLabel();

		newJumpTac = tacCreate(TAC_JUMP,newElseLabel,0,0,0);
		newElseLabelTac = tacCreate(TAC_LABEL,newElseLabel,0,0,0);

		return  tacJoin
					(tacJoin
						(tacJoin
							(tacJoin
								(tacJoin(
									tacJoin(result0,newIfTac), 
								result1),
							newJumpTac),	 
						newLabelTac),
					result2),
				newElseLabelTac);
	}
}

TAC *makeWhile(TAC *result0, TAC *result1)
{
	TAC *newWhileTac = 0;
	TAC *newLabelTac = 0;
	TAC *newJumpLabelTac = 0;
	TAC *newJumpTac = 0;
	HASH_NODE *newLabel = 0;
	HASH_NODE *newJumpLabel = 0;

	newJumpLabel = makeLabel();
	newLabel = makeLabel();

	newJumpTac = tacCreate(TAC_JUMP,newJumpLabel,0,0,0);
	newJumpLabelTac = tacCreate(TAC_LABEL,newJumpLabel,0,0,0);
	newWhileTac = tacCreate(TAC_IF,newLabel,result0 ? result0->res : 0,0,0);
	newLabelTac = tacCreate(TAC_LABEL,newLabel,0,0,0);
	
	return tacJoin(tacJoin(
			tacJoin(
			tacJoin(
				tacJoin(newJumpLabelTac,result0)
				,newWhileTac)
				,result1)
				,newJumpTac),newLabelTac);

	//return tacJoin(tacJoin(tacJoin(result0,newWhileTac), result1),newLabelTac);
}

TAC *makeAss(AST *node, TAC *result0, HASH_NODE *currentFun)
{
	if(node->son[0] != 0 && node->son[0]->type == AST_CALL){
		TAC *newCopyTac = 0;
		HASH_NODE *tempLabel = 0;
		TAC *newCallTac = 0;
	
		tempLabel = makeTemp();
		newCopyTac = tacCreate(TAC_COPY, node->symbol, tempLabel, 0,0);
		newCallTac = tacCreate(TAC_CALL_FUN,tempLabel,currentFun,0,0);
		
		return tacJoin(tacJoin(result0, newCallTac), newCopyTac);
	}
	else {
		return tacJoin(result0, tacCreate(TAC_COPY, node->symbol, result0 ? result0->res : 0, 0,0));
	}
		
}

TAC *makeFun(HASH_NODE *funId, TAC *result0, TAC *result1, TAC *result2)
{
	TAC *newBeginTac = 0;
	TAC *newEndTac = 0;

	newBeginTac = tacCreate(TAC_BEGIN_FUN,funId,0,0,0);
	newEndTac = tacCreate(TAC_END_FUN,funId,0,0,0);

	return tacJoin(tacJoin(newBeginTac, result2),newEndTac);	
}

TAC *makeSingle(int type, TAC *result0)
{
	TAC *newSingleTac = 0;
	int tac;

	switch(type){
		case AST_READ: tac = TAC_READ;
		break;
		case AST_RETURN: tac = TAC_RETURN;
		break;
		case AST_PRINT: tac = TAC_PRINT;
		break;
		default: break;
	}

	newSingleTac = tacCreate(tac,result0 ? result0->res : 0,0,0,0);

	return tacJoin(result0,newSingleTac);
}

TAC *makeCall(HASH_NODE *funId, TAC *result0)
{
	TAC *newCallTac = 0;

	newCallTac = tacCreate(TAC_CALL_FUN,makeTemp(),funId,0,0);

	return tacJoin(newCallTac, result0);
}

TAC *makeArg(AST *node, HASH_NODE *currentFun, TAC *result0, TAC *result1, int argOrder)
{
	TAC *newArgTac = 0;

	newArgTac = tacCreate(TAC_ARG,currentFun,result0 ? result0->res : 0,
		0,argOrder);	

	if(node->son[1]->type != AST_COMMA)
	{
		TAC *secondArgTac = 0;
		secondArgTac = tacCreate(TAC_ARG,currentFun,result1 ? result1->res : 0,
		0,argOrder+1);

		return tacJoin(tacJoin(tacJoin(newArgTac, result0),secondArgTac),result1);
	}
	else
	{
		return tacJoin(tacJoin(newArgTac, result0),result1);
	}

		
}
