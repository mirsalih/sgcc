# Setup testing
enable_testing()

# Include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(
  test_main
  test_main.cpp
)

# Link test executable with GoogleTest
target_link_libraries(
  test_main
  lexerlib
  gtest_main
)

# Include Google Test in the testing
include(GoogleTest)
gtest_discover_tests(test_main)
