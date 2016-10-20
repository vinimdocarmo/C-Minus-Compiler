#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <regex>
#include "../../utils/DFA/DFA.h"

using namespace std;

enum Token_Type {
    TK_OP_INVALID,
    TK_SYMBOL, // ; , ( ) [ ] { }
    TK_OP_ALGEBRAIC, // +, -, *, /
    TK_OP_ASSIGNMENT, // =
    TK_OP_RELATIONAL, // <=, >=, ==, !=
    TK_WHITESPACE,    // white space
    TK_ID,              // identifier
    TK_NUMBER,       // number
    TK_RESERVED_W, // if, int, void, read, write, while, return
    TK_EOS,              // $, End of String
};

extern const vector<string> token_tostring;
extern const map<string, Token_Type> keywords_map;

struct Token {
    Token_Type type;
    string lexeme;
};

class Lex {
public:
    Token get_token();

    string get_lexeme() { return lexeme; }

    string token_stringfy(Token_Type t);

// Constructor
    Lex(const string &source_filename);

private:
    // various member variables
    string lexeme;

    stringstream source_ss;

    DFA<char> dfa;

    int initial_state = 0;

    void add_states();

    void add_identifier_rule();

    void add_symbol_rule();

    void add_operation_rules();

    void add_white_space_rule();

    void add_number_rule();

// Get next character from string stream
    inline char get_char() {
        return static_cast<char>(source_ss.get());
    }

    inline char get_next_char() {
        return static_cast<char>(source_ss.peek());
    }
};