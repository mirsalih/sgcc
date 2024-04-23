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
    virtual std::ostream& print(std::ostream&) const = 0;
};

//TODO: when is need for other constants convert it to template
struct Constant: Exp {
    int value;
    explicit Constant(int val): value(val)
    {
    }

    std::ostream& print(std::ostream& os) const override {
        return os << value;
    }
};

struct Statement: Node {
    virtual std::ostream& print(std::ostream&) const = 0;
};

struct Return: Statement {
    std::unique_ptr<Exp> expression;
    explicit Return(std::unique_ptr<Exp> expr): expression(std::move(expr))
    {
    }

    std::ostream& print(std::ostream& os) const override {
        os << "return ";
        expression->print(os);
        return os << ';';
    }
};

struct Function: Node {
    std::string name;
    std::unique_ptr<Statement> body;
    Function(std::string n, std::unique_ptr<Statement> b): name(std::move(n)),
                                                           body(std::move(b))
    {
    }
    std::ostream& print(std::ostream& os) const {
        os << "int " << name << "() {\n  ";
        body->print(os);
        return os << "\n}\n";
    }
};

struct Program: Node {
    std::unique_ptr<Function> function;
    explicit Program(std::unique_ptr<Function> func): function(std::move(func))
    {
    }
    
    std::ostream& print(std::ostream& os) const {
        return function->print(os);
    }
};
}

#endif // __AST_NODES_H__