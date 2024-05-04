#ifndef __STAGE1_LEXER_H__
#define __STAGE1_LEXER_H__

#include <gtest/gtest.h>

#include "test_environment.h"
#include "../lexer/lexer.h"

class LexerTest : public ::testing::Test {
protected:
    void validTest(const std::string& funcName, const std::string& constVal,
                   const std::string& sourceCode) {
        auto tokens =  lexer.tokenize(sourceCode);

        EXPECT_EQ(tokens.size(), 10);
        EXPECT_EQ(tokens[0].type, sgcc::TokenType::INT_KEYWORD);
        EXPECT_EQ(tokens[0].text, "int");
        EXPECT_EQ(tokens[1].type, sgcc::TokenType::IDENTIFIER);
        EXPECT_EQ(tokens[1].text, funcName);
        EXPECT_EQ(tokens[2].type, sgcc::TokenType::OPEN_PAREN);
        EXPECT_EQ(tokens[2].text, "(");
        EXPECT_EQ(tokens[3].type, sgcc::TokenType::CLOSE_PAREN);
        EXPECT_EQ(tokens[3].text, ")");
        EXPECT_EQ(tokens[4].type, sgcc::TokenType::OPEN_BRACE);
        EXPECT_EQ(tokens[4].text, "{");
        EXPECT_EQ(tokens[5].type, sgcc::TokenType::RETURN_KEYWORD);
        EXPECT_EQ(tokens[5].text, "return");
        EXPECT_EQ(tokens[6].type, sgcc::TokenType::NUMBER);
        EXPECT_EQ(tokens[6].text, constVal);
        EXPECT_EQ(tokens[7].type, sgcc::TokenType::SEMICOLON);
        EXPECT_EQ(tokens[7].text, ";");
        EXPECT_EQ(tokens[8].type, sgcc::TokenType::CLOSE_BRACE);
        EXPECT_EQ(tokens[8].text, "}");
        EXPECT_EQ(tokens[9].type, sgcc::TokenType::END);
        EXPECT_EQ(tokens[9].text, "<end>");
    }

protected:
    sgcc::Lexer lexer;
};

TEST_F(LexerTest, doubleInvokeTokenize) {
    std::string sourceCode = "int main () \n{\n\treturn 1;\n}\n";
    auto tokens1 = lexer.tokenize(sourceCode);
    auto tokens2 = lexer.tokenize(sourceCode);

    EXPECT_EQ(tokens1.size(), tokens2.size());
    EXPECT_EQ(tokens1[0].type, tokens2[0].type);
    EXPECT_EQ(tokens1[0].text, tokens2[0].text);
    EXPECT_EQ(tokens1[1].type, tokens2[1].type);
    EXPECT_EQ(tokens1[1].text, tokens2[1].text);
}

TEST_F(LexerTest, multiDigit) {
    validTest("main", "100", SourceCode{"stage_1/valid/multi_digit.c"}.src);
}

TEST_F(LexerTest, newlines) {
    validTest("main", "0", SourceCode{"stage_1/valid/newlines.c"}.src);
}

TEST_F(LexerTest, NoNewlines) {
    validTest("main", "0", SourceCode{"stage_1/valid/no_newlines.c"}.src);
}

TEST_F(LexerTest, return0) {
    validTest("main", "0", SourceCode{"stage_1/valid/return_0.c"}.src);
}

TEST_F(LexerTest, return2) {
    validTest("main", "2", SourceCode{"stage_1/valid/return_2.c"}.src);
}

TEST_F(LexerTest, spaces) {
    validTest("main", "0", SourceCode{"stage_1/valid/spaces.c"}.src);
}

#endif //__STAGE1_LEXER_H__