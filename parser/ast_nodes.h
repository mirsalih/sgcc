#ifndef __AST_NODES_H__
#define __AST_NODES_H__

#include <memory>
#include <string>
#include <iostream>

namespace sgcc
{
struct Node {
    virtual ~Node() = default;
};

struct Exp: Node {
};

//TODO: when is need for other constants convert it to template
struct Constant: Exp {
    int value;
    explicit Constant(int val): value(val)
    {
    }
};

struct Statement: Node {
};

struct Return: Statement {
    std::unique_ptr<Exp> expression;
    explicit Return(std::unique_ptr<Exp> expr): expression(std::move(expr))
    {
    }
};

struct Function: Node {
    std::string name;
    std::unique_ptr<Statement> body;
    Function(std::string n, std::unique_ptr<Statement> b): name(std::move(n)),
                                                           body(std::move(b))
    {
    }
};

struct Program: Node {
    std::unique_ptr<Function> function;
    explicit Program(std::unique_ptr<Function> func): function(std::move(func))
    {
    }
};
}

#endif // __AST_NODES_H__