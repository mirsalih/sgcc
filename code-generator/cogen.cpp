#include "cogen.h"

#include <exception>
#include <vector>
#include <sstream>

namespace sgcc
{

void exprToInstruction(std::ostream& os, const Exp& expr) {
    using namespace std;

    if(expr.kind() == ExprKind::BINARY_OP) {
        const auto& op = static_cast<const BinaryOp&>(expr);
        
        exprToInstruction(os, *op.leftExpr);
        os << "push %rax\n";
        exprToInstruction(os, *op.rightExpr);
        os << "pop %rcx\n";

        if(op.oprtor == Operator::ADDITION) {
            os << "addl %ecx, %eax\n";
        }
        else if(op.oprtor == Operator::SUBTRACTION) {
            os << "subl %eax, %ecx\n";
            os << "movl %ecx, %eax\n";
        }
        else if(op.oprtor == Operator::MULTIPLICATION) {
            os << "imul %ecx, %eax\n";
        }
        else {
            // swap ecx with eax
            os << "push %rax\n";
            os << "movl %ecx, %eax\n";
            os << "pop %rcx\n";

            // start division
            os << "cdq\n"; // extand eax into the edx by care the sign of value
            os << "idivl %ecx\n"; // remiander to edx, quotient to eax
        }
    }
    else if(expr.kind() == ExprKind::UNARY_OP) {
        const UnaryOp& op = static_cast<const UnaryOp&>(expr);

        exprToInstruction(os, *op.expression);
        if(op.oprtor == Operator::NEGATION) {
            os << "neg %eax\n";
        }
        else if(op.oprtor == Operator::BWISE_COMPLEMENT) {
            os << "not %eax\n";
        }
        else { // Operator::LOGICAL_NEGATION
            os << "cmpl $0, %eax\nmov $0, %eax\nsetz %al\n";
        }
    }
    else { // Constant
        const Constant& c = static_cast<const Constant&>(expr);
        os << "movl $" << c.value << ", %eax\n";
    }
}

Cogen::Cogen(std::ostream& os, const Program& p)
{
    char prefix = ' ';
    if(p.function->name != "main") {
        os << ".globl main\n" << "main:\n";
        prefix = '_';
    }
    os << ".globl " << prefix << p.function->name << '\n';
    os << prefix << p.function->name << ":\n";
        
    
    try {
        Return& r = dynamic_cast<Return&>(*p.function->body);
        exprToInstruction(os, *r.expression);
        os << "ret\n";
    }
    catch(const std::bad_cast& e) {
        os << "casting failed " << e.what();
    }
}

}