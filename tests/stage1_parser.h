#ifndef __STAGE1_PARSER_H__
#define __STAGE1_PARSER_H__

#include <gtest/gtest.h>
#include <memory>

#include "test_environment.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

class ParserTest : public ::testing::Test {
protected:
    void validTest(const std::string& funcName, int constVal, const std::string& sourceCode) {
        auto pp = parser.parseProgram(lexer.tokenize(sourceCode));

        EXPECT_TRUE(pp);
        EXPECT_EQ(pp.get()->function.get()->name, funcName);

        sgcc::Return* returnStatement;
        EXPECT_NO_THROW(returnStatement = dynamic_cast<sgcc::Return*>(pp->function->body.get()));

        sgcc::Constant* constant;
        EXPECT_NO_THROW(constant = dynamic_cast<sgcc::Constant*>(returnStatement->expression.get()));
        EXPECT_EQ(constant->value, constVal);
    }
    
    void invalidTest(const std::string& sourceCode) {
        auto pp = parser.parseProgram(lexer.tokenize(sourceCode));
        EXPECT_FALSE(pp);
    }

protected:
    sgcc::Parser parser;
    sgcc::Lexer lexer;
};

TEST_F(ParserTest, multiDigit) {
    validTest("main", 100, SourceCode{"stage_1/valid/multi_digit.c"}.src);
}

TEST_F(ParserTest, newlines) {
    validTest("main", 0, SourceCode{"stage_1/valid/newlines.c"}.src);
}

TEST_F(ParserTest, noNewlines) {
    validTest("main", 0, SourceCode{"stage_1/valid/no_newlines.c"}.src);
}

TEST_F(ParserTest, return0) {
    validTest("main", 0, SourceCode{"stage_1/valid/return_0.c"}.src);
}

TEST_F(ParserTest, return2) {
    validTest("main", 2, SourceCode{"stage_1/valid/return_2.c"}.src);
}

TEST_F(ParserTest, spaces) {
    validTest("main", 0, SourceCode{"stage_1/valid/spaces.c"}.src);
}

// Invalid source code tests
TEST_F(ParserTest, missingParenthes) {
    invalidTest(SourceCode{"stage_1/invalid/missing_paren.c"}.src);
}

TEST_F(ParserTest, missingRetval) {
    invalidTest(SourceCode{"stage_1/invalid/missing_retval.c"}.src);
}

TEST_F(ParserTest, noBrace) {
    invalidTest(SourceCode{"stage_1/invalid/no_brace.c"}.src);
}

TEST_F(ParserTest, noSemicolon) {
    invalidTest(SourceCode{"stage_1/invalid/no_semicolon.c"}.src);
}

TEST_F(ParserTest, noSpace) {
    invalidTest(SourceCode{"stage_1/invalid/no_space.c"}.src);
}

TEST_F(ParserTest, wrongCase) {
    invalidTest(SourceCode{"stage_1/invalid/wrong_case.c"}.src);
}

#endif // __STAGE1_PARSER_H__