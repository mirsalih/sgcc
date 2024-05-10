#ifndef __STAGE4_PARSER_H__
#define __STAGE4_PARSER_H__

#include <gtest/gtest.h>
#include <memory>

#include "test_environment.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

class ParserTestStage_4 : public ::testing::Test {
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

TEST_F(ParserTestStage_4, andFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/and_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::AND, make_unique<Constant>(1),
                                      make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, andTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/and_true.c"};
    auto rightExpr = make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(1));
    makeProgram(make_unique<BinaryOp>(Operator::AND, make_unique<Constant>(1), move(rightExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, eqFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/eq_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::EQUAL, make_unique<Constant>(1),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, eqTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/eq_true.c"};
    makeProgram(make_unique<BinaryOp>(Operator::EQUAL, make_unique<Constant>(1),
                                      make_unique<Constant>(1)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, geFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/ge_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::GREATER_THAN_OR_EQUAL, make_unique<Constant>(1),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, geTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/ge_true.c"};
    makeProgram(make_unique<BinaryOp>(Operator::GREATER_THAN_OR_EQUAL, make_unique<Constant>(1),
                                      make_unique<Constant>(1)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, gtFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/gt_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::GREATER_THAN, make_unique<Constant>(1),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, gtTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/gt_true.c"};
    makeProgram(make_unique<BinaryOp>(Operator::GREATER_THAN, make_unique<Constant>(1),
                                      make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, leFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/le_false.c"};
    auto rightExpr = make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(1));
    makeProgram(make_unique<BinaryOp>(Operator::LESS_THAN_OR_EQUAL, make_unique<Constant>(1),
                                      move(rightExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, leTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/le_true.c"};
    makeProgram(make_unique<BinaryOp>(Operator::LESS_THAN_OR_EQUAL, make_unique<Constant>(0),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, ltFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/lt_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::LESS_THAN, make_unique<Constant>(2),
                                      make_unique<Constant>(1)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, ltTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/lt_true.c"};
    makeProgram(make_unique<BinaryOp>(Operator::LESS_THAN, make_unique<Constant>(1),
                                      make_unique<Constant>(2)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, neFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/ne_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::NOT_EQUAL, make_unique<Constant>(0),
                                      make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, neTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/ne_true.c"};

    auto leftExpr = make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(1));
    auto rightExpr = make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(2));
    makeProgram(make_unique<BinaryOp>(Operator::NOT_EQUAL, move(leftExpr), move(rightExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, orFalse) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/or_false.c"};
    makeProgram(make_unique<BinaryOp>(Operator::OR, make_unique<Constant>(0),
                                      make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, orTrue) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/or_true.c"};
    makeProgram(make_unique<BinaryOp>(Operator::OR, make_unique<Constant>(1),
                                      make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, precedence) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/precedence.c"};
    auto rigthExpr = make_unique<BinaryOp>(Operator::AND, make_unique<Constant>(0),
                                           make_unique<Constant>(2));
    makeProgram(make_unique<BinaryOp>(Operator::OR, make_unique<Constant>(1), move(rigthExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, precedence2) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/precedence_2.c"};
    auto leftExpr = make_unique<BinaryOp>(Operator::OR, make_unique<Constant>(1),
                                          make_unique<Constant>(0));
    makeProgram(make_unique<BinaryOp>(Operator::AND, move(leftExpr), make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, precedence3) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/precedence_3.c"};
    auto rightExpr = make_unique<BinaryOp>(Operator::GREATER_THAN, make_unique<Constant>(2),
                                           make_unique<Constant>(0));
    makeProgram(make_unique<BinaryOp>(Operator::EQUAL, make_unique<Constant>(2),
                                      move(rightExpr)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, precedence4) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_4/valid/precedence_4.c"};
    auto leftExpr = make_unique<BinaryOp>(Operator::EQUAL, make_unique<Constant>(2),
                                          make_unique<Constant>(2));
    makeProgram(make_unique<BinaryOp>(Operator::OR, move(leftExpr), make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, missingfirstOp) {
    SourceCode sourceCode{"stage_4/invalid/missing_first_op.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, missingMidOp) {
    SourceCode sourceCode{"stage_4/invalid/missing_mid_op.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, missingSecondOp) {
    SourceCode sourceCode{"stage_4/invalid/missing_second_op.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_4, missingSemicolon) {
    SourceCode sourceCode{"stage_4/invalid/missing_semicolon.c"};
    invalidTestNullptr(sourceCode.src);
}

#endif // __STAGE4_PARSER_H__