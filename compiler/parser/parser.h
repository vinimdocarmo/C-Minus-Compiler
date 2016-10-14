#pragma once

#include "../lexer/lex.h"
#include <vector>

class Parser {
private:
    Lex *lex;
    Token currToken;

    void comsume();

//non-terminal methods
    void program();

    void declaration_list();

    void declaration();

    void declaration_list_recur();

    void var_declaration();

    void type_specifier();

    void fun_declaration();

    void params();

    void param_list();

    void param_list_recur();

    void param();

    void compound_stmt();

    void compound_stmt_recur();

    void local_declarations();

    void local_declarations_recur();

public:
// Constructor
    Parser(Lex &);

    void parse();
};


