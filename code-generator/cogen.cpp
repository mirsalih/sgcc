#include "cogen.h"

#include <exception>
#include <vector>
#include <sstream>

namespace sgcc
{

void exprToInstruction(std::vector<std::string>& vec, const Exp& expr) {
    using namespace std;

    if(expr.kind() == ExprKind::UnaryOp) {
        const UnaryOp& op = static_cast<const UnaryOp&>(expr);
        if(op.oprtor == Operator::NEGATION) {
            vec.push_back("neg %eax\n");
        }
        else if(op.oprtor == Operator::BWISE_COMPLEMENT) {
            vec.push_back("not %eax\n");
        }
        else { // Operator::LOGICAL_NEGATION
            vec.push_back("cmp $0, %eax\nmov $0, %eax\nsetz %al\n");
        }
        exprToInstruction(vec, *op.expression);
    }
    else { // Constant
        const Constant& c = static_cast<const Constant&>(expr);
        vec.push_back("movl $"s + std::to_string(c.value) + ", %eax\n");
        return;
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
        std::vector<std::string> instructions;
        exprToInstruction(instructions, *r.expression);

        for(auto strIter = instructions.crbegin(); strIter != instructions.crend();
        std::advance(strIter, 1)) {
            os << *strIter;
        }
        os << "ret\n";
    }
    catch(const std::bad_cast& e) {
        os << "casting failed " << e.what();
    }
}

}