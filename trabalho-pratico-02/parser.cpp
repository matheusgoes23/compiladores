#include "parser.h"
#include "tokens.h"
#include <iostream>
using std::cout;

void Parser::Start()
{
    int lines = 1, tag_format = 0, tags = 0, characters = 0;

    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        // trata o token recebido do analisador léxico
        switch (lookahead)
        {
        case LINE_JUMP:
            lines++;
            break;

        case TAG_START:
            tags++;

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--" << scanner.YYText() << "\n";

            tag_format++;
            break;

        case TAG_END:
            tag_format--;

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--" << scanner.YYText() << "\n";
            break;

        case SINGLE_TAG:
            tags++;

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--" << scanner.YYText() << "\n";
            break;

        case TEXT:
            characters += scanner.YYLeng();

            for (int i = 0; i < tag_format; i++)
            {
                cout << "|\t";
            }
            cout << "+--"
                 << "Texto[" << scanner.YYLeng() << "]\n";
            break;

        case INLINE_START:
            while ((lookahead = scanner.yylex()) != 0)
            {
                if (lookahead == INLINE_END)
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
