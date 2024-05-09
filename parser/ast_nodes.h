#ifndef __AST_NODES_H__
#define __AST_NODES_H__

#include <memory>
#include <string>
#include <iostream>

namespace sgcc
{
enum class Operator {
    NEGATION, BWISE_COMPLEMENT, LOGICAL_NEGATION, ADDITION, SUBTRACTION,
    MULTIPLICATION, DIVISION, AND, OR, EQUAL, NOT_EQUAL, LESS_THAN, LESS_THAN_OR_EQUAL,
    GREATER_THAN, GREATER_THAN_OR_EQUAL
};

enum class ExprKind {
    CONSTANT, UNARY_OP, BINARY_OP
};

struct Node {
    virtual ~Node() = default;
};

struct Exp: Node {
    [[nodiscard]] virtual ExprKind kind() const = 0;
};

//TODO: when is need for other constants convert it to template
struct Constant: Exp {
    int value;
    explicit Constant(int val): value(val)
    {
    }

    ExprKind kind() const override {
        return ExprKind::CONSTANT;
    }
};

struct UnaryOp: Exp {
    Operator oprtor;
    std::unique_ptr<Exp> expression;
    explicit UnaryOp(Operator op, std::unique_ptr<Exp> expr): oprtor(op),
                     expression(std::move(expr))
    {
    }

    ExprKind kind() const override {
        return ExprKind::UNARY_OP;
    }
};

struct BinaryOp: Exp {
    Operator oprtor;
    std::unique_ptr<Exp> leftExpr;
    std::unique_ptr<Exp> rightExpr;
    
    explicit BinaryOp(Operator op, std::unique_ptr<Exp> leftE,std::unique_ptr<Exp> rightE):
            oprtor(op), leftExpr(std::move(leftE)), rightExpr(std::move(rightE))
    {
    }

    ExprKind kind() const override {
        return ExprKind::BINARY_OP;
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