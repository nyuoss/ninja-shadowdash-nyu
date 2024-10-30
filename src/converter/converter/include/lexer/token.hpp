// tokens.hpp
#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <any>
#include "token_types.hpp"

class Token {
public:
	Token(TokenType type, std::string lexeme)
    : type_(type)
    , lexeme_(std::move(lexeme))
    , literal_()  // empty any
    , line_(0)    // default line
    , column_(0)  // default column
{
}
    static Token createToken(TokenType type, 
                           std::string lexeme, 
                           std::any literal = std::any(),
                           int line = 0, 
                           int column = 0);

    // Specific factory methods for common tokens
    static Token createRuleToken(const std::string& name, int line, int column);
    static Token createBuildToken(int line, int column);
    static Token createVariableToken(const std::string& name, int line, int column);
    static Token createPathToken(const std::string& path, int line, int column);
    static Token createFlagToken(const std::string& flag, int line, int column);

    // Getters
    TokenType type() const { return type_; }
    const std::string& lexeme() const { return lexeme_; }
    const std::any& literal() const { return literal_; }
    int line() const { return line_; }
    int column() const { return column_; }

    // Utility methods
    bool isKeyword() const;
    bool isVariable() const;
    bool isOperator() const;
    bool isPath() const;
    bool isFlag() const;

private:
    Token(TokenType type, 
          std::string lexeme, 
          std::any literal,
          int line, 
          int column);

    TokenType type_;
    std::string lexeme_;
    std::any literal_;
    int line_;
    int column_;
};