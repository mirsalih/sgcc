#include "cogen.h"
#include "../utility/label_generator.h"

#include <exception>
#include <vector>
#include <sstream>

namespace sgcc
{

std::string exprToInstruction(const Exp& expr) {
    using namespace std;
    ostringstream os{};

    if(expr.kind() == ExprKind::BINARY_OP) {
        const auto& op = static_cast<const BinaryOp&>(expr);
        
        auto leftIns = exprToInstruction(*op.leftExpr);
        auto rightIns = exprToInstruction(*op.rightExpr);

        if(op.oprtor == Operator::ADDITION) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "addl %ecx, %eax\n";
        }
        else if(op.oprtor == Operator::SUBTRACTION) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "subl %eax, %ecx\n";
            os << "movl %ecx, %eax\n";
        }
        else if(op.oprtor == Operator::MULTIPLICATION) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "imul %ecx, %eax\n";
        }
        else if(op.oprtor == Operator::DIVISION) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";

            // swap ecx with eax
            os << "push %rax\n";
            os << "movl %ecx, %eax\n";
            os << "pop %rcx\n";

            // start division
            os << "cdq\n"; // extand eax into the edx by care the sign of value
            os << "idivl %ecx\n"; // remiander to edx, quotient to eax
        }
        else if(op.oprtor == Operator::EQUAL) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "cmpl %eax, %ecx\n";
            os << "mov $0, %eax\n";
            os << "setz %al\n";
        }
        else if(op.oprtor == Operator::NOT_EQUAL) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "cmpl %eax, %ecx\n";
            os << "mov $0, %eax\n";
            os << "setne %al\n";
        }
        else if (op.oprtor == Operator::GREATER_THAN) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "cmpl %eax, %ecx\n";
            os << "mov $0, %eax\n";
            os << "setg %al\n";
        }
        else if(op.oprtor == Operator::GREATER_THAN_OR_EQUAL) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "cmpl %eax, %ecx\n";
            os << "mov $0, %eax\n";
            os << "setge %al\n";
        }
        else if(op.oprtor == Operator::LESS_THAN) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "cmpl %eax, %ecx\n";
            os << "mov $0, %eax\n";
            os << "setl %al\n";
        }
        else if(op.oprtor == Operator::LESS_THAN_OR_EQUAL) {
            os << leftIns << "push %rax\n" << rightIns << "pop %rcx\n";
            os << "cmpl %eax, %ecx\n";
            os << "mov $0, %eax\n";
            os << "setle %al\n";
        }
        else if(op.oprtor == Operator::OR) {
            const LabelGen lgen{};
            const auto& jumpRightExpr = lgen();
            const auto& jumpEndExpr = lgen();

            os << leftIns;
            os << "cmpl $0, %eax\n"; // check if left expr is true
            os << "je " << jumpRightExpr <<"\n"; // if left expr is true evaluate clause 2
            os << "movl $1, %eax\n"; // we didn't jump, so left expr is true and therefore result is 1
            os << "jmp " << jumpEndExpr <<"\n";
            os << jumpRightExpr << ":\n";
            os << rightIns;
            os << "cmpl $0, %eax\n";            // check if right expr is true
            os << "movl $0, %eax\n";            // zero out EAX without changing ZF(zero flag)
            os << "setne %al\n";
            os << jumpEndExpr << ":\n";
        }
        else if(op.oprtor == Operator::AND) {
            const LabelGen lgen{};
            const auto& jumpRightExpr = lgen();
            const auto& jumpEndExpr = lgen();

            os << leftIns;
            os << "cmpl $0, %eax\n"; // check if left expr is true
            os << "jne " << jumpRightExpr <<"\n"; // if left expr is false evaluate clause 2
            os << "jmp " << jumpEndExpr <<"\n";
            os << jumpRightExpr << ":\n";
            os << rightIns;
            os << "cmpl $0, %eax\n";            // check if right expr is true
            os << "movl $0, %eax\n";            // zero out EAX without changing ZF(zero flag)
            os << "setne %al\n";
            os << jumpEndExpr << ":\n";
        }
    }
    else if(expr.kind() == ExprKind::UNARY_OP) {
        const UnaryOp& op = static_cast<const UnaryOp&>(expr);

        auto expr = exprToInstruction(*op.expression);
        if(op.oprtor == Operator::NEGATION) {
            os << expr;
            os << "neg %eax\n";
        }
        else if(op.oprtor == Operator::BWISE_COMPLEMENT) {
            os << expr;
            os << "not %eax\n";
        }
        else { // Operator::LOGICAL_NEGATION
            os << expr;
            os << "cmpl $0, %eax\n";
            os << "mov $0, %eax\n";
            os << "setz %al\n";
        }
    }
    else { // Constant
        const Constant& c = static_cast<const Constant&>(expr);
        os << "movl $" << c.value << ", %eax\n";
    }

    return os.str();
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
        os << exprToInstruction(*r.expression);
        os << "ret\n";
    }
    catch(const std::bad_cast& e) {
        os << "casting failed " << e.what();
    }
}

}