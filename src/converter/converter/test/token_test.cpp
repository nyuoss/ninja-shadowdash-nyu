#include <gtest/gtest.h>
#include "lexer/token.hpp"

TEST(TokenTest, BasicTokenCreation) {
    Token token(TokenType::RULE, "rule");
    EXPECT_EQ(token.type(), TokenType::RULE);
    EXPECT_EQ(token.lexeme(), "rule");
}

TEST(TokenTest, TokenFactoryMethods) {
    auto ruleToken = Token::createRuleToken("compile", 1, 1);
    EXPECT_EQ(ruleToken.type(), TokenType::RULE);
    EXPECT_EQ(ruleToken.lexeme(), "compile");
}

TEST(TokenTest, CreateToken) {
    Token token = Token::createToken(TokenType::RULE, "rule");
    EXPECT_EQ(token.type(), TokenType::RULE);
    EXPECT_EQ(token.lexeme(), "rule");
}

TEST(TokenTest, IsKeyword) {
    Token ruleToken = Token::createToken(TokenType::RULE, "rule");
    Token identToken = Token::createToken(TokenType::IDENTIFIER, "somevar");
    EXPECT_TRUE(ruleToken.isKeyword());
    EXPECT_FALSE(identToken.isKeyword());
}

TEST(TokenTest, IsVariable) {
    Token varToken = Token::createVariableToken("flags", 1, 1);
    Token inToken = Token::createToken(TokenType::IN, "in");
    Token outToken = Token::createToken(TokenType::OUT, "out");
    EXPECT_TRUE(varToken.isVariable());
    EXPECT_TRUE(inToken.isVariable());
    EXPECT_TRUE(outToken.isVariable());
}

TEST(TokenTest, IsOperator) {
    Token equalsToken = Token::createToken(TokenType::EQUALS, "=");
    Token colonToken = Token::createToken(TokenType::COLON, ":");
    EXPECT_TRUE(equalsToken.isOperator());
    EXPECT_TRUE(colonToken.isOperator());
}
