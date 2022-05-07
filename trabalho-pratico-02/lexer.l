%{
// Reconhecedor de uma linguagem simplificada
#include <iostream>
#include "tokens.h"
using std::cout;
%}

%option noyywrap
delim	    [ \t]
brancos     {delim}+
text        [a-zA-Z0-9 :)(\"\'.,$\+;&\-%]*
comentarios "<!--".*"-->"|"<!DOCTYPE ".*\>
eol         \n
caracter    [a-zA-Z0-9 ]
atributo    [ ][a-z]+=\".*\"
inlinestart [.# ]*[a-zA-Z0-9]*[ ]*\{
inlineend   \}
alltags     (html|head|body|style|title|h1|h2|h3|h4|h5|h6|p|b|i|table|tr|th|td|ul|li|a)
singletags   (img|br)

%%
{brancos}|{comentarios}             ; // nenhuma ação e nenhum retorno
{eol}                               return LINE_JUMP;
\<{alltags}{atributo}*\>            return TAG_START; 
"</"{caracter}*\>                   return TAG_END; 
{text}                              return TEXT;
\<{singletags}{atributo}*\>         return SINGLE_TAG;
{inlinestart}                       return INLINE_START; 
{inlineend}                         return INLINE_END; 
%%

