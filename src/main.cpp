#include <iostream>
#include "../lexer/lexer.h"

using namespace sgcc;
int main() {
    std::string input = "int main( {\n  return 0; }";
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {
        std::cout << "TType: " << token.type << ", Text: '" << token.text << "'" << std::endl;
    }

    return 0;
}