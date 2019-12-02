%{
//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror();

%}


%union{ int value; }

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
%token TK_IDENTIFIER 
%token<value> LIT_INTEGER   
%token LIT_FLOAT     
%token LIT_CHAR      
%token LIT_STRING    
%token TOKEN_ERROR   

%%

program : declist
	;

declist :  dec declist
	|
	;

dec : cmd
	| var
	| fun
	;
	

cmd : KW_IF expr KW_THEN cmd
	| KW_IF expr KW_THEN cmd KW_ELSE cmd
	| TK_IDENTIFIER '=' expr cmd
	| TK_IDENTIFIER 'q' expr 'p' '=' expr cmd
	| KW_WHILE expr '{' cmd '}' ';' cmd
	| KW_PRINT expr cmd
	| KW_READ esc cmd
	| KW_RETURN expr cmd
	| cmd ';' cmd
	| '{' cmd '}' ';' cmd
	|
	;

expr : LIT_INTEGER				{ fprintf(stderr, "LIT_INTEGER found: %d\n",$1); }
	| LIT_FLOAT
	| LIT_CHAR
	| LIT_STRING
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
	| TK_IDENTIFIER
	| TK_IDENTIFIER 'd' expr 'b'
	| TK_IDENTIFIER 'q' expr 'p'
	;

exprlist : expr exprlist
	|
	;

esc : LIT_INTEGER
	| LIT_FLOAT 
	| LIT_CHAR
	| LIT_STRING
	| TK_IDENTIFIER
	;

var : KW_INT
	| KW_FLOAT 
	| KW_CHAR
	| var TK_IDENTIFIER '=' expr ';' 
	| var TK_IDENTIFIER 'q' expr 'p' ';'
	| var TK_IDENTIFIER 'q' expr 'p' ':' exprlist ';'
	;

fun : head body fun
	|
	;

head : var TK_IDENTIFIER 'd' param 'b'
	;

body : '{' cmd '}'
	;

varparam : KW_INT
	| KW_FLOAT 
	| KW_CHAR
	;

param : varparam TK_IDENTIFIER param
	| ',' param
	|
	;

%%

int getLineNumber(void);


int yyerror(char* msg)
{
	fprintf(stderr, "Syntax error at line %d\n",getLineNumber());
	exit(3);
}

