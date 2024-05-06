#ifndef __STAGE2_PARSER_H__
#define __STAGE2_PARSER_H__

#include <gtest/gtest.h>
#include <memory>

#include "test_environment.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

class ParserTestStage_2 : public ::testing::Test {
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

        // unary operator tests
        while(progExp->kind() == ExprKind::UnaryOp && testProgExp->kind() == ExprKind::UnaryOp) {
            UnaryOp* op = static_cast<UnaryOp*>(progExp);
            UnaryOp* testOp = static_cast<UnaryOp*>(testProgExp);
            
            EXPECT_EQ(op->oprtor, testOp->oprtor);
            EXPECT_EQ(op->expression->kind(), testOp->expression->kind());
            EXPECT_TRUE(progExp = op->expression.get());
            EXPECT_TRUE(testProgExp = testOp->expression.get());
        }

        // Constant expression tests
        Constant* progConst;
        Constant* testProgConst;
        EXPECT_TRUE(progConst = dynamic_cast<Constant*>(progExp));
        EXPECT_TRUE(testProgConst = dynamic_cast<Constant*>(testProgExp));
        EXPECT_EQ(progConst->value, testProgConst->value);
    }
    
    void invalidTest(const std::string& sourceCode) {
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

TEST_F(ParserTestStage_2, bitwiseZero) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/bitwise_zero.c"};
    makeProgram(make_unique<UnaryOp>(Operator::BWISE_COMPLEMENT, make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, bitwise) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/bitwise.c"};
    makeProgram(make_unique<UnaryOp>(Operator::BWISE_COMPLEMENT, make_unique<Constant>(12)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, neg) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/neg.c"};
    makeProgram(make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(5)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, nestedOps) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/nested_ops.c"};
    makeProgram(make_unique<UnaryOp>(Operator::LOGICAL_NEGATION,
                make_unique<UnaryOp>(Operator::NEGATION, make_unique<Constant>(3))));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, nestedOps2) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/nested_ops_2.c"};
    makeProgram(make_unique<UnaryOp>(Operator::NEGATION,
                make_unique<UnaryOp>(Operator::BWISE_COMPLEMENT, make_unique<Constant>(0))));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, notFive) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/not_five.c"};
    makeProgram(make_unique<UnaryOp>(Operator::LOGICAL_NEGATION, make_unique<Constant>(5)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, notZero) {
    using namespace std;
    using namespace sgcc;

    SourceCode sourceCode{"stage_2/valid/not_zero.c"};
    makeProgram(make_unique<UnaryOp>(Operator::LOGICAL_NEGATION, make_unique<Constant>(0)));
    validTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, missingConst) {
    SourceCode sourceCode{"stage_2/invalid/missing_const.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, missingSemicolon) {
    SourceCode sourceCode{"stage_2/invalid/missing_semicolon.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, nestedMissingConst) {
    SourceCode sourceCode{"stage_2/invalid/nested_missing_const.c"};
    invalidTest(sourceCode.src);
}

TEST_F(ParserTestStage_2, wrongOrder) {
    SourceCode sourceCode{"stage_2/invalid/wrong_order.c"};
    invalidTest(sourceCode.src);
}

#endif // __STAGE2_PARSER_H__