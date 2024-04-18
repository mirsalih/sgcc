#include <gtest/gtest.h>
#include "../lexer/lexer.h"

TEST(lexerTest, TokenSizeCheck) {

    std::string input = "int main( {\n  return 0; }";
    sgcc::Lexer lexer(input);
    std::vector<sgcc::Token> tokens = lexer.tokenize();

    ASSERT_EQ(9, tokens.size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
