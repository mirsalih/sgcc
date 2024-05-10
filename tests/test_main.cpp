#include "test_environment.h"
#define TEST_LEXER 1
#define TEST_PARSER 1

#if TEST_LEXER
#include "stage1_lexer.h"
#include "stage2_lexer.h"
#include "stage3_lexer.h"
#include "stage4_lexer.h"
#endif

#if TEST_PARSER
#include "stage1_parser.h"
#include "stage2_parser.h"
#include "stage3_parser.h"
#include "stage4_parser.h"
#endif


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    std::string folder(argc == 2 ? argv[1] : "");
    test::envr = dynamic_cast<TestEnvironment*>(testing::AddGlobalTestEnvironment(
                                                    new TestEnvironment(folder)));

    return RUN_ALL_TESTS();
}
