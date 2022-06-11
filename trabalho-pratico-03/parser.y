%{
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <sstream>
#include <cctype>
#include <math.h>

using namespace std;

/* protótipos das funções especiais */
int yylex(void);
int yyparse(void);
void yyerror(const char *);

char* substr(char* text, int init, int length);
string toString(float value);

/* tabela de símbolos */
unordered_map<string,double> variables;
string storage = "";	//variavel de armazenamento
%}

%union {
	double num;
	char id[16];
	char text[9999];
}

%token <id> ID
%token <num> NUM
%token <text> TEXT
%token IF PRINT SQRT POW
%token EQUAL NOT_EQUAL BIGGER_EQUAL LESS_EQUAL

%type <num> expr

%left '+' '-'
%left '*' '/'
%left '>' '<'
%left EQUAL NOT_EQUAL BIGGER_EQUAL LESS_EQUAL SQRT POW
%nonassoc IF
%nonassoc UMINUS

%%

math: math term
	| term
	;

term: ID '=' expr 						   { variables[$1] = $3; }
	| IF '(' expr ')' ID '=' expr		   { if ($3 == 1) variables[$5] = $7; }
	| IF '(' expr ')' PRINT '(' assign ')' { if ($3 == 1) cout << storage << "\n"; storage = ""; }
	| PRINT '(' assign ')' 				   { cout << storage << "\n"; storage = ""; }
	| expr								   { cout << $1 << "\n"; }
	| '\n'
	;

assign: text ',' assign
	  | text
	  ;

text: TEXT					{ storage.append(substr($1, 1, strlen($1) - 2)); }
    | expr                  { storage.append(toString($1));                  }
    ; 

expr: expr '+' expr			{ $$ = $1 + $3; }
	| expr '-' expr   		{ $$ = $1 - $3; }
	| expr '*' expr			{ $$ = $1 * $3; }
	| expr '/' expr			
	{ 
		if ($3 == 0)
			yyerror("divisão por zero");
		else
			$$ = $1 / $3; 
	}
	| '(' expr ')'				{ $$ = $2; }
	| '-' expr %prec UMINUS 	{ $$ = - $2; }
    | expr '>' expr             { $$ = $1 > $3; }     
	| expr '<' expr				{ $$ = $1 < $3; }   
    | expr EQUAL  expr          { $$ = $1 == $3; }
    | expr NOT_EQUAL expr       { $$ = $1 != $3; }
    | expr BIGGER_EQUAL  expr   { $$ = $1 >= $3; }
    | expr LESS_EQUAL  expr     { $$ = $1 <= $3; }
	| ID						{ $$ = variables[$1]; }
	| SQRT '(' expr ')'			{ $$ = sqrt( $3 ); }
	| POW '(' expr ',' expr ')'	{ $$ = pow( $3, $5 ); }
	| NUM
	;

%%

extern FILE * yyin;  

int main(int argc, char ** argv)
{
	/* se foi passado um nome de arquivo */
	if (argc > 1)
	{
		FILE * file;
		file = fopen(argv[1], "r");
		if (!file)
		{
			cout << "Arquivo " << argv[1] << " não encontrado!\n";
			exit(1);
		}
		
		/* entrada ajustada para ler do arquivo */
		yyin = file;
	}

	yyparse();
}

void yyerror(const char * s)
{
	/* variáveis definidas no analisador léxico */
	extern int yylineno;    
	extern char * yytext;   

	/* mensagem de erro exibe o símbolo que causou erro e o número da linha */
    cout << "Erro sintático: símbolo \"" << yytext << "\" (linha " << yylineno << ")\n";
}

char* substr(char* text, int init, int length) {
/*tirando espaços em branco*/
    char* aux = new char[length + 1];
    for (int i = 0; i < length; i++)
	{
        aux[i] = *(text + init + i);
	}
	aux[length] = 0;
    return aux;
}

string toString(float value) {
/*parser de double para string*/
    ostringstream text;
    text << value;
    return text.str();
}
