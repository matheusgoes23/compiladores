#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;

// Modelo para símbolos
struct Symbol
{
    string var;
    string type;
};

// Tabela de símbolos
class SymTable
{
private:
    unordered_map<string, Symbol> table;
    SymTable *prev;

public:
    SymTable();
    SymTable(SymTable *t);

    bool Insert(string s, Symbol symb);
    Symbol *Find(string s);
};
