#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <string>
#include <iostream>

namespace sgcc
{
enum class TokenType {
    INT_KEYWORD, IDENTIFIER, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACE, RETURN_KEYWORD,
    NUMBER, END, SEMICOLON, CLOSE_BRACE, OPERATOR,
};

static constexpr const char* tokenTypesStr[] = {
    "INT_KEYWORD", "IDENTIFIER", "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BRACE", "RETURN_KEYWORD",
    "NUMBER", "END", "SEMICOLON", "CLOSE_BRACE", "OPERATOR"
};

struct Token {
    TokenType type;
    std::string text;
};

std::ostream& operator<<(std::ostream&, TokenType);
}
#endif // __TOKENS_H__
