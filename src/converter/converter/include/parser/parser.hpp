// parser.hpp
#pragma once
#include "lexer/token.hpp"
#include "build/build_elements.hpp"
#include <vector>
#include <string>
#include <map>

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    BuildFile buildFile;
    
    // Token manipulation methods
    Token peek() const;          // Look at current token without consuming
    Token advance();            // Get current token and advance to next
    bool isAtEnd() const;       // Check if we've reached end of tokens
    bool match(TokenType type); // Check if current token matches type and consume if so
    
    // Parsing helper methods
    void parseRule();           // Parse a 'rule' declaration
    void parseBuild();          // Parse a 'build' declaration
    void parseVariable(std::map<std::string, std::string>& variables); // Parse variable assignment
    void parseDefault();        // Parse a 'default' declaration

public:
    explicit Parser(std::vector<Token> tokens) 
        : tokens(std::move(tokens)), current(0) {}
    
    BuildFile parse();          // Main parsing method that returns complete BuildFile
};