#pragma once
#include <string>
#include <vector>
#include <map>

struct BuildFile; // Forward declaration for BuildTarget

struct Variable {
    std::string name;
    std::string value;
    bool isGlobal;

    // Returns string representation of the variable assignment
    std::string toString() const;
};

struct Rule {
    std::string name;
    std::string command;
    std::map<std::string, std::string> variables;

    // Expands variables in the command string using global and rule-specific variables
    std::string expandCommand(const std::map<std::string, std::string>& globalVars,
                            const std::string& in,
                            const std::string& out) const;

    // Returns string representation of the rule
    std::string toString() const;
};

struct BuildTarget {
    std::string output;
    std::string ruleName;
    std::vector<std::string> inputs;
    std::map<std::string, std::string> variables;

    // Returns the expanded command string for this build target
    std::string getExpandedCommand(const BuildFile& buildFile) const;

    // Returns string representation of the build target
    std::string toString() const;
};

struct BuildFile {
    std::map<std::string, std::string> globalVariables;
    std::map<std::string, Rule> rules;
    std::vector<BuildTarget> buildTargets;
    std::vector<std::string> defaults;

    // Returns string representation of the entire build file
    std::string toString() const;

    // Generates code representation of the build file
    std::string toCodeString() const;
};