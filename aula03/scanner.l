%{
#include "hash.h"
#define KW_IF 256
#define LIT_INT 257
#define IDENTIFIER 258
#define TOKEN_UNKNOWN 259


int lineNumber = 1;
int running = 1;
%}



%%S

"if"	 						{ return KW_IF; }
[0-9]+  						{ hashInsert(LIT_INT,yytext); fprintf(stderr,"%s",yytext); return LIT_INT; }
([-A-Za-z_]|"]")[-A-Za-z_0-9]*  { hashInsert(IDENTIFIER,yytext); fprintf(stderr,"%s",yytext); return IDENTIFIER; }
[-;=+*]							{ return yytext[0]; }
[ \t]
"\n"							{ ++lineNumber; }
"//".*
.								{ return TOKEN_UNKNOWN; }


%%

//#include "hash.h"

int main(int argc, char **argv)
{
	if(argc<2)
	{
		fprintf(stderr,"CALL:a.out file_name\n");
		exit(1);
	}
	if((yyin = fopen(argv[1],"r")) == 0)
	{
		fprintf(stderr,"Cannot open file\n");
		exit(2);		
	}
	int tok;

	hashInit();
	while(running){
		tok = yylex();
		if(!running) break;
		fprintf(stderr,"[%d, ",lineNumber);
		if(tok<256) 
		{
			fprintf(stderr,"[%c] \n",tok);
		} else {
			switch(tok)
			{
				case KW_IF: fprintf(stderr,"[KW_IF] \n"); break;
				case LIT_INT: fprintf(stderr,"[LIT_INT] \n"); break;
				case IDENTIFIER: fprintf(stderr,"[IDENTIFIER] \n"); break;
				case TOKEN_UNKNOWN: fprintf(stderr,"[TOKEN_UNKNOWN] \n"); break;
				default: fprintf(stderr,"[UNKNOWN ERROR] \n"); break;
				break;
			}			
		}

	}


	hashPrint();
	exit(0);
	
}

int yywrap()
{
	running = 0;
	return 1;
}

