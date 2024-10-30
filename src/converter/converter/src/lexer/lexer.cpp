// lexer.cpp
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "lexer/token_types.hpp"


Lexer::Lexer(std::string source) 
    : source_(std::move(source)) {}

Token Lexer::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::EOF_TOKEN, "");
    }

    char c = advance();

    // Basic cases are enough
    switch (c) {
        case ':': return Token(TokenType::COLON, ":");
        case '=': return Token(TokenType::EQUALS, "=");
        case '\n': return Token(TokenType::NEWLINE, "\n");
        case '$': return Token(TokenType::DOLLAR, "$");
    }

    // Handle identifiers and paths
    if (std::isalpha(c) || c == '_') {
        return scanIdentifier();
    }
    
    // Handle paths and flags
    if (c == '.' || c == '/' || c == '-') {
        return scanPath();
    }

    return Token(TokenType::INVALID, std::string(1, c));
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        tokens_.push_back(nextToken());
    }
    return tokens_;
}

// lexer.cpp (additional implementations)

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else {
            break;
        }
    }
}

Token Lexer::scanIdentifier() {
    size_t start = current_ - 1;  // -1 because we already advanced once
    
    while (!isAtEnd()) {
        char c = peek();
        if (std::isalnum(c) || c == '_') {
            advance();
        } else {
            break;
        }
    }
    
    std::string identifier = source_.substr(start, current_ - start);
    return Token(TokenType::IDENTIFIER, identifier);
}

Token Lexer::scanPath() {
    size_t start = current_ - 1;  // -1 because we already advanced once
    
    while (!isAtEnd()) {
        char c = peek();
        // Allow most characters in paths except whitespace and special characters
        if (!std::isspace(c) && c != ':' && c != '=' && c != '$') {
            advance();
        } else {
            break;
        }
    }
    
    std::string path = source_.substr(start, current_ - start);
    
    // If it starts with '-', it's likely a flag
    if (path[0] == '-') {
        return Token(TokenType::FLAG, path);
    }
    return Token(TokenType::PATH, path);
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.length();
}

char Lexer::advance() {
    return source_[current_++];
}

char Lexer::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source_[current_];
}