#ifndef __LEXER_H__
#define __LEXER_H__

#include <vector>
#include <cctype>
#include <iosfwd>

#include "token.h"

namespace sgcc
{
class Lexer {
public:
    explicit Lexer(const std::string&);
    std::vector<Token> tokenize();

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