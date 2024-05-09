#ifndef __AST_PARSER_H__
#define __AST_PARSER_H__

#include <vector>
#include "ast_nodes.h"

namespace sgcc
{
// forward declaration
class Token;

class Parser {
public:
    std::unique_ptr<Program> parseProgram(const std::vector<Token>&);

private:
    void advance();
    std::unique_ptr<Function> parseFunction();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Exp> parseExpression();
    std::unique_ptr<Exp> parseTerm();
    std::unique_ptr<Exp> parseFactor();
    std::unique_ptr<Exp> parseLogicalAnd();
    std::unique_ptr<Exp> parseEquality();
    std::unique_ptr<Exp> parseRelational();
    std::unique_ptr<Exp> parseAdditive();

    std::vector<Token>::const_iterator current;
    std::vector<Token>::const_iterator end;
};
}

#endif // __AST_PARSER_H__