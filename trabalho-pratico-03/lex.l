%{ 
#include "parser.tab.h"
%}

%option yylineno
%option noyywrap

digito	[0-9]
letra_	[a-zA-Z_]
pontdig {digito}*"."?{digito}+
digpont {digito}+"."

numero  {pontdig}|{digpont}
id		{letra_}({letra_}|{digito})*
text    \"[a-zA-Z0-9=, ]*\"

%%

[\t ]+		{ ; /* ignora espaço em branco */ }
{numero}    { yylval.num = atof(yytext); return NUM; }
{id}		{
                if(strcmp(yytext, "if") == 0) {
                    return IF;
                }
                else if (strcmp(yytext, "print") == 0) {
                    return PRINT;
                }
                else if (strcmp(yytext, "pow") == 0) {
                    return POW;
                }
                else if (strcmp(yytext, "sqrt") == 0) {
                    return SQRT;
                }
                else {
                    strcpy(yylval.id, yytext); 
                    return ID; 
                }
            }
{text}      { strcpy(yylval.text, yytext); return TEXT; }
"=="        { return EQUAL; }
"!="        { return NOT_EQUAL; }
">="        { return BIGGER_EQUAL; }
"<="        { return LESS_EQUAL; }
.|\n		{ return yytext[0]; }

%%
