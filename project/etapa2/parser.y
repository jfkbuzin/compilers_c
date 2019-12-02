%{
//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
int yylex();
int yyerror();

%}


%union{ HASH_NODE *node; }

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
%token<node> TK_IDENTIFIER 
%token<node> LIT_INTEGER   
%token<node> LIT_FLOAT     
%token<node> LIT_CHAR      
%token<node> LIT_STRING    
%token TOKEN_ERROR   

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
	

cmdlist: cmd ';' cmdlist
	|
	;

cmd : KW_IF expr KW_THEN cmd 
	| KW_IF expr KW_THEN cmd KW_ELSE cmd
	| TK_IDENTIFIER '=' expr 	{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	| TK_IDENTIFIER 'q' expr 'p' '=' expr 	{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	| KW_WHILE expr cmd
	| KW_PRINT expr 
	| KW_READ esc 
	| KW_RETURN expr 
	| block 
	|
	;


expr : LIT_INTEGER				{ fprintf(stderr, "LIT_INTEGER found: %s\n",$1->text); }
	| LIT_FLOAT					{ fprintf(stderr, "LIT_FLOAT found: %s\n",$1->text); }
	| LIT_CHAR					{ fprintf(stderr, "LIT_CHAR found: %s\n",$1->text); }
	| LIT_STRING				{ fprintf(stderr, "LIT_STRING found: %s\n",$1->text); }
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr '>' expr
	| expr '<' expr
	| expr ',' expr
	| expr OPERATOR_EQ expr
	| expr OPERATOR_OR expr
	| expr OPERATOR_LE expr
	| expr OPERATOR_GE expr
	| expr OPERATOR_AND expr
	| OPERATOR_NOT expr
	| 'd' expr 'b'
	| TK_IDENTIFIER				{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	| TK_IDENTIFIER 'd' expr 'b'{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
	| TK_IDENTIFIER 'q' expr 'p'{ fprintf(stderr, "TK_IDENTIFIER found: %s\n",$1->text); }
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

fun : head block
	;

head : type ident2
	;

block : '{' cmdlist '}'
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

