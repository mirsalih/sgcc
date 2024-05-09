#ifndef __STAGE4_LEXER_H__
#define __STAGE4_LEXER_H__

#include <gtest/gtest.h>
#include <algorithm>

#include "test_environment.h"
#include "../lexer/lexer.h"

class LexerTestStage_4 : public ::testing::Test {
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

TEST_F(LexerTestStage_4, andFalse) {
    SourceCode sourceCode{"stage_4/valid/and_false.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "&&"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, andTrue) {
    SourceCode sourceCode{"stage_4/valid/and_true.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "&&"}, {tt::OPERATOR, "-"},
                    {tt::NUMBER, "1"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, eqFalse) {
    SourceCode sourceCode{"stage_4/valid/eq_false.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "=="}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, eqTrue) {
    SourceCode sourceCode{"stage_4/valid/eq_true.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "=="}, {tt::NUMBER, "1"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, geFalse) {
    SourceCode sourceCode{"stage_4/valid/ge_false.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, ">="}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, geTrue) {
    SourceCode sourceCode{"stage_4/valid/ge_true.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, ">="}, {tt::NUMBER, "1"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, gtFalse) {
    SourceCode sourceCode{"stage_4/valid/gt_false.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, ">"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, gtTrue) {
    SourceCode sourceCode{"stage_4/valid/gt_true.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, ">"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, leFalse) {
    SourceCode sourceCode{"stage_4/valid/le_false.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "<="}, {tt::OPERATOR, "-"}, {tt::NUMBER, "1"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, leTrue) {
    SourceCode sourceCode{"stage_4/valid/le_true.c"};
    makeTestTokens({{tt::NUMBER, "0"}, {tt::OPERATOR, "<="}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, ltFalse) {
    SourceCode sourceCode{"stage_4/valid/lt_false.c"};
    makeTestTokens({{tt::NUMBER, "2"}, {tt::OPERATOR, "<"}, {tt::NUMBER, "1"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, ltTrue) {
    SourceCode sourceCode{"stage_4/valid/lt_true.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "<"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, neFalse) {
    SourceCode sourceCode{"stage_4/valid/ne_false.c"};
    makeTestTokens({{tt::NUMBER, "0"}, {tt::OPERATOR, "!="}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, neTrue) {
    SourceCode sourceCode{"stage_4/valid/ne_true.c"};
    makeTestTokens({{tt::OPERATOR, "-"}, {tt::NUMBER, "1"}, {tt::OPERATOR, "!="},
                   {tt::OPERATOR, "-"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, orFalse) {
    SourceCode sourceCode{"stage_4/valid/or_false.c"};
    makeTestTokens({{tt::NUMBER, "0"}, {tt::OPERATOR, "||"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, orTrue) {
    SourceCode sourceCode{"stage_4/valid/or_true.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "||"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, precedence) {
    SourceCode sourceCode{"stage_4/valid/precedence.c"};
    makeTestTokens({{tt::NUMBER, "1"}, {tt::OPERATOR, "||"}, {tt::NUMBER, "0"},
                    {tt::OPERATOR, "&&"}, {tt::NUMBER, "2"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, precedence2) {
    SourceCode sourceCode{"stage_4/valid/precedence_2.c"};
    makeTestTokens({{tt::OPEN_PAREN, "("}, {tt::NUMBER, "1"}, {tt::OPERATOR, "||"},
                    {tt::NUMBER, "0"}, {tt::CLOSE_PAREN, ")"}, {tt::OPERATOR, "&&"},
                    {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, precedence3) {
    SourceCode sourceCode{"stage_4/valid/precedence_3.c"};
    makeTestTokens({{tt::NUMBER, "2"}, {tt::OPERATOR, "=="}, {tt::NUMBER, "2"},
                    {tt::OPERATOR, ">"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

TEST_F(LexerTestStage_4, precedence4) {
    SourceCode sourceCode{"stage_4/valid/precedence_4.c"};
    makeTestTokens({{tt::NUMBER, "2"}, {tt::OPERATOR, "=="}, {tt::NUMBER, "2"},
                    {tt::OPERATOR, "||"}, {tt::NUMBER, "0"}});
    validTest(sourceCode.src);
}

#endif // __STAGE4_LEXER_H__