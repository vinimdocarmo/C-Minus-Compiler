#include "lex.h"

using namespace std;

// Note: order of the vector entries is tied to order of Token_Type constants!
const vector<string> token_tostring{
        "TK_OP_INVALID",
        "TK_SYMBOL",
        "TK_OP_ALGEBRAIC",
        "TK_OP_ASSIGNMENT",
        "TK_OP_RELATIONAL",
        "TK_WHITESPACE",
        "TK_ID",
        "TK_NUMBER",
        "TK_RESERVED_W",
        "TK_EOS"
};

const map<string, Token_Type> keywords_map{
        {"if",     Token_Type::TK_RESERVED_W},
        {"else",     Token_Type::TK_RESERVED_W},
        {"int",    Token_Type::TK_RESERVED_W},
        {"void",   Token_Type::TK_RESERVED_W},
        {"read",   Token_Type::TK_RESERVED_W},
        {"write",   Token_Type::TK_RESERVED_W},
        {"while",  Token_Type::TK_RESERVED_W},
        {"return", Token_Type::TK_RESERVED_W}
};

using namespace std;

Token Lex::get_token() {

    dfa.reset();

    char c = get_char();

    if (c == '$') {
        Token eos;
        eos.lexeme = "$";
        eos.type = Token_Type::TK_EOS;
        return eos;
    }

    while (!source_ss.eof()) {
        char next = get_next_char();

        lexeme += c;

        dfa.input(c);

        Token tk;

        if (dfa.is_accepting() && !dfa.is_accepting(next)) {
            tk.lexeme = lexeme;
            tk.type = (Token_Type) dfa.state();

            for (const auto word : keywords_map) {
                if (word.first == tk.lexeme) {
                    tk.type = Token_Type::TK_RESERVED_W;
                    break;
                }
            }

            lexeme.clear();

            return tk;
        }

        c = get_char();
    }

    Token invalid_token;

    invalid_token.type = Token_Type::TK_OP_INVALID;

    return invalid_token;
}

vector<Token> Lex::get_tokens() {
    Token token;
    vector<Token> tokens;

    do {
        token = get_token();
        tokens.push_back(token);
    } while (token.type != Token_Type::TK_EOS);

    return tokens;
}

Lex::Lex(const string &source_filename) : dfa(0, false) {

    ifstream source_file(source_filename, ios_base::in);

    if (!source_file) {
        cerr << "Cannot open the file " << source_filename << endl;
        exit(EXIT_FAILURE);
    }

    source_ss << source_file.rdbuf();
    source_ss << "$";

    source_file.close();

    dfa.reset();

    this->add_final_states();
    this->add_identifier_rule();
    this->add_symbol_rule();
    this->add_operation_rules();
    this->add_white_space_rule();
    this->add_number_rule();

}

string Lex::token_stringfy(Token_Type t) {
    return token_tostring[static_cast<int>(t)];
}

void Lex::add_final_states() {
    dfa.add_final_state(Token_Type::TK_SYMBOL);
    dfa.add_final_state(Token_Type::TK_OP_ALGEBRAIC);
    dfa.add_final_state(Token_Type::TK_OP_ASSIGNMENT);
    dfa.add_final_state(Token_Type::TK_OP_RELATIONAL);
    dfa.add_final_state(Token_Type::TK_WHITESPACE);
    dfa.add_final_state(Token_Type::TK_ID);
    dfa.add_final_state(Token_Type::TK_RESERVED_W);
    dfa.add_final_state(Token_Type::TK_NUMBER);
}

void Lex::add_identifier_rule() {
    for (char c = 'a'; c <= 'z'; c++) {
        dfa.add_transition(initial_state, c, Token_Type::TK_ID);
        dfa.add_transition(Token_Type::TK_ID, c, Token_Type::TK_ID);
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        dfa.add_transition(initial_state, c, Token_Type::TK_ID);
        dfa.add_transition(Token_Type::TK_ID, c, Token_Type::TK_ID);
    }

    for (char c = '0'; c <= '9'; c++) {
        dfa.add_transition(Token_Type::TK_ID, c, Token_Type::TK_ID);
    }
}

void Lex::add_symbol_rule() {
    for (const char symbol : {'(', ')', '[', ']', ',', ';', '{', '}'}) {
        dfa.add_transition(initial_state, symbol, Token_Type::TK_SYMBOL);
    }
}

void Lex::add_operation_rules() {
    dfa.add_transition(initial_state, '=', Token_Type::TK_OP_ASSIGNMENT);

    for (const char op : {'+', '-', '*', '/'}) {
        dfa.add_transition(initial_state, op, Token_Type::TK_OP_ALGEBRAIC);
    };

    for (const string op : {">=", "<=", "==", "!="}) {
        dfa.add_transition(initial_state, op[0], Token_Type ::TK_OP_RELATIONAL);
        dfa.add_transition(Token_Type ::TK_OP_RELATIONAL, op[1], Token_Type ::TK_OP_RELATIONAL);
    }
}

void Lex::add_white_space_rule() {
    dfa.add_transition(initial_state, ' ', Token_Type::TK_WHITESPACE);
    dfa.add_transition(initial_state, '\n', Token_Type::TK_WHITESPACE);
    dfa.add_transition(Token_Type::TK_WHITESPACE, ' ', Token_Type::TK_WHITESPACE);
}

void Lex::add_number_rule() {
    for (char c = '0'; c <= '9'; c++) {
        dfa.add_transition(initial_state, c, Token_Type::TK_NUMBER);
        dfa.add_transition(Token_Type::TK_NUMBER, c, Token_Type::TK_NUMBER);
    }
}

