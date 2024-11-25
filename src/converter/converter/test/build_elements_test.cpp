#include <gtest/gtest.h>
#include "build/build_elements.hpp"

TEST(BuildElementsTest, VariableToString) {
    Variable var{"flags", "-O2", true};
    EXPECT_EQ(var.toString(), "flags = -O2");
}

TEST(BuildElementsTest, RuleToString) {
    Rule rule;
    rule.name = "compile";
    rule.command = "g++ $in -o $out";
    EXPECT_FALSE(rule.toString().empty());
}

TEST(BuildElementsTest, RuleExpandCommand) {
    Rule rule;
    rule.name = "compile";
    rule.command = "g++ $flags $in -o $out";
    
    std::map<std::string, std::string> globalVars{{"flags", "-O2"}};
    
    auto expanded = rule.expandCommand(globalVars, "input.cpp", "output.o");
    EXPECT_EQ(expanded, "g++ -O2 input.cpp -o output.o");
}

TEST(BuildElementsTest, BuildTargetToString) {
    BuildTarget target;
    target.output = "output.o";
    target.ruleName = "compile";
    target.inputs = {"input.cpp"};
    
    std::string expected = "build output.o: compile input.cpp\n";
    EXPECT_EQ(target.toString(), expected);
}

TEST(BuildElementsTest, BuildFileToString) {
    BuildFile buildFile;
    buildFile.globalVariables["flags"] = "-O2";
    
    Rule rule;
    rule.name = "compile";
    rule.command = "g++ $flags $in -o $out";
    buildFile.rules["compile"] = rule;
    
    BuildTarget target;
    target.output = "output.o";
    target.ruleName = "compile";
    target.inputs = {"input.cpp"};
    buildFile.buildTargets.push_back(target);
    
    std::string result = buildFile.toString();
    EXPECT_TRUE(result.find("flags = -O2") != std::string::npos);
    EXPECT_TRUE(result.find("rule compile") != std::string::npos);
    EXPECT_TRUE(result.find("build output.o: compile input.cpp") != std::string::npos);
}
