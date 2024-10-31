#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "build/build_elements.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printBuildFile(const BuildFile& buildFile) {
    std::cout << "Global Variables:\n";
    for (const auto& [key, value] : buildFile.globalVariables) {
        std::cout << key << " = " << value << "\n";
    }

    std::cout << "\nRules:\n";
    for (const auto& [name, rule] : buildFile.rules) {
        std::cout << "rule " << name << "\n";
        std::cout << "  command = " << rule.command << "\n";
        for (const auto& [key, value] : rule.variables) {
            std::cout << "  " << key << " = " << value << "\n";
        }
    }

    std::cout << "\nBuild Targets:\n";
    for (const auto& target : buildFile.buildTargets) {
        std::cout << "build " << target.output << ": " << target.ruleName;
        for (const auto& input : target.inputs) {
            std::cout << " " << input;
        }
        std::cout << "\n";
        for (const auto& [key, value] : target.variables) {
            std::cout << "  " << key << " = " << value << "\n";
        }

        // Print the expanded command
        std::cout << "  Expanded Command: " << target.getExpandedCommand(buildFile) << "\n";
    }

    std::cout << "\nDefault Targets:\n";
    for (const auto& def : buildFile.defaults) {
        std::cout << def << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <build file>\n";
        return 1;
    }

    try {
        std::string filePath = argv[1];
        std::string source = readFile(filePath);

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();

        Parser parser(tokens);

        // Parse the input and build the BuildFile object
        BuildFile buildFile = parser.parse();

        // Generate the code representation
        std::string output = buildFile.toCodeString();

        // Write the output to 'build.ninja.cc'
        std::ofstream outFile("build.ninja.cc");
        if (outFile.is_open()) {
            outFile << output;
            outFile.close();
        } else {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}