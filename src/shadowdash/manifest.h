#pragma once

#include <initializer_list>
#include <string_view>

namespace shadowdash {

class Token {
 public:
  enum Type { LITERAL, VAR };

  constexpr Token(Type type, std::string_view value)
      : type_(type), value_(value) {}

  constexpr Token(std::string_view value) : Token(Token::LITERAL, value) {}

  constexpr Token(const char* value) : Token(Token::LITERAL, value) {}

  Type type_;
  std::string_view value_;
};

constexpr Token operator"" _l(const char* value, std::size_t len) {
  return Token(Token::Type::LITERAL, { value, len });
}

constexpr Token operator"" _v(const char* value, std::size_t len) {
  return Token(Token::Type::VAR, { value, len });
}

class str {
 public:
  str(std::initializer_list<Token> tokens) : tokens_(tokens) {}
  std::initializer_list<Token> tokens_;
};

using binding = std::pair<std::string_view, str>;
using map = std::initializer_list<binding>;

class list {
 public:
  list(std::initializer_list<str> values) : values_(values) {}
  std::initializer_list<str> values_;
};

/*
Ninja: cflags = -Wall
Shadowdash: let(cflags, "-Wall");
*/
class var {
 public:
  var(const char* name, str value) {}
};

/*
Ninja: 
rule cc
    command = gcc $cflags -c $in -o $out
    description = Compile a CXX file

Shadowdash: 
rule cc = rule { {
    bind(command, "gcc", "cflags"_v, "-c", in, "-o", out),
    bind(description, "Compile a CXX file"),
} };
*/
class rule {
 public:
  rule(map bindings) {}
};

/*
Ninja:
build foo.o: cc foo.c
    cflags = -Wall

Shadowdash:
build(list{ str{ "foo.o" } },
    {},
    cc,
    list{ str{ "foo.c" } },
    {},
    {},
    { bind(cflags, "-Wall") }
);
*/
class build {
 public:
  build(                       //
      list outputs,            //
      list implicit_outputs,   //
      rule& rule,              //
      list inputs,             //
      list implicit_inputs,    //
      list order_only_inputs,  //
      map bindings)            //
  {}
};

static constexpr auto in = "in"_v;
static constexpr auto out = "out"_v;

/*
Parts of Ninja still left to be included in the design of Shadowdash: 
1. The phony rule
2. Default target statements
3. Pools
*/

}  // namespace shadowdash

#define let(name, ...) \
  var name {           \
    #name, str {       \
      __VA_ARGS__      \
    }                  \
  }

#define bind(name, ...) \
  {                     \
    #name, str {        \
      __VA_ARGS__       \
    }                   \
  }
