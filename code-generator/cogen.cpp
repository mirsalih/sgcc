#include "cogen.h"

#include <exception>

namespace sgcc
{

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
        Return* r = dynamic_cast<Return*>(p.function->body.get());
        Constant *c = dynamic_cast<Constant*>(r->expression.get());
        os << "movl $" << c->value << ", %eax\n";
        os << "ret\n";

    }
    catch(const std::bad_cast& e) {
        os << "casting failed " << e.what();
    }
}

}