#ifndef __TEST_ENVIRONMENT_H__
#define __TEST_ENVIRONMENT_H__

#include <gtest/gtest.h>
#include "../inc/reader.h"

class TestEnvironment;
namespace test
{
    TestEnvironment* envr;
}

// evironment class
class TestEnvironment : public testing::Environment {
public:
    explicit TestEnvironment(const std::string &eFolder) {
        examplesFolder = eFolder;
    }

public:
    std::string examplesFolder;
};

// To read source code from file
struct SourceCode {
    SourceCode(const std::string& file): src(SourceReader{}
                                         (test::envr->examplesFolder + file))
    {

    }

    const std::string src;
};

TEST(environmentTest, isEnvironmentReady) {
    using namespace test;

    ASSERT_TRUE(envr);
    EXPECT_FALSE(envr->examplesFolder.empty());
}

#endif // __TEST_ENVIRONMENT_H__