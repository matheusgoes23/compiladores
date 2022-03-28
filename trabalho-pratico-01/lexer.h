#include <unordered_map>
#include <string>
#include <sstream>
using std::string;
using std::stringstream;
using std::unordered_map;

// Cada token possui uma tag (número a partir de 256)
// a tag de caracteres individuais é seu código ASCII
enum Tag
{
    TYPE = 256,
    NUM,
    ID
};

// Classes para representar tokens
struct Token
{
    int tag;
    Token() : tag(0) {}
    Token(int t) : tag(t) {}
    virtual string toString()
    {
        stringstream ss;
        ss << char(tag);
        return ss.str();
    }
};

struct Num : public Token
{
    string type;
    float float_value;
    int value;

    Num() : Token(Tag::NUM), value(0) {}
    Num(int v_int, string tp, float v_float) : Token(Tag::NUM), value(v_int), type(tp), float_value(v_float) {}
    Num(int v, string) : Token(Tag::NUM), value(v) {}
    virtual string toString()
    {
        stringstream ss;
        if (type == "int")
        {
            ss << value;
            return ss.str();
        }
        else
        {
            ss << float_value;
            return ss.str();
        }
    }
};

struct Id : public Token
{
    string tp;
    Id() : Token(Tag::ID) {}
    Id(int t, string tp) : Token(t), tp(tp) {}
    virtual string toString() { return tp; }
};

// Declarando o analisador léxico
class Lexer
{
private:
    // Tipos de tokens da linguagem
    struct
    {
        Token t;
        Num n;
        Id i;
    } token;

    int line = 1;
    char peek;
    unordered_map<string, Id> id_table;

public:
    Lexer();
    int Lineno();
    Token *Scan();
};
