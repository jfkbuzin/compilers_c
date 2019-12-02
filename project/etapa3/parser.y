%{
//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
int yylex();
int yyerror();
extern FILE *yyout;

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



%left '<' '>'
%left '+' '-'
%left '*' '/'

%%

program : declist 				{ astPrint($1,0);
								codePrint(yyout,$1);
 								}
	;

declist :  dec declist 			{ $$ = astCreate(AST_DEC, 0,$1,$2,0,0); }
	| 							{ $$ = 0; }
	;

dec : var 						{ $$ = $1; }
	| fun						{ $$ = $1; }
	;
	

cmdlist: cmd ';' cmdlist		{ $$ = astCreate(AST_CMD, 0,$1,$3,0,0);}
	|							{ $$ = 0; }
	;

cmd : KW_IF expr KW_THEN cmd 	{ $$ = astCreate(AST_IF, 0,$2,$4,0,0); }
	| KW_IF expr KW_THEN cmd KW_ELSE cmd 	{ $$ = astCreate(AST_IF, 0,$2,$4,$6,0); }
	| TK_IDENTIFIER '=' expr 	{ $$ = astCreate(AST_ASS, $1,$3,0,0,0); }
	| TK_IDENTIFIER 'q' expr 'p' '=' expr 	{ $$ = astCreate(AST_VECTOR, $1,$3,$6,0,0); }
	| KW_WHILE expr cmd 		{ $$ = astCreate(AST_WHILE, 0,$2,$3,0,0); }
	| KW_PRINT expr 			{ $$ = astCreate(AST_PRINT, 0,$2,0,0,0); }
	| KW_READ esc 				{ $$ = astCreate(AST_READ, 0,$2,0,0,0); }
	| KW_RETURN expr 			{ $$ = astCreate(AST_RETURN, 0,$2,0,0,0); }
	| block  					{ $$ = $1; }
	| 							{ $$ = 0; }
	;

expr : LIT_INTEGER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| LIT_FLOAT					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| LIT_CHAR					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| LIT_STRING				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| expr '+' expr 			{ $$ = astCreate(AST_ADD, 0,$1,$3,0,0); }
	| expr '-' expr 			{ $$ = astCreate(AST_SUB, 0,$1,$3,0,0); }
	| expr '*' expr 			{ $$ = astCreate(AST_MUL, 0,$1,$3,0,0); }
	| expr '/' expr 			{ $$ = astCreate(AST_DIV, 0,$1,$3,0,0); }
	| expr '>' expr 			{ $$ = astCreate(AST_GREATER, 0,$1,$3,0,0); }
	| expr '<' expr 			{ $$ = astCreate(AST_LESS, 0,$1,$3,0,0); }
	| expr ',' expr 			{ $$ = astCreate(AST_COMMA, 0,$1,$3,0,0); }
	| expr OPERATOR_EQ expr 	{ $$ = astCreate(AST_EQUAL, 0,$1,$3,0,0); }
	| expr OPERATOR_OR expr 	{ $$ = astCreate(AST_OR, 0,$1,$3,0,0); }
	| expr OPERATOR_LE expr 	{ $$ = astCreate(AST_LE, 0,$1,$3,0,0); }
	| expr OPERATOR_GE expr 	{ $$ = astCreate(AST_GE, 0,$1,$3,0,0); }
	| expr OPERATOR_AND expr 	{ $$ = astCreate(AST_AND, 0,$1,$3,0,0); }
	| OPERATOR_NOT expr 		{ $$ = astCreate(AST_NOT, 0,$2,0,0,0); }
	| 'd' expr 'b'				{ $$ = astCreate(AST_PAREN, 0,$2,0,0,0); }
	| TK_IDENTIFIER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| TK_IDENTIFIER 'd' expr 'b'{ $$ = astCreate(AST_SYMBOL, $1,$3,0,0,0); }
	| TK_IDENTIFIER 'q' expr 'p'{ $$ = astCreate(AST_SYMBOL_VEC, $1,$3,0,0,0); }
	;

exprlist : expr exprlist		{ $$ = astCreate(AST_EXPR, 0,$1,$2,0,0); }
	|							{ $$ = 0; }
	;

esc : LIT_INTEGER 				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| LIT_FLOAT 				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| LIT_CHAR					{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| LIT_STRING				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| TK_IDENTIFIER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	;

type: KW_INT 					{ $$ = astCreate(AST_TYPE_INT, 0,0,0,0,0); }
	| KW_FLOAT 					{ $$ = astCreate(AST_TYPE_FLOAT, 0,0,0,0,0); }
	| KW_CHAR 					{ $$ = astCreate(AST_TYPE_CHAR, 0,0,0,0,0); }
	;

var : type ident 				{ $$ = astCreate(AST_VAR, 0,$1,$2,0,0);}
	;

fun : type TK_IDENTIFIER 'd' param 'b'  block 	{ $$ = astCreate(AST_FUN, $2,$1,$4,$6,0); }
	;

block : '{' cmdlist '}'			{ $$ = astCreate(AST_LCMD, 0,$2,0,0,0);}
	;

paramlist: type TK_IDENTIFIER { $$ = astCreate(AST_PARAM, $2,$1,0,0,0); }
	| type TK_IDENTIFIER ',' paramlist { $$ = astCreate(AST_PARAM, $2,$1,$4,0,0); }
	;

param : paramlist				{ $$ = astCreate(AST_PARAM_LIST, 0,$1,0,0,0); }
	|							{ $$ = 0; }
	;


ident:   TK_IDENTIFIER '=' expr ';' { $$ = astCreate(AST_ASS, $1,$3,0,0,0); }
	| 	 TK_IDENTIFIER 'q' expr 'p' ';' { $$ = astCreate(AST_VECTOR, $1,$3,0,0,0); }
	|	 TK_IDENTIFIER 'q' expr 'p' ':' exprlist ';' { $$ = astCreate(AST_VECTOR_ATT, $1,$3,$6,0,0);}
	;

%%

int getLineNumber(void);


int yyerror(char* msg)
{
	fprintf(stderr, "Syntax error at line %d\n",getLineNumber());
	exit(3);
}

