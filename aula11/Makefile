etapa3: main.o hash.o ast.o lex.yy.o y.tab.o
	gcc main.o hash.o ast.o lex.yy.o y.tab.o -o etapa3

main.o: main.c
	gcc -c main.c
ast.o: ast.c
	gcc -c ast.c
hash.o: hash.c
	gcc -c hash.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c	
y.tab.o: y.tab.c
	gcc -c y.tab.c
lex.yy.c: y.tab.c scanner.l
	lex scanner.l
y.tab.c: parser.y
	yacc -d parser.y

clean:
	rm *.o etapa3 lex.yy.c y.tab.c y.tab.h