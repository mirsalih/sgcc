#include <memory>
#include <exception>
#include "parser.h"

namespace sgcc
{

Parser::Parser(std::vector<Token> t): tokens(t)
{
    //TODO: at advance change this with project related exceptions.
    if(t.empty()) throw std::runtime_error("empty token container!");
    current = tokens.begin();
}

/**
 * @brief Formal grammar for <program> ::= <function>
 * 
 * @return std::unique_ptr<Program> 
 */
std::unique_ptr<Program> Parser::parseProgram()
{
    auto function = parseFunction();
    if (function) {
        return std::make_unique<Program>(std::move(function));
    }
    return nullptr;
}

void Parser::advance() {
    if(current != tokens.end()) current++;
}

/**
 * @brief Formal grammar for <function> ::= "int" <id> "(" ")" "{" <statement> "}"
 * 
 * @return std::unique_ptr<Function> 
 */
std::unique_ptr<Function> Parser::parseFunction() {
    if (current->type != TokenType::INT_KEYWORD) return nullptr;
    advance();
    if (current->type != TokenType::IDENTIFIER) return nullptr;
    std::string name = current->text;
    advance();
    if(current->type != TokenType::OPEN_PAREN) return nullptr;
    advance();
    if(current->type != TokenType::CLOSE_PAREN) return nullptr;
    advance();
    if (current->type != TokenType::OPEN_BRACE) return nullptr;
    advance();
    auto statement = parseStatement();
    if (current->type != TokenType::CLOSE_BRACE) return nullptr;
    advance();
    return std::make_unique<Function>(name, std::move(statement));
}

/**
 * @brief Formal grammar for <statement> ::= "return" <exp> ";"
 * 
 * @return std::unique_ptr<Statement> 
 */
std::unique_ptr<Statement> Parser::parseStatement() {
    if (current->type != TokenType::RETURN_KEYWORD) return nullptr;
    advance();
    auto expression = parseExpression();
    if (current->type != TokenType::SEMICOLON) return nullptr;
    advance();
    return std::make_unique<Return>(std::move(expression));
}

/**
 * @brief Formal grammar for <exp> ::= <int>
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseExpression() {
    if (current->type != TokenType::NUMBER) return nullptr;
    int value = std::stoi(current->text);
    advance();
    return std::make_unique<Constant>(value);
}
}
