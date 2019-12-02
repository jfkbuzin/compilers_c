%{
//Nome: Josué Filipe Keglevich de Buzin, cartão: 166409


int lineNumber = 1;
int running = 1;

int getLineNumber(void){
	return lineNumber;
}

int isRunning(void){
	return running;
}


#include "hash.h"
#include "ast.h"
#include "y.tab.h"

%}

%x COMMENT
%x STRING

%%

"char"	 						{ return KW_CHAR; }
"int"	 						{ return KW_INT; }
"float"	 						{ return KW_FLOAT; }
"if"	 						{ return KW_IF; }
"then"	 						{ return KW_THEN; }
"else"	 						{ return KW_ELSE; }
"while"	 						{ return KW_WHILE; }
"read"	 						{ return KW_READ; }
"return"	 					{ return KW_RETURN; }
"print"	 						{ return KW_PRINT; }
"or"	 						{ return OPERATOR_OR; }
"and"	 						{ return OPERATOR_AND; }
"not"	 						{ return OPERATOR_NOT; }
"<="	 						{ return OPERATOR_LE; }
">="	 						{ return OPERATOR_GE; }
"=="	 						{ return OPERATOR_EQ; }
[-,;:dbqp{}=+\-*/<>!&$#]		{ return yytext[0]; }
[0-9]+  						{ yylval.symbol = hashInsert(SYMBOL_LIT_INT,yytext); return LIT_INTEGER; }
[0-9]+"."[0-9]+					{ yylval.symbol = hashInsert(SYMBOL_LIT_REAL,yytext); return LIT_FLOAT; }
([-A-Zace-or-z_0-9]|"]")[-A-Zace-or-z_0-9]*  { yylval.symbol = hashInsert(SYMBOL_IDENTIFIER,yytext); return TK_IDENTIFIER; }
\'(\\.|[^'\\])\'				{ yylval.symbol = hashInsert(SYMBOL_LIT_CHAR,yytext); return LIT_CHAR; }				
[ \t]
"\n"							{ ++lineNumber; }
"//".*

"\""							{ BEGIN(STRING); }
<STRING>"\""					{ yytext[strlen(yytext)-1]='\0'; yylval.symbol = hashInsert(SYMBOL_LIT_STRING,yytext); BEGIN(INITIAL); return LIT_STRING; }
<STRING>"\n"					{ yymore(); ++lineNumber; }
<STRING>[^"]					{ yymore(); }

"/*"							{ BEGIN(COMMENT); }
<COMMENT>"*/"					{ BEGIN(INITIAL); }
<COMMENT>"\n"					{ ++lineNumber; }
<COMMENT>.
.								{ return TOKEN_ERROR; }


%%

//#include "main.c"

void initMe(void){
	hashInit();
}


int yywrap()
{
	running = 0;
	return 1;
}

