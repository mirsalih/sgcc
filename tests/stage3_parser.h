#ifndef __STAGE3_PARSER_H__
#define __STAGE3_PARSER_H__

#include <gtest/gtest.h>
#include <memory>

#include "test_environment.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

class ParserTestStage_3 : public ::testing::Test {
private:
    void testExpression(const sgcc::Exp& expr1, const sgcc::Exp& expr2)
    {
        using namespace sgcc;
        
        EXPECT_EQ(expr1.kind(), expr2.kind());
        if(expr1.kind() == ExprKind::BINARY_OP) {
            const auto& op1 = static_cast<const BinaryOp&>(expr1);
            const auto& op2 = static_cast<const BinaryOp&>(expr2);
            
            EXPECT_EQ(op1.oprtor, op2.oprtor);
            testExpression(*op1.leftExpr, *op2.leftExpr);
            testExpression(*op1.rightExpr, *op2.rightExpr);
        }
        else if(expr1.kind() == ExprKind::UNARY_OP) {
            const auto& op1 = static_cast<const UnaryOp&>(expr1);
            const auto& op2 = static_cast<const UnaryOp&>(expr2);
            
            EXPECT_EQ(op1.oprtor, op2.oprtor);
            testExpression(*op1.expression, *op2.expression);
        }
        else if(expr1.kind() == ExprKind::CONSTANT){
            // Constant expression tests
            auto const1 = static_cast<const Constant&>(expr1);
            auto const2 = static_cast<const Constant&>(expr2);

            EXPECT_EQ(const1.value, const2.value);
        }
        else {
            EXPECT_FALSE(true);
        }
    }

protected:
    void validTest(const std::string& sourceCode) {
        using namespace sgcc;

        auto pp = parser.parseProgram(lexer.tokenize(sourceCode));

        // program tests
        EXPECT_TRUE(pp);
        EXPECT_TRUE(testProg);

        // function test
        EXPECT_EQ(pp->function->name, testProg->function->name);

        // Return statement tests
        Return* progRet;
        Return* testProgRet;
        EXPECT_TRUE(progRet = dynamic_cast<Return*>(pp->function->body.get()));
        EXPECT_TRUE(testProgRet = dynamic_cast<Return*>(testProg->function->body.get()));

        // Expression tests
        Exp* progExp;
        Exp* testProgExp;
        EXPECT_TRUE(progExp = dynamic_cast<Exp*>(progRet->expression.get()));
        EXPECT_TRUE(testProgExp = dynamic_cast<Exp*>(testProgRet->expression.get()));

        testExpression(*progExp, *testProgExp);
    }
    
    void invalidTest(const std::string& sourceCode) {
        EXPECT_ANY_THROW(parser.parseProgram(lexer.tokenize(sourceCode)));
    }

    void invalidTestNullptr(const std::string& sourceCode) {
        auto pp = parser.parseProgram(lexer.tokenize(sourceCode));
        EXPECT_FALSE(pp);
    }

    void makeProgram(std::unique_ptr<sgcc::Exp> expr) {
        using namespace sgcc;
        using namespace std;
        testProg = make_unique<Program>(make_unique<Function>("main", make_unique<Return>(move(expr))));
    }

protected:
    sgcc::Parser parser;
    sgcc::Lexer lexer;
    std::unique_ptr<sgcc::Program> testProg;
};

TEST_F(ParserTestStage_3, add) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/add.c"};
    makeProgram(make_unique<BinaryOp>(Operator::ADDITION, make_unique<Constant>(1),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, associativity) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/associativity.c"};
    auto rigtExpr = make_unique<BinaryOp>(Operator::SUBTRACTION, make_unique<Constant>(1),
                                          make_unique<Constant>(2));
    makeProgram(make_unique<BinaryOp>(Operator::SUBTRACTION, move(rigtExpr),
                                      make_unique<Constant>(3)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, associativity2) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/associativity_2.c"};
    auto rigtExpr = make_unique<BinaryOp>(Operator::DIVISION, make_unique<Constant>(6),
                                          make_unique<Constant>(3));
    makeProgram(make_unique<BinaryOp>(Operator::DIVISION, move(rigtExpr),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, div) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/div.c"};
    makeProgram(make_unique<BinaryOp>(Operator::DIVISION, make_unique<Constant>(4),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, divNeg) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/div_neg.c"};
    auto rigtExpr = make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(12));
    makeProgram(make_unique<BinaryOp>(Operator::DIVISION, move(rigtExpr),
                                      make_unique<Constant>(5)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, mult) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/mult.c"};
    makeProgram(make_unique<BinaryOp>(Operator::MULTIPLICATION, make_unique<Constant>(2),
                                      make_unique<Constant>(3)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, parens) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/parens.c"};
    auto leftExpr = make_unique<BinaryOp>(Operator::ADDITION, make_unique<Constant>(3),
                                          make_unique<Constant>(4));
    makeProgram(make_unique<BinaryOp>(Operator::MULTIPLICATION, make_unique<Constant>(2),
                                      move(leftExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, precedence) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/precedence.c"};
    auto leftExpr = make_unique<BinaryOp>(Operator::MULTIPLICATION, make_unique<Constant>(3),
                                          make_unique<Constant>(4));
    makeProgram(make_unique<BinaryOp>(Operator::ADDITION, make_unique<Constant>(2),
                                      move(leftExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, sub) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/sub.c"};
    makeProgram(make_unique<BinaryOp>(Operator::SUBTRACTION, make_unique<Constant>(1),
                                          make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, subNeg) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_3/valid/sub_neg.c"};
    auto rigtExpr = make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(1));
    makeProgram(make_unique<BinaryOp>(Operator::SUBTRACTION, make_unique<Constant>(2),
                                      move(rigtExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, malformedParen) {
    SourceCode sourceCode{"stage_3/invalid/malformed_paren.c"};
    invalidTestNullptr(sourceCode.src);
}

TEST_F(ParserTestStage_3, missingFirstOperand) {
    SourceCode sourceCode{"stage_3/invalid/missing_first_op.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, missingSecondOperand) {
    SourceCode sourceCode{"stage_3/invalid/missing_second_op.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_3, noSemicolon) {
    SourceCode sourceCode{"stage_3/invalid/no_semicolon.c"};
    invalidTestNullptr(sourceCode.src);
}
#endif // __STAGE3_PARSER_H__