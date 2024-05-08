#ifndef __STAGE1_PARSER_H__
#define __STAGE1_PARSER_H__

#include <gtest/gtest.h>
#include <memory>

#include "test_environment.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

class ParserTestStage_1 : public ::testing::Test {
protected:
    void validTest(const std::string& funcName, int constVal, const std::string& sourceCode) {
        auto pp = parser.parseProgram(lexer.tokenize(sourceCode));

        EXPECT_TRUE(pp);
        EXPECT_EQ(pp->function->name, funcName);

        sgcc::Return* returnStatement;
        EXPECT_TRUE(returnStatement = dynamic_cast<sgcc::Return*>(pp->function->body.get()));

        sgcc::Constant* constant;
        EXPECT_TRUE(constant = dynamic_cast<sgcc::Constant*>(returnStatement->expression.get()));
        EXPECT_EQ(constant->value, constVal);
    }
    
    void invalidTest(const std::string& sourceCode) {
        auto pp = parser.parseProgram(lexer.tokenize(sourceCode));
        EXPECT_FALSE(pp);
    }

    void invalidTestThrow(const std::string& sourceCode) {
        EXPECT_ANY_THROW(parser.parseProgram(lexer.tokenize(sourceCode)));
    }

protected:
    sgcc::Parser parser;
    sgcc::Lexer lexer;
};

TEST_F(ParserTestStage_1, multiDigit) {
    validTest("main", 100, SourceCode{"stage_1/valid/multi_digit.c"}.src);
}

TEST_F(ParserTestStage_1, newlines) {
    validTest("main", 0, SourceCode{"stage_1/valid/newlines.c"}.src);
}

TEST_F(ParserTestStage_1, noNewlines) {
    validTest("main", 0, SourceCode{"stage_1/valid/no_newlines.c"}.src);
}

TEST_F(ParserTestStage_1, return0) {
    validTest("main", 0, SourceCode{"stage_1/valid/return_0.c"}.src);
}

TEST_F(ParserTestStage_1, return2) {
    validTest("main", 2, SourceCode{"stage_1/valid/return_2.c"}.src);
}

TEST_F(ParserTestStage_1, spaces) {
    validTest("main", 0, SourceCode{"stage_1/valid/spaces.c"}.src);
}

// Invalid source code tests
TEST_F(ParserTestStage_1, missingParenthes) {
    invalidTest(SourceCode{"stage_1/invalid/missing_paren.c"}.src);
}

TEST_F(ParserTestStage_1, missingRetval) {
    invalidTestThrow(SourceCode{"stage_1/invalid/missing_retval.c"}.src);
}

TEST_F(ParserTestStage_1, noBrace) {
    invalidTest(SourceCode{"stage_1/invalid/no_brace.c"}.src);
}

TEST_F(ParserTestStage_1, noSemicolon) {
    invalidTest(SourceCode{"stage_1/invalid/no_semicolon.c"}.src);
}

TEST_F(ParserTestStage_1, noSpace) {
    invalidTest(SourceCode{"stage_1/invalid/no_space.c"}.src);
}

TEST_F(ParserTestStage_1, wrongCase) {
    invalidTest(SourceCode{"stage_1/invalid/wrong_case.c"}.src);
}

#endif // __STAGE1_PARSER_H__