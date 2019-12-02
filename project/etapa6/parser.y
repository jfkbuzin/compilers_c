%{
//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "tacs.h"
int yylex();
int yyerror();
extern FILE *yyout;

int getLineNumber(void);

%}


%union{ 
	HASH_NODE *symbol; 
	AST *ast; 
}

%token KW_CHAR       
%token KW_INT        
%token KW_FLOAT      
%token KW_IF         
%token KW_THEN       
%token KW_ELSE       
%token KW_WHILE      
%token KW_READ       
%token KW_RETURN     
%token KW_PRINT      
%token OPERATOR_LE   
%token OPERATOR_GE   
%token OPERATOR_EQ   
%token OPERATOR_OR   
%token OPERATOR_AND  
%token OPERATOR_NOT  
%token<symbol> TK_IDENTIFIER 
%token<symbol> LIT_INTEGER   
%token<symbol> LIT_FLOAT     
%token<symbol> LIT_CHAR      
%token<symbol> LIT_STRING    
%token TOKEN_ERROR   

%type<ast> expr;
%type<ast> cmd;
%type<ast> cmdlist;
%type<ast> program;
%type<ast> declist;
%type<ast> dec;
%type<ast> var;
%type<ast> fun;
%type<ast> exprlist;
%type<ast> esc;
%type<ast> type;
%type<ast> block;
%type<ast> paramlist;
%type<ast> param;
%type<ast> ident;
%type<ast> comma;
%type<ast> id;



%left '<' '>' '='
%left '+' '-'
%left '*' '/'

%%

program : declist 				{ 
								TAC * taclist;
								astPrint($1,0);
								codePrint(yyout,$1);
								setDeclaration($1);
								//checkUndeclared();
								//checkOperands($1);
								//semanticError();
								taclist = reverse(tacGenerate($1,0,0,0));
								tacPrintForward(taclist);
								generateASM(taclist, $1, "asm.s");
 								}
	;

declist :  dec declist 			{ $$ = astCreate(AST_DEC, 0,$1,$2,0,0,getLineNumber()); }
	| 							{ $$ = 0; }
	;

dec : var 						{ $$ = $1; }
	| fun						{ $$ = $1; }
	;
	

cmdlist: cmd ';' cmdlist		{ $$ = astCreate(AST_CMD, 0,$1,$3,0,0,getLineNumber()); }
	|							{ $$ = 0; }
	;

cmd : KW_IF expr KW_THEN cmd 	{ $$ = astCreate(AST_IF, 0,$2,$4,0,0,getLineNumber()); }
	| KW_IF expr KW_THEN cmd KW_ELSE cmd 	{ $$ = astCreate(AST_IF, 0,$2,$4,$6,0,getLineNumber()); }
	| TK_IDENTIFIER '=' expr 	{ $$ = astCreate(AST_ASS, $1,$3,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER 'q' expr 'p' '=' expr 	{ $$ = astCreate(AST_VECTOR, $1,$3,$6,0,0,getLineNumber()); }
	| KW_WHILE expr cmd 		{ $$ = astCreate(AST_WHILE, 0,$2,$3,0,0,getLineNumber()); }
	| KW_PRINT expr 			{ $$ = astCreate(AST_PRINT, 0,$2,0,0,0,getLineNumber()); }
	| KW_READ esc 				{ $$ = astCreate(AST_READ, 0,$2,0,0,0,getLineNumber()); }
	| KW_RETURN expr 			{ $$ = astCreate(AST_RETURN, 0,$2,0,0,0,getLineNumber()); }
	| block  					{ $$ = $1; }
	| 							{ $$ = 0; }
	;

expr : LIT_INTEGER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| LIT_FLOAT					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| LIT_CHAR					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| LIT_STRING				{ $$ = astCreate(AST_SYMBOL_STRING, $1,0,0,0,0,getLineNumber()); }
	| expr '+' expr 			{ $$ = astCreate(AST_ADD, 0,$1,$3,0,0,getLineNumber()); }
	| expr '-' expr 			{ $$ = astCreate(AST_SUB, 0,$1,$3,0,0,getLineNumber()); }
	| expr '*' expr 			{ $$ = astCreate(AST_MUL, 0,$1,$3,0,0,getLineNumber()); }
	| expr '/' expr 			{ $$ = astCreate(AST_DIV, 0,$1,$3,0,0,getLineNumber()); }
	| expr '>' expr 			{ $$ = astCreate(AST_GREATER, 0,$1,$3,0,0,getLineNumber()); }
	| expr '<' expr 			{ $$ = astCreate(AST_LESS, 0,$1,$3,0,0,getLineNumber()); }
	| expr ',' expr 			{ $$ = astCreate(AST_COMMA_EXPR, 0,$1,$3,0,0,getLineNumber()); }
	| expr OPERATOR_EQ expr 	{ $$ = astCreate(AST_EQUAL, 0,$1,$3,0,0,getLineNumber()); }
	| expr OPERATOR_OR expr 	{ $$ = astCreate(AST_OR, 0,$1,$3,0,0,getLineNumber()); }
	| expr OPERATOR_LE expr 	{ $$ = astCreate(AST_LE, 0,$1,$3,0,0,getLineNumber()); }
	| expr OPERATOR_GE expr 	{ $$ = astCreate(AST_GE, 0,$1,$3,0,0,getLineNumber()); }
	| expr OPERATOR_AND expr 	{ $$ = astCreate(AST_AND, 0,$1,$3,0,0,getLineNumber()); }
	| OPERATOR_NOT expr 		{ $$ = astCreate(AST_NOT, 0,$2,0,0,0,getLineNumber()); }
	| 'd' expr 'b'				{ $$ = astCreate(AST_PAREN, 0,$2,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER 'd' comma 'b'{ $$ = astCreate(AST_CALL, $1,$3,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER 'd' 'b'		{ $$ = astCreate(AST_CALL, $1,0,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER 'q' expr 'p'{ $$ = astCreate(AST_SYMBOL_VEC, $1,$3,0,0,0,getLineNumber()); }
	;


comma : id ',' comma  { $$ = astCreate(AST_COMMA, 0,$1,$3,0,0,getLineNumber()); }
	| id				{ $$ = $1; }
	;

id :  LIT_INTEGER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| LIT_FLOAT					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| LIT_CHAR					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| LIT_STRING				{ $$ = astCreate(AST_SYMBOL_STRING, $1,0,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	| TK_IDENTIFIER 'q' expr 'p'{ $$ = astCreate(AST_SYMBOL_VEC, $1,$3,0,0,0,getLineNumber()); }
	;
   
exprlist : expr exprlist		{ $$ = astCreate(AST_EXPR, 0,$1,$2,0,0,getLineNumber()); }
	|							{ $$ = 0; }
	;

esc : TK_IDENTIFIER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0,getLineNumber()); }
	;

type: KW_INT 					{ $$ = astCreate(AST_TYPE_INT, 0,0,0,0,0,getLineNumber()); }
	| KW_FLOAT 					{ $$ = astCreate(AST_TYPE_FLOAT, 0,0,0,0,0,getLineNumber()); }
	| KW_CHAR 					{ $$ = astCreate(AST_TYPE_CHAR, 0,0,0,0,0,getLineNumber()); }
	;

var : type ident 				{ $$ = astCreate(AST_VAR, 0,$1,$2,0,0,getLineNumber()); }
	;

fun : type TK_IDENTIFIER 'd' param 'b'  block 	{ $$ = astCreate(AST_FUN, $2,$1,$4,$6,0,getLineNumber()); }
	;

block : '{' cmdlist '}'			{ $$ = astCreate(AST_LCMD, 0,$2,0,0,0,getLineNumber()); }
	;

paramlist: type TK_IDENTIFIER { $$ = astCreate(AST_PARAM, $2,$1,0,0,0,getLineNumber()); }
	| type TK_IDENTIFIER ',' paramlist { $$ = astCreate(AST_PARAM, $2,$1,$4,0,0,getLineNumber()); }
	;

param : paramlist				{ $$ = astCreate(AST_PARAM_LIST, 0,$1,0,0,0,getLineNumber()); }
	|							{ $$ = 0; }
	;


ident:   TK_IDENTIFIER '=' expr ';' { $$ = astCreate(AST_ASS_ATT, $1,$3,0,0,0,getLineNumber()); }
	| 	 TK_IDENTIFIER 'q' expr 'p' ';' { $$ = astCreate(AST_VECTOR, $1,$3,0,0,0,getLineNumber()); }
	|	 TK_IDENTIFIER 'q' expr 'p' ':' exprlist ';' { $$ = astCreate(AST_VECTOR_ATT, $1,$3,$6,0,0,getLineNumber()); }
	;

%%




int yyerror(char* msg)
{
	fprintf(stderr, "Syntax error at line %d\n",getLineNumber());
	exit(3);
}

