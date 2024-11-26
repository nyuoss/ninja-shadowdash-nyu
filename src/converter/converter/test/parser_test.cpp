#include <gtest/gtest.h>
#include "parser/parser.hpp"
#include "lexer/lexer.hpp"

class ParserTest : public ::testing::Test {
protected:
    BuildFile parse(const std::string& source) {
        Lexer lexer(source);
        Parser parser(lexer.scanTokens());
        return parser.parse();
    }
};

TEST_F(ParserTest, SimpleRule) {
    auto buildFile = parse(R"(
rule compile
    command = g++ -c $in -o $out
)");
    ASSERT_EQ(buildFile.rules.size(), 1);
    EXPECT_NE(buildFile.rules.find("compile"), buildFile.rules.end());
}

TEST_F(ParserTest, BuildStatement) {
    auto buildFile = parse(R"(
rule compile
    command = g++ -c $in -o $out
build output.o: compile input.cpp
)");
    ASSERT_EQ(buildFile.buildTargets.size(), 1);
    EXPECT_EQ(buildFile.buildTargets[0].output, "output.o");
    EXPECT_EQ(buildFile.buildTargets[0].ruleName, "compile");
    ASSERT_EQ(buildFile.buildTargets[0].inputs.size(), 1);
    EXPECT_EQ(buildFile.buildTargets[0].inputs[0], "input.cpp");
}

TEST_F(ParserTest, Variables) {
    auto buildFile = parse(R"(
flags = -O2
rule compile
    command = g++ $flags $in -o $out
)");
    ASSERT_EQ(buildFile.globalVariables.size(), 1);
    EXPECT_EQ(buildFile.globalVariables["flags"], "-O2");
}
