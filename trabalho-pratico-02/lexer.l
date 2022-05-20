%{
// Reconhecedor de uma linguagem simplificada
#include <iostream>
#include "tokens.h"
using std::cout;
%}

%option noyywrap

delim	            [ \t]
brancos             {delim}+
text                [a-zA-Z0-9 :)(\"\'.,$\+;&\-%]*
comentarios         "<!--".*"-->"|"<!DOCTYPE ".*\>
eol                 \n
caracter            [a-zA-Z0-9 ]
atributo            [ ][a-z]+=\".*\"
inlineblockstart    [.# ]*[a-zA-Z0-9]*[ ]*\{
inlineblockend      \}
alltags             (html|head|body|style|title|h1|h2|h3|h4|h5|h6|p|b|i|table|tr|th|td|ul|li|a)
singletags          (img|br)

%%
{brancos}|{comentarios}             ;                           // nenhuma ação pois é espaços em branco ou comentários
{eol}                               return LINE_JUMP;           // reconhece e retorna um salto de linha
\<{alltags}{atributo}*\>            return TAG_START;           // reconhece e retorna uma tag de início
"</"{caracter}*\>                   return TAG_END;             // reconhece e retorna uma tag de fim
{text}                              return TEXT;                // reconhece e retorna caracteres dentro de tag
\<{singletags}{atributo}*\>         return SINGLE_TAG;          // reconhece e retorna uma tag simples que não tem um par
{inlineblockstart}                  return INLINE_BLOCK_START;  // reconhece e retorna um início de um css inline-block
{inlineblockend}                    return INLINE_BLOCK_END;    // reconhece e retorna um fim de um css inline-block
%%

