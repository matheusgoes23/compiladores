all: interpreter

# Compiler
CPPC=g++

# Lexer
FLEX=flex

# Yacc 
BISON=bison

interpreter: lex.yy.c parser.tab.c
	$(CPPC) lex.yy.c parser.tab.c -std=c++17 -o interpreter

lex.yy.c: lex.l
	$(FLEX) lex.l

parser.tab.c: parser.y
	$(BISON) -d parser.y

clean:
	rm interpreter lex.yy.c parser.tab.c parser.tab.h