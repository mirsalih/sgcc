#include <memory>
#include <exception>

#include "parser.h"
#include "../lexer/token.h"

namespace sgcc
{

Operator convertToBinaryOperator(const std::string& str) {
    if(str == "+")  return Operator::ADDITION;
    if(str == "-")  return Operator::SUBTRACTION;
    if(str == "*")  return Operator::MULTIPLICATION;
    if(str == "||") return Operator::OR;
    if(str == "&&") return Operator::AND;
    if(str == "==") return Operator::EQUAL;
    if(str == "!=") return Operator::NOT_EQUAL;
    if(str == "<")  return Operator::LESS_THAN;
    if(str == "<=") return Operator::LESS_THAN_OR_EQUAL;
    if(str == ">")  return Operator::GREATER_THAN;
    if(str == ">=") return Operator::GREATER_THAN_OR_EQUAL;
    
    throw std::runtime_error("Failed to Conver Token to BinaryOp: " + str);
}

Operator convertToUnaryOperator(const std::string& str) {
    if(str == "~") return Operator::BWISE_COMPLEMENT;
    if(str == "-") return Operator::NEGATION;
    if(str == "!") return Operator::LOGICAL_NEGATION;
    
    throw std::runtime_error("Failed to Conver Token to UnaryOp: " + str);
}
/**
 * @brief Formal grammar for <program> ::= <function>
 * 
 * @return std::unique_ptr<Program> 
 */
std::unique_ptr<Program> Parser::parseProgram(const std::vector<Token>& tokens)
{
    //TODO: at advance change this with project related exceptions.
    if(tokens.empty()) throw std::runtime_error("empty token container!");
    current = tokens.cbegin();
    end = tokens.cend();

    auto function = parseFunction();
    if (function) {
        return std::make_unique<Program>(std::move(function));
    }
    return nullptr;
}

void Parser::advance() {
    if(current != end) ++current;
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
    if(!statement) return nullptr;
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
 * @brief Formal grammar for <exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseExpression() {
    using namespace std;

    auto logicalAnd = parseLogicalAnd();
    while(current->text == "||") {
        advance();
        logicalAnd = make_unique<BinaryOp>(Operator::OR, move(logicalAnd), parseLogicalAnd());
    }
    return logicalAnd;
}

/**
 * @brief Formal grammar for <logical-and-exp> ::= <equality-exp> { "&&" <equality-exp> }
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseLogicalAnd() {
    using namespace std;

    auto equality = parseEquality();
    while(current->text == "&&") {
        advance();
        equality = make_unique<BinaryOp>(Operator::AND, move(equality), parseEquality());
    }
    return equality;
}

/**
 * @brief Formal grammar for <equality-exp> ::= <relational-exp> { ("!=" | "==") <relational-exp> }
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseEquality() {
    using namespace std;

    auto relational = parseRelational();
    while(current->text == "!=" || current->text == "==") {
        const auto op = convertToBinaryOperator(current->text);
        advance();
        relational = make_unique<BinaryOp>(op, move(relational), parseRelational());
    }
    return relational;
}

/**
 * @brief Formal grammar for <relational-exp> ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseRelational() {
    using namespace std;

    auto additive = parseAdditive();
    while(current->text == "<" || current->text == ">" || current->text == "<="
          || current->text == ">=") {
        const auto op = convertToBinaryOperator(current->text);
        advance();        
        additive = make_unique<BinaryOp>(op, move(additive), parseAdditive());
    }
    return additive;
}

/**
 * @brief Formal grammar for <additive-exp> ::= <term> { ("+" | "-") <term> }
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseAdditive() {
        using namespace std;

    auto term = parseTerm();
    while(current->text == "-" || current->text == "+") {
        const auto op = convertToBinaryOperator(current->text);
        advance();        
        term = make_unique<BinaryOp>(op, move(term), parseTerm());
    }
    return term;
}

/**
 * @brief Formal grammar for <term> ::= <factor> { ("*" | "/") <factor> }
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseTerm() {
    using namespace std;

    auto factor = parseFactor();
    while(current->text == "*" || current->text == "/") {
        const auto op = convertToBinaryOperator(current->text);
        advance();
        factor = make_unique<BinaryOp>(op, move(factor), parseFactor());
    }
    return factor;
}

/**
 * @brief Formal grammar for <factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int>
 * 
 * @return std::unique_ptr<Exp> 
 */
std::unique_ptr<Exp> Parser::parseFactor() {
    using namespace std;

    if(current->type == TokenType::OPEN_PAREN) {
        advance();
        auto expr = parseExpression();
        if(current->type != TokenType::CLOSE_PAREN) throw runtime_error("expected a ')'");
        advance();
        
        return expr;
    }
    else if(current->text == "~" || current->text == "-" || current->text == "!") {
        const auto op = convertToUnaryOperator(current->text);
        advance();
        if (auto factor = parseFactor()) {
            return make_unique<UnaryOp>(op, move(factor));
        }
    }
    else if(current->type == TokenType::NUMBER) {
        int value = stoi(current->text);
        advance();
        return make_unique<Constant>(value);
    }
    
    throw runtime_error("expected a factor!");
}

}
