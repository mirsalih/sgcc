#include "lexer.h"

namespace sgcc
{
std::ostream& operator<<(std::ostream& os, TokenType tt)
{
    return os << tokenTypesStr[static_cast<int>(tt)];
}

void Lexer::advance()
{
    pos++;
    if (pos < input.length()) {
        currentChar = input[pos];
    } else {
        currentChar = '\0'; // End of input
    }
}

void Lexer::skipWhitespace()
{
    while (currentChar != '\0' && std::isspace(currentChar)) {
        advance();
    }
}

Token Lexer::identifier()
{
    std::string result;
    while (currentChar != '\0' && (std::isalnum(currentChar) || currentChar == '_')) {
        result += currentChar;
        advance();
    }

    if (result == "int") return {TokenType::INT_KEYWORD, result};
    if (result == "return") return {TokenType::RETURN_KEYWORD, result};
    return {TokenType::IDENTIFIER, result};
}

Token Lexer::number()
{
    std::string result;
    while (currentChar != '\0' && std::isdigit(currentChar)) {
        result += currentChar;
        advance();
    }
    return {TokenType::NUMBER, result};
}

std::vector<Token> Lexer::tokenize(const std::string& str)
{
    std::vector<Token> tokens;
    input = str;
    pos = 0;
    currentChar = input[0];

    while (currentChar != '\0') {
        skipWhitespace();

        if (std::isalpha(currentChar) || currentChar == '_') {
            tokens.push_back(identifier());
        }
        else if (std::isdigit(currentChar)) {
            tokens.push_back(number());
        }
        else {
            switch (currentChar) {
                case ';':
                    tokens.push_back({TokenType::SEMICOLON, ";"});
                    advance();
                    break;
                case '(':
                    tokens.push_back({TokenType::OPEN_PAREN, "("});
                    advance();
                    break;
                case ')':
                    tokens.push_back({TokenType::CLOSE_PAREN, ")"});
                    advance();
                    break;
                case '{':
                    tokens.push_back({TokenType::OPEN_BRACE, "{"});
                    advance();
                    break;
                case '}':
                    tokens.push_back({TokenType::CLOSE_BRACE, "}"});
                    advance();
                    break;
                case '-':
                    tokens.push_back({TokenType::UNARY_OPRATION, "-"});
                    advance();
                    break;
                case '!':
                    tokens.push_back({TokenType::UNARY_OPRATION, "!"});
                    advance();
                    break;
                case '~':
                    tokens.push_back({TokenType::UNARY_OPRATION, "~"});
                    advance();
                    break;
            }
        }
    }

    tokens.push_back({TokenType::END, "<end>"});
    return tokens;
}
}