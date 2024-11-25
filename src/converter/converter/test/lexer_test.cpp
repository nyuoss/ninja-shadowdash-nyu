#include "gtest/gtest.h"
#include "lexer/lexer.hpp"

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(LexerTest, EmptyInput) {
    Lexer lexer("");
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type(), TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, SimpleRule) {
    Lexer lexer("rule cc");
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type(), TokenType::RULE);
    EXPECT_EQ(tokens[1].type(), TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].type(), TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, BuildStatement) {
    Lexer lexer("build output.o: compile input.cpp");
    auto tokens = lexer.scanTokens();

    // Debug print tokens
    for (const auto& token : tokens) {
        std::cout << "Type: " << static_cast<int>(token.type()) 
                  << ", Lexeme: '" << token.lexeme() << "'\n";
    }

    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens[0].type(), TokenType::BUILD);
    ASSERT_EQ(tokens[1].type(), TokenType::PATH);
    ASSERT_EQ(tokens[2].type(), TokenType::COLON);
    ASSERT_EQ(tokens[3].type(), TokenType::IDENTIFIER);
    ASSERT_EQ(tokens[4].type(), TokenType::PATH);
}

TEST_F(LexerTest, Variables) {
    Lexer lexer("cflags = -Wall");
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type(), TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].type(), TokenType::EQUALS);
    EXPECT_EQ(tokens[2].type(), TokenType::FLAG);
    EXPECT_EQ(tokens[3].type(), TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, Comments) {
    Lexer lexer("# This is a comment");
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type(), TokenType::COMMENT);
    EXPECT_EQ(tokens[1].type(), TokenType::EOF_TOKEN);
}
