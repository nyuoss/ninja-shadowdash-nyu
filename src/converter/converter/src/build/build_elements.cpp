// build_elements.cpp
#include "include/build/build_elements.hpp"
#include <sstream>

std::string Variable::toString() const {
    return name + " = " + value;
}

std::string Rule::expandCommand(const std::map<std::string, std::string>& globalVars,
                              const std::string& in,
                              const std::string& out) const {
    std::string result = command;
    
    // First replace rule-specific variables
    for (const auto& [key, value] : variables) {
        std::string placeholder = "$" + key;
        size_t pos = result.find(placeholder);
        if (pos != std::string::npos) {
            result.replace(pos, placeholder.length(), value);
        }
    }
    
    // Then replace global variables
    for (const auto& [key, value] : globalVars) {
        std::string placeholder = "$" + key;
        size_t pos = result.find(placeholder);
        if (pos != std::string::npos && variables.find(key) == variables.end()) {
            result.replace(pos, placeholder.length(), value);
        }
    }
    
    // Replace $in and $out
    size_t inPos = result.find("$in");
    if (inPos != std::string::npos) {
        result.replace(inPos, 3, in);
    }
    
    size_t outPos = result.find("$out");
    if (outPos != std::string::npos) {
        result.replace(outPos, 4, out);
    }
    
    return result;
}

std::string BuildTarget::getExpandedCommand(const BuildFile& buildFile) const {
    const Rule& rule = buildFile.rules.at(ruleName);
    std::string inputList;
    for (const auto& input : inputs) {
        if (!inputList.empty()) inputList += " ";
        inputList += input;
    }
    return rule.expandCommand(buildFile.globalVariables, inputList, output);
}