// lexer.hpp
#pragma once
#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
public:
   explicit Lexer(std::string source);
   std::vector<Token> scanTokens();

private:
   Token nextToken();
   void skipWhitespace();
   Token scanIdentifierOrPath(); // Updated method name
   Token scanComment();
   Token scanOperator(char firstChar); // Add this line
   
   bool isAtEnd() const;
   char advance(); 
   char peek() const;

   std::string source_;
   size_t current_ = 0;
   std::vector<Token> tokens_;
};