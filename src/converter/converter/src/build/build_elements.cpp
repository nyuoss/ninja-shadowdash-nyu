// build_elements.cpp
#include "build/build_elements.hpp"
#include <sstream>
#include <regex>

std::string Variable::toString() const {
    return name + " = " + value;
}

std::string Rule::expandCommand(const std::map<std::string, std::string>& globalVars,
                              const std::string& in,
                              const std::string& out) const {
    std::string result = command;

    // Replace variables in the command string
    std::regex varPattern(R"(\$[a-zA-Z_][a-zA-Z0-9_]*)");
    std::smatch match;
    std::string::const_iterator searchStart(result.cbegin());

    while (std::regex_search(searchStart, result.cend(), match, varPattern)) {
        std::string varName = match.str().substr(1); // Remove the '$'
        std::string replacement;

        // Check rule-specific variables first
        auto ruleVarIt = variables.find(varName);
        if (ruleVarIt != variables.end()) {
            replacement = ruleVarIt->second;
        }
        // Then check global variables
        else {
            auto globalVarIt = globalVars.find(varName);
            if (globalVarIt != globalVars.end()) {
                replacement = globalVarIt->second;
            }
            // Special variables
            else if (varName == "in") {
                replacement = in;
            } else if (varName == "out") {
                replacement = out;
            } else {
                // Unknown variable, replace with empty string or throw an error
                replacement = "";
            }
        }

        result.replace(match.position(), match.length(), replacement);
        searchStart = result.begin() + match.position() + replacement.length();
    }

    return result;
}

std::string Rule::toString() const {
    std::ostringstream oss;
    oss << "rule " << name << "\n";
    for (const auto& var : variables) {
        oss << "  " << var.first << " = " << var.second << "\n";
    }
    oss << "  command = " << command << "\n";
    return oss.str();
}

std::string BuildTarget::toString() const {
    std::ostringstream oss;
    oss << "build " << output << ": " << ruleName;
    for (const auto& input : inputs) {
        oss << " " << input;
    }
    oss << "\n";
    for (const auto& var : variables) {
        oss << "  " << var.first << " = " << var.second << "\n";
    }
    return oss.str();
}

std::string BuildFile::toString() const {
    std::ostringstream oss;

    // Global variables
    for (const auto& var : globalVariables) {
        oss << var.first << " = " << var.second << "\n";
    }
    oss << "\n";

    // Rules
    for (const auto& rulePair : rules) {
        oss << rulePair.second.toString() << "\n";
    }
    oss << "\n";

    // Build targets
    for (const auto& target : buildTargets) {
        oss << target.toString() << "\n";
    }

    // Default targets
    if (!defaults.empty()) {
        oss << "\ndefault";
        for (const auto& def : defaults) {
            oss << " " << def;
        }
        oss << "\n";
    }

    return oss.str();
}

std::string BuildFile::toCodeString() const {
    std::ostringstream oss;
    oss << "#include \"../manifest.h\"\n\n";
    oss << "using namespace shadowdash;\n\n";
    oss << "void manifest() {\n";

    // Global variables
    for (const auto& var : globalVariables) {
        oss << "  let(" << var.first << ", \"" << var.second << "\");\n";
    }
    oss << "\n";

    // Rules
    for (const auto& rulePair : rules) {
        oss << "  auto " << rulePair.first << " = rule{ {\n";
        oss << "      bind(command";

        // Split command string and process tokens
        std::istringstream commandStream(rulePair.second.command);
        std::string token;
        while (commandStream >> token) {
            if (token == "$") {
                continue;  // Skip standalone $ tokens
            }
            // Handle special tokens
            if (token == "flags") {
                oss << ", \"flags\"_v";
            } else if (token == "in") {
                oss << ", in";
            } else if (token == "out") {
                oss << ", out";
            } else {
                oss << ", \"" << token << "\"";
            }
        }
        oss << "),\n";

        for (const auto& var : rulePair.second.variables) {
            oss << "      bind(" << var.first << ", \"" << var.second << "\"),\n";
        }
        oss << "  } };\n\n";
    }

    // Build targets
    for (const auto& target : buildTargets) {
        oss << "  build(list{ str{ \"" << target.output << "\" } },\n";
        oss << "        {},\n";
        oss << "        " << target.ruleName << ",\n";
        oss << "        list{ ";
        for (size_t i = 0; i < target.inputs.size(); ++i) {
            oss << "str{ \"" << target.inputs[i] << "\" }";
            if (i < target.inputs.size() - 1) oss << ", ";
        }
        oss << " },\n";
        oss << "        {},\n";
        oss << "        {},\n";
        oss << "        {},\n";
        if (!target.variables.empty()) {
            oss << "        { ";
            for (auto it = target.variables.begin(); it != target.variables.end(); ++it) {
                oss << "bind(" << it->first << ", \"" << it->second << "\")";
                if (std::next(it) != target.variables.end()) oss << ", ";
            }
            oss << " }\n";
        } else {
            oss << "        {}\n";
        }
        oss << "  );\n\n";
    }

    oss << "}\n";
    return oss.str();
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