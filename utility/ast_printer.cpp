#include "ast_printer.h"
#include "../parser/ast_nodes.h"

namespace sgcc
{
std::ostream& printExpr(std::ostream& os, const Exp& expr) {
    if(expr.kind() == ExprKind::BINARY_OP) {
        const auto& op = static_cast<const BinaryOp&>(expr);

        if(op.oprtor == Operator::ADDITION)                 os << "+(";
        if(op.oprtor == Operator::SUBTRACTION)              os << "-(";
        if(op.oprtor == Operator::MULTIPLICATION)           os << "*(";
        if(op.oprtor == Operator::DIVISION)                 os << "/(";
        if(op.oprtor == Operator::OR)                       os << "||(";
        if(op.oprtor == Operator::AND)                      os << "&&(";
        if(op.oprtor == Operator::EQUAL)                    os << "==(";
        if(op.oprtor == Operator::NOT_EQUAL)                os << "!=(";
        if(op.oprtor == Operator::LESS_THAN)                os << "<(" ;
        if(op.oprtor == Operator::LESS_THAN_OR_EQUAL)       os << "<=(";
        if(op.oprtor == Operator::GREATER_THAN)             os << ">(" ;
        if(op.oprtor == Operator::GREATER_THAN_OR_EQUAL)    os << ">=(";
        
        printExpr(os, *op.leftExpr);
        os << ", ";
        printExpr(os, *op.rightExpr);
        os << ")";
    }
    else if(expr.kind() == ExprKind::UNARY_OP) {
        const auto& op = static_cast<const UnaryOp&>(expr);

        if(op.oprtor == Operator::NEGATION)         os << "-(";
        if(op.oprtor == Operator::LOGICAL_NEGATION) os << "!(";
        if(op.oprtor == Operator::BWISE_COMPLEMENT) os << "~(";

        printExpr(os, *op.expression);
        os << ")";
    }
    else {
        const auto& c = static_cast<const Constant&>(expr);
        os << "Int<" << c.value <<">";
    }
    
    return os;
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
    os << "\n";

    return os;
}

}