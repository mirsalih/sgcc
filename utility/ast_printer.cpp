#include "ast_printer.h"
#include "../parser/ast_nodes.h"

namespace sgcc
{
std::ostream& printExpr(std::ostream& os, const Exp& expr)
{
    if(expr.kind() == ExprKind::UnaryOp) {
        const auto& op = static_cast<const UnaryOp&>(expr);

        if(op.oprtor == Operator::NEGATION) {
            os << "OP<-> ";
        }
        if(op.oprtor == Operator::LOGICAL_NEGATION) {
            os << "OP<!> ";
        }
        if(op.oprtor == Operator::BWISE_COMPLEMENT) {
            os << "OP<~> ";
        }
        return printExpr(os, *op.expression);
    }
    else {
        const auto& c = static_cast<const Constant&>(expr);
        os << "Int<" << c.value <<">\n";
        return os;
    }
}

std::ostream& print(std::ostream& os, const Node& ast)
{
    const Program& p = dynamic_cast<const Program&>(ast);
    os << "FUN INT " << p.function->name << ":\n";
    os << "\tparams: ()\n";
    os << "\tbody:\n";
    const Return& r = dynamic_cast<const Return&>(*p.function->body);
    os << "\t\tRETURN ";
    printExpr(os, *r.expression);

    return os;
}

}