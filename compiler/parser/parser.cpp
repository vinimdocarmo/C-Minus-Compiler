
#include "parser.h"

void Parser::consume_token() {
    do {
        curr_index_token++;
        curr_token = tokens.at(curr_index_token);
    } while (curr_token.type == Token_Type::TK_WHITESPACE);
}

bool Parser::terminal(Token_Type token_type) {
    bool isEqual = match(token_type);

    if (isEqual) {
        consume_token();
    }

    return isEqual;
}

bool Parser::terminal(string lexeme) {
    bool isEqual = curr_token.lexeme == lexeme;

    if (isEqual) {
        consume_token();
    }

    return isEqual;
}

void Parser::backtrack(int backtracked_index) {
    curr_index_token = backtracked_index;
    curr_token = tokens.at(curr_index_token);
}

bool Parser::match(Token_Type type) {
    return curr_token.type == type;
}

void Parser::parse() {
    if (program() && match(Token_Type::TK_EOS)) {
        cout << "Compilation success" << endl;
    } else {
        cerr << "Compilation error" << endl;
        exit(EXIT_FAILURE);
    }
}

// program → declaration-list
bool Parser::program() {
    return declaration_list();
}

// declaration-list → declaration declaration-list-recur | declaration
bool Parser::declaration_list() {
    unsigned int saved_index = curr_index_token;

    return (declaration() && declaration_list_recur())
           || (backtrack(saved_index), declaration());
}

// declaration-list-recur → declaration declaration-list-recur | declaration
bool Parser::declaration_list_recur() {
    unsigned int saved_index = curr_index_token;

    return (declaration() && declaration_list_recur())
            || (backtrack(saved_index), declaration());
}

// declaration → var-declaration | fun-declaration
bool Parser::declaration() {
    unsigned int saved_index = curr_index_token;

    return (var_declaration())
           || (backtrack(saved_index), fun_declaration());
}

// var-declaration → type-specifier ID ; | type-specifier ID [ NUM ] ;
bool Parser::var_declaration() {
    unsigned int saved_index = curr_index_token;

    return (type_specifier() && terminal(Token_Type::TK_ID) && terminal(";"))
           || (backtrack(saved_index), type_specifier() && terminal(Token_Type::TK_ID) && terminal("[") &&
                                   terminal(Token_Type::TK_NUMBER) && terminal("]") && terminal(";"));
}

// type-specifier → int | void
bool Parser::type_specifier() {
    return terminal("int") || terminal("void");
}

// fun-declaration → type-specifier ID ( params ) compound-stmt
bool Parser::fun_declaration() {
    return type_specifier() && terminal(Token_Type::TK_ID) && terminal("(") && params() && terminal(")") &&
           compound_stmt();
}

// params → param-list | void
bool Parser::params() {
    unsigned int saved_index = curr_index_token;

    return (param_list())
           || (backtrack(saved_index), terminal("void"));
}

// param-list → param param-list-recur | param
bool Parser::param_list() {
    unsigned int saved_index = curr_index_token;

    return (param() && param_list_recur())
           || (backtrack(saved_index), param());
}


// param-list-recur → , param param-list-recur | , param
bool Parser::param_list_recur() {
    unsigned int saved_index = curr_index_token;

    return (terminal(",") && param() && param_list_recur())
           || (backtrack(saved_index), terminal(",") && param());
}

// param → type-specifier ID | type-specifier ID [ ]
bool Parser::param() {
    unsigned int saved_index = curr_index_token;

    return (type_specifier() && terminal(Token_Type::TK_ID))
           || (backtrack(saved_index), type_specifier() && terminal(Token_Type::TK_ID) && terminal("[") && terminal("]"));
}

// { local-declarations statement-list } | { local-declarations } | { statement-list } | { }
bool Parser::compound_stmt() {
    unsigned int saved_index = curr_index_token;

    return (terminal("{") && local_declarations() && statement_list() && terminal("}"))
           || (backtrack(saved_index), terminal("{") && local_declarations() && terminal("}"))
           || (backtrack(saved_index), terminal("{") && statement_list() && terminal("}"))
           || (backtrack(saved_index), terminal("{") && terminal("}"));
}

// local-declarations → var-declaration local-declarations-recur | var-declaration
bool Parser::local_declarations() {
    unsigned int saved_index = curr_index_token;

    return (var_declaration() && local_declarations_recur())
           || (backtrack(saved_index), var_declaration());
}

// local-declarations → var-declaration local-declarations-recur | var-declaration
bool Parser::local_declarations_recur() {
    unsigned int saved_index = curr_index_token;

    return (var_declaration() && local_declarations_recur())
           || (backtrack(saved_index), var_declaration());
}

// statement-list → statement statement-list-recur | statement
bool Parser::statement_list() {
    unsigned int saved_index = curr_index_token;

    return (statement() && statement_list_recur())
           || (backtrack(saved_index), statement());
}

// statement-list → statement statement-list-recur | statement
bool Parser::statement_list_recur() {
    unsigned int saved_index = curr_index_token;

    return (statement() && statement_list_recur())
           || (backtrack(saved_index), statement());
}

// statement → expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt
bool Parser::statement() {
    unsigned int saved_index = curr_index_token;

    return (expression_stmt())
           || (backtrack(saved_index), compound_stmt())
           || (backtrack(saved_index), selection_stmt())
           || (backtrack(saved_index), iteration_stmt())
           || (backtrack(saved_index), return_stmt())
           || (backtrack(saved_index), read_stmt())
           || (backtrack(saved_index), write_stmt());
}

// expression-stmt → expression ; | ;
bool Parser::expression_stmt() {
    unsigned int saved_index = curr_index_token;

    return (expression() && terminal(";"))
           || (backtrack(saved_index), terminal(";"));
}

// selection-stmt → if ( expression ) statement selection-stmt-expanded
bool Parser::selection_stmt() {
    return terminal("if") && terminal("(") && expression() && terminal(")") && statement() && selection_stmt_expanded();
}

bool Parser::selection_stmt_expanded() {
    bool empty = true;
    return (terminal("else") && expression_stmt()) || empty;
}

// iteration-stmt → while ( expression ) statement
bool Parser::iteration_stmt() {
    return terminal("while") && terminal("(") && expression() && terminal(")") && statement();
}

// return-stmt → return ; | return expression ;
bool Parser::return_stmt() {
    unsigned int saved_index = curr_index_token;

    return (terminal("return") && terminal(";"))
           || (backtrack(saved_index), terminal("return") && expression() && terminal(";"));
}


// read-stmt → read ( ID ) ;
bool Parser::read_stmt() {
    return terminal("read") && terminal("(") && var() && terminal(")") && terminal(";");
}

// write-stmt → write ( expression ) ;
bool Parser::write_stmt() {
    return terminal("write") && terminal("(") && expression() && terminal(")") && terminal(";");
}

// expression → var = expression | simple-expression
bool Parser::expression() {
    unsigned int saved_index = curr_index_token;

    return (var() && terminal("=") && expression())
           || (backtrack(saved_index), simple_expression());
}

// var → ID | ID [ expression ]
bool Parser::var() {
    unsigned int saved_index = curr_index_token;

    return (terminal(Token_Type::TK_ID))
           || (backtrack(saved_index), terminal(Token_Type::TK_ID) && terminal("[") && expression() && terminal("]"));
}

// simple-expression → additive-expression relop additive-expression | additive-expression
bool Parser::simple_expression() {
    unsigned int saved_index = curr_index_token;

    return (additive_expression() && re_lop() && additive_expression())
           || (backtrack(saved_index), additive_expression());
}

// relop → <= | < | > | >= | == | !=
bool Parser::re_lop() {
    unsigned int saved_index = curr_index_token;

    return (terminal("<="))
           || (backtrack(saved_index), terminal("<"))
           || (backtrack(saved_index), terminal(">"))
           || (backtrack(saved_index), terminal(">="))
           || (backtrack(saved_index), terminal("=="))
           || (backtrack(saved_index), terminal("!="));
}

// additive-expression → term additive-expression-recur | term
bool Parser::additive_expression() {
    unsigned int saved_index = curr_index_token;

    return (term() && additive_expression_recur())
           || (backtrack(saved_index), term());
}

// additive-expression-recur → addop term additive-expression-recur | addop term
bool Parser::additive_expression_recur() {
    unsigned int saved_index = curr_index_token;

    return (add_op() && term() && additive_expression_recur())
           || (backtrack(saved_index), add_op() && term());
}

// addop → + | -
bool Parser::add_op() {
    unsigned int saved_index = curr_index_token;

    return (terminal("+"))
           || (backtrack(saved_index), terminal("-"));
}

// term → factor term-recur | factor
bool Parser::term() {
    unsigned int saved_index = curr_index_token;

    return (factor() && term_recur())
           || (backtrack(saved_index), factor());
}


// term-recur → mulop factor term-recur | mulop factor
bool Parser::term_recur() {
    unsigned int saved_index = curr_index_token;

    return (mul_op() && factor() && term_recur())
           || (backtrack(saved_index), mul_op() && factor());
}

// mulop → * | /
bool Parser::mul_op() {
    unsigned int saved_index = curr_index_token;

    return (terminal("*"))
           || (backtrack(saved_index), terminal("/"));
}

// factor → ( expression ) | var | call | NUM
bool Parser::factor() {
    unsigned int saved_index = curr_index_token;

    return (terminal("(") && expression() && terminal(")"))
           || (backtrack(saved_index), var())
           || (backtrack(saved_index), call())
           || (backtrack(saved_index), terminal(Token_Type::TK_NUMBER));
}

// call → ID ( args ) | ID ( )
bool Parser::call() {
    unsigned int saved_index = curr_index_token;

    return (terminal(Token_Type::TK_ID) && terminal("(") && args() && terminal(")"))
           || (backtrack(saved_index), terminal(Token_Type::TK_ID) && terminal("(") && terminal(")"));
}

// args → arg-list
bool Parser::args() {
    return arg_list();
}

// arg-list → expression arg-list-recur | expression
bool Parser::arg_list() {
    unsigned int saved_index = curr_index_token;

    return (expression() && arg_list_recur())
           || (backtrack(saved_index), expression());
}

// arg-list-recur → , expression arg-list-recur | , expression
bool Parser::arg_list_recur() {
    unsigned int saved_index = curr_index_token;

    return (terminal(",") && expression() && arg_list_recur())
           || (backtrack(saved_index), terminal(",") && expression());
}