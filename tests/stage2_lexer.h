#ifndef __STAGE2_LEXER_H__
#define __STAGE2_LEXER_H__

#include <gtest/gtest.h>
#include <algorithm>

#include "test_environment.h"
#include "../lexer/lexer.h"

class LexerTestStage_2 : public ::testing::Test {
protected:
    void validTest(const std::string& sourceCode) {
        auto tokens =  lexer.tokenize(sourceCode);

        EXPECT_EQ(tokens.size(), testTokens.size());
        for(size_t i = 0; i < tokens.size(); i++) {
            const auto& t1 = tokens[i];
            const auto& t2 = testTokens[i];

            EXPECT_EQ(t1.type, t2.type) << "index " << i;
            EXPECT_EQ(t1.text, t2.text) << "index " << i << " type1 " << t1.type << " type2 " << t2.type;
        }
    }

    void makeTestTokens(std::initializer_list<sgcc::Token> retExpr) {
        testTokens = {{tt::INT_KEYWORD, "int"}, {tt::IDENTIFIER, "main"},
                        {tt::OPEN_PAREN, "("}, {tt::CLOSE_PAREN, ")"}, {tt::OPEN_BRACE, "{"},
                        {tt::RETURN_KEYWORD, "return"}};
        testTokens.insert(testTokens.end(), retExpr);
        testTokens.insert(testTokens.end(), { {tt::SEMICOLON, ";"}, {tt::CLOSE_BRACE, "}"},
                        {tt::END, "<end>"}} );
    }

protected:
    using tt = sgcc::TokenType;
    sgcc::Lexer lexer;
    std::vector<sgcc::Token> testTokens;
};

TEST_F(LexerTestStage_2, bitwiseZero) {
    SourceCode sourceCode{"stage_2/valid/bitwise_zero.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "~"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_2, bitwise) {
    SourceCode sourceCode{"stage_2/valid/bitwise.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "~"}, {tt::NUMBER, "12"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_2, neg) {
    SourceCode sourceCode{"stage_2/valid/neg.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "-"}, {tt::NUMBER, "5"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_2, nestedOps) {
    SourceCode sourceCode{"stage_2/valid/nested_ops.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "!"}, {tt::UNARY_OPRATION, "-"}, {tt::NUMBER, "3"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_2, nestedOps2) {
    SourceCode sourceCode{"stage_2/valid/nested_ops_2.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "-"}, {tt::UNARY_OPRATION, "~"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_2, notFive) {
    SourceCode sourceCode{"stage_2/valid/not_five.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "!"}, {tt::NUMBER, "5"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_2, notZero) {
    SourceCode sourceCode{"stage_2/valid/not_zero.c"};
    makeTestTokens({{tt::UNARY_OPRATION, "!"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

#endif // __STAGE2_LEXER_H__