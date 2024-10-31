// token_types.hpp
#pragma once

enum class TokenType {
    // Keywords
    RULE,       // 'rule'
    BUILD,      // 'build'
    DEFAULT,    // 'default'
    COMMAND,    // 'command'
    
    // Variables
    VARIABLE,   // e.g., 'flags'
    IDENTIFIER, // Names of rules, targets, etc.
    
    // Operators
    EQUALS,     // '='
    COLON,      // ':'
    DOLLAR,     // '$'
    
    // Values
    STRING,     // Any string value
    PATH,       // File paths
    FLAG,       // Compiler flags like -O2, -c
    
    // Special
    NEWLINE,    // Line endings are significant in build files
    INDENT,     // Indentation is significant
    EOF_TOKEN,  // End of file
    
    // Separators
    SPACE,      // Spaces between elements
    LPAREN,     // '('
    RPAREN,     // ')'
    
    // Special Variables
    IN,         // $in
    OUT,        // $out

    COMMENT,    // Comments starting with #

    // Invalid
    INVALID
};