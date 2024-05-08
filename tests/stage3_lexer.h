#ifndef __STAGE3_LEXER_H__
#define __STAGE3_LEXER_H__

#include <gtest/gtest.h>
#include <algorithm>

#include "test_environment.h"
#include "../lexer/lexer.h"

class LexerTestStage_3 : public ::testing::Test {
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

TEST_F(LexerTestStage_3, add) {
    SourceCode sourceCode{"stage_3/valid/add.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "+"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, associativity) {
    SourceCode sourceCode{"stage_3/valid/associativity.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "-"}, {tt::NUMBER, "2"},
                     {tt::OPERATOR, "-"}, {tt::NUMBER, "3"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, associativity2) {
    SourceCode sourceCode{"stage_3/valid/associativity_2.c"};
    makeTestTokens({{tt::NUMBER, "6"}, {tt::OPERATOR, "/"}, {tt::NUMBER, "3"},
                     {tt::OPERATOR, "/"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, div) {
    SourceCode sourceCode{"stage_3/valid/div.c"};
    makeTestTokens({{tt::NUMBER, "4"}, {tt::OPERATOR, "/"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, divNeg) {
    SourceCode sourceCode{"stage_3/valid/div_neg.c"};
    makeTestTokens({{tt::OPEN_PAREN, "("},{tt::OPERATOR, "-"}, {tt::NUMBER, "12"},
                    {tt::CLOSE_PAREN, ")"}, {tt::OPERATOR, "/"}, {tt::NUMBER, "5"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, mult) {
    SourceCode sourceCode{"stage_3/valid/mult.c"};
    makeTestTokens({{tt::NUMBER, "2"},{tt::OPERATOR, "*"}, {tt::NUMBER, "3"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, parens) {
    SourceCode sourceCode{"stage_3/valid/parens.c"};
    makeTestTokens({{tt::NUMBER, "2"}, {tt::OPERATOR, "*"}, {tt::OPEN_PAREN, "("},
                    {tt::NUMBER, "3"}, {tt::OPERATOR, "+"}, {tt::NUMBER, "4"},
                    {tt::CLOSE_PAREN, ")"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, precedence) {
    SourceCode sourceCode{"stage_3/valid/precedence.c"};
    makeTestTokens({{tt::NUMBER, "2"}, {tt::OPERATOR, "+"}, {tt::NUMBER, "3"},
                    {tt::OPERATOR, "*"}, {tt::NUMBER, "4"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, sub) {
    SourceCode sourceCode{"stage_3/valid/sub.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "-"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_3, subNeg) {
    SourceCode sourceCode{"stage_3/valid/sub_neg.c"};
    makeTestTokens({{tt::NUMBER, "2"}, {tt::OPERATOR, "-"}, {tt::OPERATOR, "-"}, {tt::NUMBER, "1"}});
    validTest(sourceCode.src);
}
#endif // __STAGE3_LEXER_H__