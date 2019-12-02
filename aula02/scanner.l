

%%

"if"	 						{ printf("[KW_IF] "); }
[0-9]+  						{ printf("[NUMBER] "); }
([-A-Za-z_]|"]")[-A-Za-z_0-9]*  { printf("[ID] "); }
";"								{ printf("[PEV] "); }
"="								{ printf("[EQUAL] "); }
"+"								{ printf("[PLUS] "); }
[ \t\n]
"//".*
.								{ printf("[ERROR:UNKNOWN] "); }


%%

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
	yylex();
	exit(0);
	
}

int yywrap()
{
	return 1;
}

