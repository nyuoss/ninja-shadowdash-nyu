// lexer.cpp
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "lexer/token_types.hpp"
#include <iostream> // Add this line for debugging output

Lexer::Lexer(std::string source) 
    : source_(std::move(source)) {}

Token Lexer::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::EOF_TOKEN, "");
    }

    char c = advance();

    switch (c) {
        case ':': return Token(TokenType::COLON, ":");
        case '=': return Token(TokenType::EQUALS, "=");
        case '\n': return Token(TokenType::NEWLINE, "\n");
        case '$': return Token(TokenType::DOLLAR, "$");
        case '#': return scanComment(); // Handle comments
        default:
            if (std::isalpha(c) || c == '_' || c == '.' || c == '-') {
                return scanIdentifierOrPath();
            } else {
                std::cout << "Invalid token: " << c << " at position " << current_ << std::endl; // Debugging output
                return Token(TokenType::INVALID, std::string(1, c));
            }
    }
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        Token token = nextToken();
        std::cout << "Scanned token: type=" << static_cast<int>(token.type()) << " lexeme=" << token.lexeme() << std::endl; // Debugging output
        tokens_.push_back(token);
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

Token Lexer::scanComment() {
    size_t start = current_ - 1;  // -1 because we already advanced once
    
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
    
    std::string comment = source_.substr(start, current_ - start);
    std::cout << "Comment: " << comment << " at position " << start << std::endl; // Debugging output
    return Token(TokenType::COMMENT, comment);
}

Token Lexer::scanIdentifierOrPath() {
    size_t start = current_ - 1;  // -1 because we already advanced once

    while (!isAtEnd()) {
        char c = peek();
        // Allow alphanumeric characters, underscores, dots, dashes, and plus signs
        if (std::isalnum(c) || c == '_' || c == '.' || c == '-' || c == '+') {
            advance();
        } else {
            break;
        }
    }

    std::string lexeme = source_.substr(start, current_ - start);
    std::cout << "Lexeme: " << lexeme << " at position " << start << std::endl; // Debugging output

    // Check if the lexeme is a keyword
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"rule", TokenType::RULE},
        {"build", TokenType::BUILD},
        {"default", TokenType::DEFAULT},
        {"command", TokenType::COMMAND}
        // Add other keywords as needed
    };

    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        // It's a keyword
        return Token(it->second, lexeme);
    } else {
        // If it starts with '-', it's likely a flag
        if (lexeme[0] == '-') {
            return Token(TokenType::FLAG, lexeme);
        } else if (lexeme.find('.') != std::string::npos || lexeme.find('/') != std::string::npos) {
            // If it contains a dot or slash, treat it as a PATH
            return Token(TokenType::PATH, lexeme);
        } else {
            // Otherwise, treat it as an IDENTIFIER
            return Token(TokenType::IDENTIFIER, lexeme);
        }
    }
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