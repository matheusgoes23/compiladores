#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

void Parser::Program()
{
    // program -> math block
    Match(Tag::ID);
    Block();
}

void Parser::Block()
{
    // block -> { decls stmts }
    if (!Match('{'))
        throw SyntaxError(scanner.Lineno(), "\'{\' esperado");

    // Nova tabela de símbolos para o bloco
    // ------------------------------------
    SymTable *saved = symtable;
    symtable = new SymTable(symtable);
    // ------------------------------------

    if (!Match('}'))
        throw SyntaxError(scanner.Lineno(), "\'}\' esperado");

    // Tabela do escopo envolvente volta a ser a tabela ativa
    // ------------------------------------------------------
    delete symtable;
    symtable = saved;
    // ------------------------------------------------------
}

void Parser::Decls()
{
    // decls -> decl decls
    //        | ϵ
    // decl  -> type id;

    while (lookahead->tag == Tag::TYPE)
    {
        string type{lookahead->toString()};
        Match(Tag::TYPE);

        string name{lookahead->toString()};
        Match(Tag::ID);

        Symbol s{name, type};

        // Insere variável na tabela de símbolos
        if (!symtable->Insert(name, s))
        {
            // A inserção falha quando a variável já está na tabela
            stringstream ss;
            ss << "variável \"" << name << "\" já definida";
            throw SyntaxError(scanner.Lineno(), ss.str());
        }
        if (!Match(';'))
        {
            stringstream ss;
            ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
            throw SyntaxError{scanner.Lineno(), "esperado ;"};
        }
    }
}

void Parser::Stmts()
{
    // stmts -> stmts stmt
    //        | ϵ
    // stmt  -> block
    //        | expr;

    while (true)
    {
        switch (lookahead->tag)
        {
        // stmt -> block
        case '{':
            Block();
            break;
        // stmt -> expr;
        case Tag::NUM:
            Expr();
            if (!Match(';'))
            {
                stringstream ss;
                ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            break;
        case '(':
            Expr();
            if (!Match(';'))
            {
                stringstream ss;
                ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            break;
        case Tag::ID:
            Expr();
            if (!Match(';'))
            {
                stringstream ss;
                ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            break;
        // stmts -> ϵ
        default:
            return;
        }
    }
}

// Já removida a recursividade à esquerda
void Parser::Expr()
{
    // expr -> term add
    Term();
    while (true)
    {
        // add -> + term { print(+) } add
        if (lookahead->tag == '+')
        {
            Match('+');
            Term();
            cout << '+';
        }
        // add -> - term { print(-) } add
        else if (lookahead->tag == '-')
        {
            Match('-');
            Term();
            cout << '-';
        }
        // add -> ϵ
        else
            return;
    }
}

// Já removida a recursividade à esquerda
void Parser::Term()
{
    // term -> factor mult
    Factor();
    while (true)
    {
        // mult -> * factor { print(*) } mult
        if (lookahead->tag == '*')
        {
            Match('*');
            Factor();
            cout << '*';
        }
        // mult -> / factor { print(/) } mult
        else if (lookahead->tag == '/')
        {
            Match('/');
            Factor();
            cout << '/';
        }
        // mult -> ϵ
        else
            return;
    }
}

void Parser::Factor()
{
    switch (lookahead->tag)
    {
    // factor -> (expr)
    case '(':
    {
        Match('(');
        Expr();
        if (!Match(')'))
            throw SyntaxError{scanner.Lineno(), "\')\' esperado"};
    }
    break;
    // factor -> num
    case Tag::NUM:
    {
        cout << "(" << lookahead->toString() << ")";
        Match(Tag::NUM);
    }
    break;
    // factor -> id
    case Tag::ID:
    {
        // Verifica tipo da variável na tabela de símbolos
        Symbol *s = symtable->Find(lookahead->toString());
        if (!s)
        {
            stringstream ss;
            ss << "variável \"" << lookahead->toString() << "\" não declarada";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        cout << "(" << s->var << ':' << s->type << ")";
        Match(Tag::ID);
    }
    break;
    // Erro de sintaxe
    default:

        stringstream ss;
        ss << "símbolo \'" << lookahead->toString() << "\' inválido na expressão";
        throw SyntaxError{scanner.Lineno(), ss.str()};
        break;
    }
}

bool Parser::Match(int tag)
{
    if (tag == lookahead->tag)
    {
        lookahead = scanner.Scan();
        return true;
    }

    return false;
}

Parser::Parser()
{
    lookahead = scanner.Scan();
    symtable = nullptr;
}

void Parser::Start()
{
    Program();
}