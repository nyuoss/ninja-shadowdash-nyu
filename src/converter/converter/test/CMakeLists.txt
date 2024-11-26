# Download and configure GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Prevent overriding parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add test executable
add_executable(converter_tests
    lexer_test.cpp
    token_test.cpp
    parser_test.cpp
    build_elements_test.cpp
)

# Link test executable with Google Test and project library
target_link_libraries(converter_tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    converter_lib
)

# Enable test discovery
include(GoogleTest)
gtest_discover_tests(converter_tests)
