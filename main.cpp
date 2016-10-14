#include "compiler/lexer/lex.h"
#include "compiler/parser/parser.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        cerr << "You must provide the file as an argument" << endl;
        exit(EXIT_FAILURE);
    }

    Lex lex{argv[1]};

    Parser parser{lex};

    parser.parse();

    exit(EXIT_SUCCESS);
}