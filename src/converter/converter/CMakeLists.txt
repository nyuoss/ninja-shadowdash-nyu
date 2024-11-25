cmake_minimum_required(VERSION 3.14)
project(ninja_converter VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add coverage build type
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage")

# Create main library target
add_library(converter_lib
    src/lexer/lexer.cpp
    src/lexer/token.cpp
    src/parser/parser.cpp
    src/build/build_elements.cpp
)

target_include_directories(converter_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add test directory
enable_testing()
add_subdirectory(test)

# Create main executable
add_executable(converter src/main.cpp)
target_link_libraries(converter PRIVATE converter_lib)
