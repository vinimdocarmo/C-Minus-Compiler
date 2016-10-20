
#include "parser.h"

void Parser::consume_token() {
    do {
        curr_token = lex->get_token();
    } while (curr_token.type == Token_Type::TK_WHITESPACE);
}

bool Parser::terminal(Token_Type token_type) {
    bool isEqual = curr_token.type == token_type;
    consume_token();
    return isEqual;
}

bool Parser::terminal(string lexeme) {
    bool isEqual = curr_token.lexeme == lexeme;
    consume_token();
    return isEqual;
}

void Parser::parse() {
    if (!program()) {
        cerr << "Erro de compilação!" << endl;
        exit(EXIT_FAILURE);
    } else {
        cout << "Compilação bem sucedida!" << endl;
    };
}

bool Parser::program() {
    return declaration_list();
}

bool Parser::declaration_list() {
    return declaration() && declaration_list_recur();
}

bool Parser::declaration_list_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, declaration() && declaration_list_recur())
           || (curr_token = *save, declaration());
}

bool Parser::declaration() {
    Token *save = &curr_token;

    return (curr_token = *save, var_declaration())
           || (curr_token = *save, fun_declaration());
}

bool Parser::var_declaration() {
    Token *save = &curr_token;

    return (curr_token = *save, type_specifier() && terminal(Token_Type::TK_ID) && terminal(";"))
           || (curr_token = *save, type_specifier() && terminal(Token_Type::TK_ID) && terminal("[") &&
                                   terminal(Token_Type::TK_NUMBER) && terminal("]") && terminal(";"));
}

bool Parser::type_specifier() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("int")) || (curr_token = *save, terminal("void"));
}

bool Parser::fun_declaration() {
    return type_specifier() && terminal(Token_Type::TK_ID) && terminal("(") && params() && terminal(")") &&
           compound_stmt();
}

bool Parser::params() {
    Token *save = &curr_token;

    return (curr_token = *save, param_list())
           || (curr_token = *save, terminal("void"));
}

bool Parser::param_list() {
    Token *save = &curr_token;

    return (curr_token = *save, param() && param_list_recur())
           || (curr_token = *save, param());
}

bool Parser::param_list_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal(",") && param() && param_list_recur())
           || (curr_token = *save, terminal(",") && param());
}

bool Parser::param() {
    Token *save = &curr_token;

    return (curr_token = *save, type_specifier() && terminal(Token_Type::TK_ID))
           || (curr_token = *save, type_specifier() && terminal(Token_Type::TK_ID) && terminal("[") && terminal("]"));
}

bool Parser::compound_stmt() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("{") && local_declarations() && statement_list() && terminal("}"))
           || (curr_token = *save, terminal("{") && local_declarations() && terminal("}"))
           || (curr_token = *save, terminal("{") && terminal("}"));
}

bool Parser::local_declarations() {
    Token *save = &curr_token;

    return (curr_token = *save, var_declaration() && local_declarations_recur())
           || (curr_token = *save, var_declaration());
}

bool Parser::local_declarations_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, var_declaration() && local_declarations_recur())
           || (curr_token = *save, var_declaration());
}

bool Parser::statement_list() {
    Token *save = &curr_token;

    return (curr_token = *save, statement() && statement_list_recur())
           || (curr_token = *save, statement());
}

bool Parser::statement_list_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, statement() && statement_list_recur())
           || (curr_token = *save, statement());
}

bool Parser::statement() {
    Token *save = &curr_token;

    return (curr_token = *save, expression_stmt())
           || (curr_token = *save, compound_stmt())
           || (curr_token = *save, selection_stmt())
           || (curr_token = *save, iteration_stmt())
           || (curr_token = *save, return_stmt());
}

bool Parser::expression_stmt() {
    Token *save = &curr_token;

    return (curr_token = *save, expression() && terminal(";"))
           || (curr_token = *save, terminal(";"));
}

bool Parser::selection_stmt() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("if") && terminal("(") && expression() && terminal(")") && statement())
           || (curr_token = *save, terminal("if") && terminal("(") && expression() && terminal(")") && statement() &&
                                   terminal("else") && statement());
}

bool Parser::iteration_stmt() {
    return terminal("while") && terminal("(") && expression() && terminal(")") && statement();
}

bool Parser::return_stmt() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("return") && terminal(";"))
           || (curr_token = *save, terminal("return") && expression() && terminal(";"));
}

bool Parser::expression() {
    Token *save = &curr_token;

    return (curr_token = *save, var() && terminal("=") && expression())
           || (curr_token = *save, simple_expression());
}

bool Parser::var() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal(Token_Type::TK_ID))
           || (curr_token = *save, terminal(Token_Type::TK_ID) && terminal("[") && expression() && terminal("]"));
}

bool Parser::simple_expression() {
    Token *save = &curr_token;

    return (curr_token = *save, additive_expression() && re_lop() && additive_expression())
           || (curr_token = *save, additive_expression());
}

bool Parser::re_lop() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("<="))
           || (curr_token = *save, terminal("<"))
           || (curr_token = *save, terminal(">"))
           || (curr_token = *save, terminal(">="))
           || (curr_token = *save, terminal("=="))
           || (curr_token = *save, terminal("!="));
}

bool Parser::additive_expression() {
    return term() && additive_expression_recur();
}

bool Parser::additive_expression_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, add_op() && term() && additive_expression_recur())
           || (curr_token = *save, add_op() && term());
}

bool Parser::add_op() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("+"))
           || (curr_token = *save, terminal("-"));
}

bool Parser::term() {
    Token *save = &curr_token;

    return (curr_token = *save, factor() && term_recur())
           || (curr_token = *save, factor());
}

bool Parser::term_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, mul_op() && factor() && term_recur())
           || (curr_token = *save, mul_op() && factor());
}

bool Parser::mul_op() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("*"))
           || (curr_token = *save, terminal("/"));
}

bool Parser::factor() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal("(") && expression() && terminal(")"))
           || (curr_token = *save, var())
           || (curr_token = *save, call())
           || (curr_token = *save, terminal(Token_Type::TK_NUMBER));
}

bool Parser::call() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal(Token_Type::TK_ID) && terminal("(") && args() && terminal(")"))
           || (curr_token = *save, terminal(Token_Type::TK_ID) && terminal("(") && terminal(")"));
}

bool Parser::args() {
    return arg_list();
}

bool Parser::arg_list() {
    Token *save = &curr_token;

    return (curr_token = *save, expression() && arg_list_recur())
           || (curr_token = *save, expression());
}

bool Parser::arg_list_recur() {
    Token *save = &curr_token;

    return (curr_token = *save, terminal(",") && expression() && arg_list_recur())
           || (curr_token = *save, expression());
}