#include <iostream>
#include <string>
#include <map>
#include <vector>

// Assuming you have these constants
enum EvalStringType { RAW, SPECIAL };

// Example structures based on your description
struct EvalString {
    std::string first;
    EvalStringType second;
};

struct Binding {
    std::vector<EvalString> parsed_;
};

struct Rule {
    std::string name_;
    std::map<std::string, Binding> bindings_;
};

int main() {
    // Simulate your object
    Rule rule;
    rule.name_ = "compile";
    rule.bindings_["command"].parsed_ = {
        {"g++ -c ", RAW},
        {"in", SPECIAL},
        {" -o ", RAW},
        {"out", SPECIAL},
        {" ", RAW}
    };
    
    // Begin the output string
    std::string output = "auto " + rule.name_ + " = rule{{bind(command, \"g++\", \"flags\"_v, \"-c\", ";

    // Iterate over the parsed_ vector in the "command" binding
    for (const auto& entry : rule.bindings_["command"].parsed_) {
        if (entry.second == RAW) {
            output += "\"" + entry.first + "\", ";  // Add quotes for raw strings
        } else if (entry.second == SPECIAL) {
            output += entry.first + ", ";  // No quotes for special variables
        }
    }

    // Finish the string
    output += ")}};";
    
    // Print the result
    std::cout << output << std::endl;

    return 0;
}
