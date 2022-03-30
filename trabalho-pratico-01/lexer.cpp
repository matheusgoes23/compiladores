#include "lexer.h"
#include "math.h"
#include <fstream>
#include <sstream>
extern std::ifstream fin;
using std::stringstream;

// Construtor
Lexer::Lexer()
{
	// Insere palavras reservadas na tabela de id's
	id_table["int"] = Id{Tag::TYPE, "int"};
	id_table["float"] = Id{Tag::TYPE, "float"};
	id_table["char"] = Id{Tag::TYPE, "char"};
	id_table["bool"] = Id{Tag::TYPE, "bool"};

	// Inicia a leitura da entrada
	peek = fin.get();
}

// Retorna o número da linha atual
int Lexer::Lineno()
{
	return line;
}

// Retorna tokens para o analisador sintático
Token *Lexer::Scan()
{
	// Ignorando espaços, tabulações, saltos de linha e comentários
	while (isspace(peek) || peek == '/')
	{
		// Ignorando apenas espaços, tabulações e saltos de linha
		if (isspace(peek))
		{
			if (peek == '\n')
				line += 1;
			peek = fin.get();
		}

		// Ignorando apenas comentários
		if (peek == '/')
		{
			peek = fin.get();

			// Ignorando comentário do tipo //
			if (peek == '/')
			{
				// Ignorando até o salto de linha
				while (peek != '\n')
					peek = fin.get();

				// Lê o próximo caractere
				peek = fin.get();
			}
			else
			{
				// Ignorando comentário do tipo /**/
				if (peek == '*')
				{
					bool condition = true;

					while (condition)
					{
						peek = fin.get();

						if (peek == '*')
						{
							peek = fin.get();

							if (peek == '/')
							{
								peek = fin.get();
								condition = false;
							}
						}
					}

					// Lê o próximo caractere
					peek = fin.get();
				}
			}
		}
	}

	// Retorna números
	if (isdigit(peek))
	{
		int v = 0;
		int v_float = 0;
		int exponent = 0;
		float result = 0;
		do
		{
			// Converte caractere 'n' para o dígito numérico n
			int n = peek - '0';
			v = 10 * v + n;
			peek = fin.get();
		} while (isdigit(peek));

		// Tratando número com ponto flutuante
		if (peek == '.')
		{
			peek = fin.get();

			while (isdigit(peek))
			{
				int n = peek - '0';
				v_float = 10 * v_float + n;
				exponent++;
				
				peek = fin.get();
			}

			result = v_float / pow(10, exponent);
			result = result + v;

			token.n = Num{v, "float", result};
			return &token.n;
		}

		token.n = Num{v, "int", result};
		return &token.n;
	}

	// Retorna palavras-chave e identificadores
	if (isalpha(peek))
	{
		stringstream ss;
		do
		{
			ss << peek;
			peek = fin.get();

		} while (isalpha(peek));

		string s = ss.str();
		auto pos = id_table.find(s);

		// Se o lexema já está na tabela
		if (pos != id_table.end())
		{
			// Retorna o token associado
			token.i = pos->second;
			return &token.i;
		}

		// Se o lexema ainda não está na tabela
		Id new_id{Tag::ID, s};
		id_table[s] = new_id;

		// Retorna o token ID
		token.i = new_id;
		return &token.i;
	}

	// Operadores (e caracteres não alphanuméricos isolados)
	Token op{peek};
	peek = ' ';

	// Retorna o token
	token.t = op;
	return &token.t;
}