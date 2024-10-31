// parser.cpp
#include "parser/parser.hpp"
#include "lexer/token.hpp"
#include <stdexcept>
#include <iostream> // Add this line for debugging output

Token Parser::peek() const {
    if (isAtEnd()) return Token(TokenType::EOF_TOKEN, "");
    return tokens[current];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return current >= tokens.size();
}

bool Parser::match(TokenType type) {
    if (isAtEnd()) return false;
    if (peek().type() != type) return false;
    advance();
    return true;
}

void Parser::parseVariable(std::map<std::string, std::string>& variables) {
    Token name = advance(); // Variable name
    if (name.type() != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected variable name at line " + 
                               std::to_string(name.line()));
    }
    
    std::cout << "Variable name: " << name.lexeme() << " at line " << name.line() << std::endl; // Debugging output
    
    if (!match(TokenType::EQUALS)) {
        std::cout << "Next token type: " << static_cast<int>(peek().type()) << " lexeme: " << peek().lexeme() << std::endl; // Debugging output
        throw std::runtime_error("Expected '=' after variable name at line " + 
                               std::to_string(name.line()));
    }
    
    // Collect all tokens until NEWLINE as the variable value
    std::string valueString;
    while (!isAtEnd() && peek().type() != TokenType::NEWLINE) {
        Token valueToken = advance();
        valueString += valueToken.lexeme();
        if (!isAtEnd() && peek().type() != TokenType::NEWLINE) {
            valueString += ' '; // Add a space between tokens
        }
    }
    
    std::cout << "Variable value: " << valueString << std::endl; // Debugging output
    variables[name.lexeme()] = valueString;
}

void Parser::parseRule() {
    Token name = advance(); // Rule name
    if (name.type() != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected rule name at line " + 
                               std::to_string(name.line()));
    }
    
    Rule rule;
    rule.name = name.lexeme();
    
    // Consume any NEWLINES
    while (!isAtEnd() && peek().type() == TokenType::NEWLINE) {
        advance(); // consume newline
    }
    
    // Parse rule body
    while (!isAtEnd() && (peek().type() == TokenType::IDENTIFIER || peek().type() == TokenType::COMMAND)) {
        if (peek().lexeme() == "command" || peek().type() == TokenType::COMMAND) {
            advance(); // consume "command"
            
            if (!match(TokenType::EQUALS)) {
                throw std::runtime_error("Expected '=' after command at line " + 
                                       std::to_string(peek().line()));
            }
            
            // Collect all tokens until NEWLINE as the command value
            std::string commandString;
            while (!isAtEnd() && peek().type() != TokenType::NEWLINE) {
                Token commandToken = advance();
                commandString += commandToken.lexeme();
                if (!isAtEnd() && peek().type() != TokenType::NEWLINE) {
                    commandString += ' '; // Add a space between tokens
                }
            }
            
            std::cout << "Command string: " << commandString << std::endl; // Debugging output
            rule.command = commandString;
        } else {
            parseVariable(rule.variables);
        }
        
        if (!match(TokenType::NEWLINE)) {
            throw std::runtime_error("Expected newline after rule definition at line " + 
                                   std::to_string(peek().line()));
        }
    }
    
    buildFile.rules[name.lexeme()] = rule;
}

void Parser::parseBuild() {
    BuildTarget target;
    
    // Parse output file
    Token output = advance();
    if (output.type() != TokenType::PATH && output.type() != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected output file at line " + 
                               std::to_string(output.line()));
    }
    target.output = output.lexeme();
    
    if (!match(TokenType::COLON)) {
        throw std::runtime_error("Expected ':' after output file at line " + 
                               std::to_string(peek().line()));
    }
    
    // Parse rule name
    Token ruleName = advance();
    if (ruleName.type() != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected rule name at line " + 
                               std::to_string(ruleName.line()));
    }
    target.ruleName = ruleName.lexeme();
    
    // Parse input files
    while (!isAtEnd() && (peek().type() == TokenType::PATH || peek().type() == TokenType::IDENTIFIER)) {
        target.inputs.push_back(advance().lexeme());
    }
    
    // Parse variables (if any)
    while (!isAtEnd() && peek().type() == TokenType::NEWLINE) {
        advance(); // consume newline
    }
    
    while (!isAtEnd() && peek().type() == TokenType::IDENTIFIER) {
        parseVariable(target.variables);
        
        if (!match(TokenType::NEWLINE)) {
            throw std::runtime_error("Expected newline after build variable at line " + 
                                   std::to_string(peek().line()));
        }
    }
    
    buildFile.buildTargets.push_back(target);
}

void Parser::parseDefault() {
    while (!isAtEnd() && (peek().type() == TokenType::IDENTIFIER || peek().type() == TokenType::PATH)) {
        buildFile.defaults.push_back(advance().lexeme());
    }
    
    if (!match(TokenType::NEWLINE)) {
        throw std::runtime_error("Expected newline after default target at line " + 
                               std::to_string(peek().line()));
    }
}

BuildFile Parser::parse() {
    while (!isAtEnd()) {
        Token token = peek();
        
        if (token.type() == TokenType::NEWLINE) {
            advance();
            continue;
        }
        
        if (token.type() == TokenType::RULE) {
            advance(); // consume 'rule' token
            parseRule();
        } else if (token.type() == TokenType::BUILD) {
            advance(); // consume 'build' token
            parseBuild();
        } else if (token.type() == TokenType::DEFAULT) {
            advance(); // consume 'default' token
            parseDefault();
        } else if (token.type() == TokenType::IDENTIFIER) {
            parseVariable(buildFile.globalVariables);
        } else if (token.type() == TokenType::COMMENT) {
            advance(); // Skip comments
        } else {
            std::cout << "Unexpected token type: " << static_cast<int>(token.type()) << " lexeme: " << token.lexeme() << std::endl; // Debugging output
            throw std::runtime_error("Unexpected token at line " +
                                   std::to_string(token.line()));
        }
    }
    
    return buildFile;
}