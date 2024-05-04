#ifndef __LEXER_H__
#define __LEXER_H__

#include <vector>
#include <cctype>

#include "token.h"

namespace sgcc
{
class Lexer {
public:
    std::vector<Token> tokenize(const std::string&);

private:
    void advance();
    void skipWhitespace();
    Token identifier();
    Token number();

private:
    std::string input;
    std::size_t pos;
    char currentChar;
};
}
#endif // __LEXER_H__