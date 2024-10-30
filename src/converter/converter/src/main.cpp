// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "include/lexer/lexer.hpp"
#include "include/parser/parser.hpp"
#include "include/build/build_elements.hpp"

class NinjaConverter {
public:
    explicit NinjaConverter(bool verbose = false) : verbose_(verbose) {}

    bool convertFile(const std::string& inputPath, const std::string& outputPath) {
        try {
            // Read input file
            std::string source = readFile(inputPath);
            if (verbose_) {
                std::cout << "Read " << source.length() << " bytes from " << inputPath << "\n";
            }

            // Lexical analysis
            Lexer lexer(source);
            auto tokens = lexer.scanTokens();
            if (verbose_) {
                std::cout << "Lexer produced " << tokens.size() << " tokens\n";
                printTokens(tokens);
            }

            // Parsing
            Parser parser(tokens);
            auto buildFile = parser.parse();
            if (verbose_) {
                std::cout << "Parser created AST with " 
                         << buildFile->statements().size() << " statements\n";
                printAST(*buildFile);
            }

            // Generate output
            std::string output = generateOutput(*buildFile);
            
            // Write to output file
            writeFile(outputPath, output);
            if (verbose_) {
                std::cout << "Written output to " << outputPath << "\n";
            }

            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return false;
        }
    }

private:
    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file) {
            throw std::runtime_error("Cannot open input file: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static void writeFile(const std::string& path, const std::string& content) {
        std::ofstream file(path);
        if (!file) {
            throw std::runtime_error("Cannot open output file: " + path);
        }
        file << content;
    }

    void printTokens(const std::vector<Token>& tokens) const {
        std::cout << "\nTokens:\n";
        for (const auto& token : tokens) {
            std::cout << "  " << static_cast<int>(token.type) 
                     << ": '" << token.text << "'\n";
        }
        std::cout << "\n";
    }

    void printAST(const BuildFile& file) const {
        std::cout << "\nAbstract Syntax Tree:\n";
        for (const auto& stmt : file.statements()) {
            if (auto build = dynamic_cast<const BuildStatement*>(stmt.get())) {
                printBuildStatement(*build);
            } else if (auto rule = dynamic_cast<const RuleStatement*>(stmt.get())) {
                printRuleStatement(*rule);
            } else if (auto var = dynamic_cast<const VariableStatement*>(stmt.get())) {
                printVariableStatement(*var);
            }
        }
        std::cout << "\n";
    }

    void printBuildStatement(const BuildStatement& stmt) const {
        std::cout << "  Build:\n";
        std::cout << "    Outputs: ";
        for (const auto& output : stmt.outputs()) {
            std::cout << output << " ";
        }
        std::cout << "\n    Rule: " << stmt.rule() << "\n";
        std::cout << "    Inputs: ";
        for (const auto& input : stmt.inputs()) {
            std::cout << input << " ";
        }
        std::cout << "\n";
        for (const auto& [name, value] : stmt.variables()) {
            std::cout << "    Var " << name << " = " << value << "\n";
        }
    }

    void printRuleStatement(const RuleStatement& stmt) const {
        std::cout << "  Rule: " << stmt.name() << "\n";
        for (const auto& [name, value] : stmt.variables()) {
            std::cout << "    " << name << " = " << value << "\n";
        }
    }

    void printVariableStatement(const VariableStatement& stmt) const {
        std::cout << "  Variable: " << stmt.name() << " = " << stmt.value() << "\n";
    }

    std::string generateOutput(const BuildFile& file) {
        std::stringstream output;
        
        // Here you would implement the conversion to your target build system
        // This is a placeholder that just creates a simple text representation
        for (const auto& stmt : file.statements()) {
            if (auto build = dynamic_cast<const BuildStatement*>(stmt.get())) {
                output << "build(";
                for (const auto& out : build->outputs()) {
                    output << out << " ";
                }
                output << "): " << build->rule() << " <";
                for (const auto& in : build->inputs()) {
                    output << in << " ";
                }
                output << ">\n";
            } else if (auto rule = dynamic_cast<const RuleStatement*>(stmt.get())) {
                output << "rule " << rule->name() << " {\n";
                for (const auto& [name, value] : rule->variables()) {
                    output << "  " << name << " = " << value << "\n";
                }
                output << "}\n";
            } else if (auto var = dynamic_cast<const VariableStatement*>(stmt.get())) {
                output << "var " << var->name() << " = " << var->value() << "\n";
            }
        }
        
        return output.str();
    }

    bool verbose_;
};

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " [options] input_file output_file\n"
              << "Options:\n"
              << "  -v, --verbose    Enable verbose output\n"
              << "  -h, --help       Display this help message\n";
}

int main(int argc, char* argv[]) {
    bool verbose = false;
    std::string inputFile;
    std::string outputFile;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if (inputFile.empty()) {
            inputFile = argv[i];
        } else if (outputFile.empty()) {
            outputFile = argv[i];
        } else {
            std::cerr << "Unexpected argument: " << argv[i] << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (inputFile.empty() || outputFile.empty()) {
        std::cerr << "Input and output files must be specified\n";
        printUsage(argv[0]);
        return 1;
    }

    NinjaConverter converter(verbose);
    if (!converter.convertFile(inputFile, outputFile)) {
        return 1;
    }

    return 0;
}