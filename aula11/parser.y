%{
//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
int yylex();
int yyerror();

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

%left '<' '>'
%left '+' '-'
%left '*' '/'

%%

program : declist
	;

declist :  dec declist
	|
	;

dec : var
	| fun
	;
	

cmdlist: cmd cmdlist				{ $$ = astCreate(AST_LCMD, 0,$1,$2,0,0); }
	|								{ $$ = 0; }
	;

cmd : KW_IF expr KW_THEN cmd 		{ $$ = astCreate(AST_IF, 0,$2,$4,0,0); }
	| KW_IF expr KW_THEN cmd KW_ELSE cmd 	{ $$ = astCreate(AST_IF, 0,$2,$4,$6,0); }
	| TK_IDENTIFIER '=' expr ';'	{ $$ = astCreate(AST_ASS, $1,$3,0,0,0); }
	| TK_IDENTIFIER 'q' expr 'p' '=' expr ';'	{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	| KW_WHILE expr cmd 			{ $$ = 0; }
	| KW_PRINT expr ';' 			{ $$ = 0; }
	| KW_READ esc ';' 				{ $$ = 0; }
	| KW_RETURN expr ';' 			{ $$ = 0; }
	| cmd ';' cmd 					{ $$ = 0; }
	| block ';'    					{ $$ = 0; }
	| 								{ $$ = 0; }
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
	| expr ',' expr 			{ $$ = 0; }
	| expr OPERATOR_EQ expr 	{ $$ = astCreate(AST_EQUAL, 0,$1,$3,0,0); }
	| expr OPERATOR_OR expr 	{ $$ = astCreate(AST_OR, 0,$1,$3,0,0); }
	| expr OPERATOR_LE expr 	{ $$ = astCreate(AST_LE, 0,$1,$3,0,0); }
	| expr OPERATOR_GE expr 	{ $$ = astCreate(AST_GE, 0,$1,$3,0,0); }
	| expr OPERATOR_AND expr 	{ $$ = astCreate(AST_AND, 0,$1,$3,0,0); }
	| OPERATOR_NOT expr 		{ $$ = 0; }
	| 'd' expr 'b'				{ $$ = 0; }
	| TK_IDENTIFIER				{ $$ = astCreate(AST_SYMBOL, $1,0,0,0,0); }
	| TK_IDENTIFIER 'd' expr 'b'{ $$ = 0; }
	| TK_IDENTIFIER 'q' expr 'p'{ $$ = 0; }
	;

exprlist : expr exprlist
	|
	;

esc : LIT_INTEGER 				{ fprintf(stderr, "LIT_INTEGER found: %s\n",$1->text); }
	| LIT_FLOAT 				{ fprintf(stderr, "LIT_FLOAT found: %s\n",$1->text); }
	| LIT_CHAR					{ fprintf(stderr, "LIT_CHAR found: %s\n",$1->text); }
	| LIT_STRING				{ fprintf(stderr, "LIT_STRING found: %s\n",$1->text); }
	| TK_IDENTIFIER				{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	;

type: KW_INT
	| KW_FLOAT 
	| KW_CHAR
	;

var : type ident3 
	;

fun : head block fun
	|
	;

head : type ident2
	;

block : '{' cmdlist '}'			{ astPrint($2,0); }
	;

paramlist: type ident1
	| type ident1 ',' paramlist
	;

param : paramlist
	|
	;

ident1 : TK_IDENTIFIER { fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	;

ident2 : TK_IDENTIFIER 'd' param 'b' { fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	;

ident3 : TK_IDENTIFIER '=' expr ';' { fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	| 	 TK_IDENTIFIER 'q' expr 'p' ';' { fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	|	 TK_IDENTIFIER 'q' expr 'p' ':' exprlist ';' { fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	;

%%

int getLineNumber(void);


int yyerror(char* msg)
{
	fprintf(stderr, "Syntax error at line %d\n",getLineNumber());
	exit(3);
}

