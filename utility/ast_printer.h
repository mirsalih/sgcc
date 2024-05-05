#ifndef __AST_PRINTER_H__
#define __AST_PRINTER_H__
#include <iosfwd>

namespace sgcc
{
struct Node;

std::ostream& print(std::ostream&, const Node&);
}
#endif // __AST_PRINTER_H__