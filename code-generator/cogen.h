#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include <iosfwd>
#include <../parser/ast_nodes.h>

namespace sgcc
{
class Cogen {
public:
    explicit Cogen(std::ostream&, const Program&);
};
}

#endif // __CODE_GENERATOR_H__