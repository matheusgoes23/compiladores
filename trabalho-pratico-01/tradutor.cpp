#include <iostream>
#include <fstream>
#include <cstring>
#include "parser.h"
#include "error.h"
using namespace std;

// Arquivo de entrada
ifstream fin;

// Programa pode receber nomes de arquivos
int main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--help"))
    {
        fin.open(argv[1]);

        if (!fin.is_open())
        {
            cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
            cout << "Verifique se o nome está correto, ou se é protegido para leitura.\n";
            exit(EXIT_FAILURE);
        }

        Parser tradutor;
        try
        {
            tradutor.Start();
        }
        catch (SyntaxError err)
        {
            err.What();
        }

        cout << endl;
        fin.close();
    }
    else
    {
        cout
            << "Instrucoes: \n\tuso: ./tradutor [arquivo.math]\t\t"
            << "ler o arquivo contendo expressoes aritmeticas em "
            << "notacao infixada e traduz para notacao pos-fixada.\n";
    }
}