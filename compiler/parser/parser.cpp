
#include "parser.h"

Parser::Parser(Lex &lex) : lex(&lex) {
    this->currToken = lex.get_token();
}

void Parser::parse() {
    this->program();
}

void Parser::comsume() {
    //TODO
}

void Parser::program() {
    this->declaration_list();
}

void Parser::declaration_list() {
    this->declaration();
    this->declaration_list_recur();
}

void Parser::declaration() {
    /**
     * TODO: o que fazer quando declaration deriva dos ou mais não terminais?
     *      declaration -> var_declaration | fun_declaration
     *
     *      É necessário usar lookahead? - SIM
     */
}

void Parser::declaration_list_recur() {

}

