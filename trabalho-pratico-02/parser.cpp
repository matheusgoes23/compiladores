#include "parser.h"
#include "tokens.h"
#include <iostream>
using std::cout;

void Parser::Start()
{
    int lines = 1;      // contador de linhas no arquivo
    int tag_format = 0; // formatador de indentação de tags
    int tags = 0;       // contador de pares de tags html
    int characters = 0; // contador de caracteres do conteúdo exibido pela página

    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        // trata o token recebido do analisador léxico
        switch (lookahead)
        {
        // reconhece o salto de linha
        case LINE_JUMP:
            lines++;
            break;

        // reconhece uma tag de início <tag>
        case TAG_START:
            tags++;

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--" << scanner.YYText() << "\n";

            tag_format++;
            break;

        // reconhece uma tag de fim </tag>
        case TAG_END:
            tag_format--;

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--" << scanner.YYText() << "\n";
            break;

        // reconhece uma tag que não reconhece um par início-fim
        case SINGLE_TAG:
            tags++;

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--" << scanner.YYText() << "\n";
            break;

        // reconhece caracteres de conteúdo exibido pela página
        case TEXT:
            characters += scanner.YYLeng();

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--"
                 << "Texto[" << scanner.YYLeng() << "]\n";
            break;

        // reconhece css inline-block dentro de style
        case INLINE_BLOCK_START:
            while ((lookahead = scanner.yylex()) != 0)
            {
                if (lookahead == INLINE_BLOCK_END)
                {
                    break;
                }
                else if (lookahead == LINE_JUMP)
                {
                    lines++;
                }
            }
            break;
        }
    }

    cout << "\nHá " << lines << " linhas no arquivo.\n"
         << "Há " << tags << " pares de Tags HTML.\n"
         << "Há " << characters << " caracteres do conteúdo exibido pela página.\n";
}
