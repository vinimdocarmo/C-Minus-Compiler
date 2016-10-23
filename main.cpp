#include "compiler/lexer/lex.h"
#include "compiler/parser/parser.h"

#define DEBUG_MODE true

using namespace std;

void DEBUG_TOKENS(vector<Token>);

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        cerr << "You must provide the file as an argument" << endl;
        exit(EXIT_FAILURE);
    }

    Lex lex{argv[1]};

    vector<Token> tokens = lex.get_tokens();

    DEBUG_TOKENS(tokens);

    Parser parser{tokens};

    parser.parse();

    exit(EXIT_SUCCESS);
}

void DEBUG_TOKENS(vector<Token> tokens) {
    if (!DEBUG_MODE)
        return;

    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens.at(i).lexeme << " -> " << token_tostring[static_cast<int>(tokens.at(i).type)] << endl;
    }
}