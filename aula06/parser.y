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

program : cmdlist
	;

cmdlist : cmd cmdlist
	|
	;

cmd : KW_IF expr cmd
	| TK_IDENTIFIER '=' expr ';'
	;

expr : LIT_INTEGER				{ fprintf(stderr, "LIT_INTEGER found: %d\n",$1); }
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr OPERATOR_EQ expr
	| 'd' expr 'b'
	| TK_IDENTIFIER
	| TK_IDENTIFIER 'd' expr 'b'
	;



%%

int getLineNumber(void);


int yyerror(char* msg)
{
	fprintf(stderr, "Syntax error at line %d\n",getLineNumber());
	exit(3);
}

