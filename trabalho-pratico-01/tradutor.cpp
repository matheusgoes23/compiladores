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
    if (argc == 2)
    {
        fin.open(argv[1]);

        if (!fin.is_open())
        {
            cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
            cout << "Verifique se o nome está correto, ou se é protegido para leitura.\n";
            exit(EXIT_FAILURE);
        }
        else if (argv[1] == "--help")
        {
            cout << "Intrucoes: Para ser escritas...";
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
}