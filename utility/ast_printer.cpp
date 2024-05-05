#include "ast_printer.h"
#include "../parser/ast_nodes.h"

namespace sgcc
{
std::ostream& print(std::ostream& os, const Node& ast)
{
    const Program& p = dynamic_cast<const Program&>(ast);
    os << "FUN INT " << p.function->name << ":\n";
    os << "\tparams: ()\n";
    os << "\tbody:\n";
    const Return& r = dynamic_cast<const Return&>(*p.function->body);
    const Constant& c = dynamic_cast<const Constant&>(*r.expression);
    os << "\t\tRETURN Int<" << c.value <<">\n";
    return os;
}

}