#ifndef __AST_PARSER_H__
#define __AST_PARSER_H__

#include <vector>
#include <memory.h>
#include "ast_nodes.h"
#include "../lexer/token.h"

namespace sgcc
{
class Parser {
public:
    explicit Parser(std::vector<Token>);
    std::unique_ptr<Program> parseProgram();

private:
    void advance();
    std::unique_ptr<Function> parseFunction();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Exp> parseExpression();

    const std::vector<Token> tokens;
    std::vector<Token>::const_iterator current;
};
}

#endif // __AST_PARSER_H__