// tokens.cpp
#include "lexer/token.hpp"
#include <stdexcept>

// Keywords mapping
static const std::unordered_map<std::string_view, TokenType> keywords = {
    {"rule", TokenType::RULE},
    {"build", TokenType::BUILD},
    {"default", TokenType::DEFAULT},
    {"command", TokenType::COMMAND}
};

// Special variables mapping
static const std::unordered_map<std::string_view, TokenType> special_vars = {
    {"in", TokenType::IN},
    {"out", TokenType::OUT}
};

Token::Token(TokenType type, std::string lexeme, std::any literal, int line, int column)
    : type_(type), lexeme_(std::move(lexeme)), literal_(std::move(literal)), line_(line), column_(column) {}

Token Token::createToken(TokenType type, 
                        std::string lexeme, 
                        std::any literal,
                        int line, 
                        int column) {
    return Token(type, std::move(lexeme), std::move(literal), line, column);
}

Token Token::createRuleToken(const std::string& name, int line, int column) {
    return Token(TokenType::RULE, name, name, line, column);
}

Token Token::createBuildToken(int line, int column) {
    return Token(TokenType::BUILD, "build", std::any(), line, column);
}

Token Token::createVariableToken(const std::string& name, int line, int column) {
    return Token(TokenType::VARIABLE, name, name, line, column);
}

Token Token::createPathToken(const std::string& path, int line, int column) {
    return Token(TokenType::PATH, path, path, line, column);
}

Token Token::createFlagToken(const std::string& flag, int line, int column) {
    return Token(TokenType::FLAG, flag, flag, line, column);
}

Token Token::createCommentToken(const std::string& comment, int line, int column) {
    return Token(TokenType::COMMENT, comment, comment, line, column);
}

bool Token::isKeyword() const {
    return keywords.find(lexeme_) != keywords.end();
}

bool Token::isVariable() const {
    return type_ == TokenType::VARIABLE || 
           type_ == TokenType::IN || 
           type_ == TokenType::OUT;
}

bool Token::isOperator() const {
    return type_ == TokenType::EQUALS || 
           type_ == TokenType::COLON || 
           type_ == TokenType::DOLLAR;
}

bool Token::isPath() const {
    return type_ == TokenType::PATH;
}

bool Token::isFlag() const {
    return type_ == TokenType::FLAG;
}

// Example of how parsing might work for your build file:
// flags = -O3
// Tokens: [VARIABLE("flags"), EQUALS, FLAG("-O3"), NEWLINE]

// rule compile
// Tokens: [RULE, IDENTIFIER("compile"), NEWLINE]

// command = g++ $flags -c $in -o $out
// Tokens: [VARIABLE("command"), EQUALS, IDENTIFIER("g++"), 
//         DOLLAR, VARIABLE("flags"), FLAG("-c"), 
//         DOLLAR, IN, FLAG("-o"), DOLLAR, OUT, NEWLINE]

// build hello.o: compile hello.cpp
// Tokens: [BUILD, PATH("hello.o"), COLON, 
//         IDENTIFIER("compile"), PATH("hello.cpp"), NEWLINE]