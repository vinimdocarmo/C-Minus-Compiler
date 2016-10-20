#pragma once

#include "../lexer/lex.h"
#include <vector>

class Parser {
private:
    Lex *lex;

    Token curr_token;

    void consume_token();

    bool terminal(Token_Type);

    bool terminal(string);

//non-terminal methods
    bool program();

    bool declaration_list();

    bool declaration_list_recur();

    bool declaration();

    bool var_declaration();

    bool type_specifier();

    bool fun_declaration();

    bool params();

    bool param_list();

    bool param_list_recur();

    bool param();

    bool compound_stmt();

    bool local_declarations();

    bool local_declarations_recur();

    bool statement_list_recur();

    bool statement_list();

    bool statement();

    bool expression_stmt();

    bool selection_stmt();

    bool iteration_stmt();

    bool return_stmt();

    bool expression();

    bool var();

    bool simple_expression();

    bool re_lop();

    bool additive_expression();

    bool additive_expression_recur();

    bool add_op();

    bool term();

    bool term_recur();

    bool mul_op();

    bool factor();

    bool call();

    bool args();

    bool arg_list();

    bool arg_list_recur();

public:
// Constructor
    Parser(Lex& lex) : lex(&lex) {
        this->curr_token = lex.get_token();
    };

    void parse();
};


