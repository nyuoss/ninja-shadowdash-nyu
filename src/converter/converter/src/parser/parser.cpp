// parser.cpp
#include "parser/parser.hpp"
#include "lexer/token.hpp"
#include <stdexcept>

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
    
    if (!match(TokenType::EQUALS)) {
        throw std::runtime_error("Expected '=' after variable name at line " + 
                               std::to_string(name.line()));
    }
    
    Token value = advance(); // Variable value
    if (value.type() != TokenType::STRING && value.type() != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected variable value at line " + 
                               std::to_string(value.line()));
    }
    
    variables[name.lexeme()] = value.lexeme();
}

void Parser::parseRule() {
    Token name = advance(); // Rule name
    if (name.type() != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected rule name at line " + 
                               std::to_string(name.line()));
    }
    
    Rule rule;
    rule.name = name.lexeme();
    
    // Parse rule body (must be indented)
    while (!isAtEnd() && peek().type() == TokenType::INDENT) {
        advance(); // consume indent
        
        if (peek().type() == TokenType::IDENTIFIER && 
            peek().lexeme() == "command") {
            advance(); // consume "command"
            
            if (!match(TokenType::EQUALS)) {
                throw std::runtime_error("Expected '=' after command at line " + 
                                       std::to_string(peek().line()));
            }
            
            Token command = advance();
            if (command.type() != TokenType::STRING) {
                throw std::runtime_error("Expected command string at line " + 
                                       std::to_string(command.line()));
            }
            
            rule.command = command.lexeme();
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
    if (output.type() != TokenType::STRING) {
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
    while (!isAtEnd() && peek().type() == TokenType::STRING) {
        target.inputs.push_back(advance().lexeme());
    }
    
    // Parse variables (if any)
    while (!isAtEnd() && peek().type() == TokenType::INDENT) {
        advance(); // consume indent
        parseVariable(target.variables);
        
        if (!match(TokenType::NEWLINE)) {
            throw std::runtime_error("Expected newline after build variable at line " + 
                                   std::to_string(peek().line()));
        }
    }
    
    buildFile.buildTargets.push_back(target);
}

void Parser::parseDefault() {
    while (!isAtEnd() && peek().type() == TokenType::STRING) {
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
        
        if (token.type() == TokenType::IDENTIFIER) {
            if (token.lexeme() == "rule") {
                parseRule();
            } else if (token.lexeme() == "build") {
                parseBuild();
            } else if (token.lexeme() == "default") {
                parseDefault();
            } else {
                parseVariable(buildFile.globalVariables);
            }
        } else {
            throw std::runtime_error("Unexpected token at line " + 
                                   std::to_string(token.line()));
        }
    }
    
    return buildFile;
}