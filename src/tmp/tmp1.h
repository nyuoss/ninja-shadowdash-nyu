#pragma once

#include <iostream>
#include <initializer_list>
#include <string>
#include <string_view>

namespace shadowdash {

class Token {
public:
    enum Type { LITERAL, VAR };

    // Constructor that takes a std::string to ensure ownership
    Token(Type type, std::string value)
        : type_(type), value_(std::move(value)) {}

    Token(std::string value) : Token(Token::LITERAL, std::move(value)) {}

    Type type_;
    std::string value_;  // Store std::string to manage its own memory
};

// User-defined literals for convenience
Token operator"" _l(const char* value, std::size_t len) {
    return Token(Token::LITERAL, std::string(value, len));
}

Token operator"" _v(const char* value, std::size_t len) {
    return Token(Token::VAR, std::string(value, len));
}

class str {
public:
    str(std::initializer_list<Token> tokens) : tokens_(tokens) {}
    std::initializer_list<Token> tokens_;
};

// Typedefs for readability
using binding = std::pair<std::string, str>;
using map = std::initializer_list<binding>;

class list {
public:
    list(std::initializer_list<str> values) : values_(values) {}
    std::initializer_list<str> values_;
};

class var {
public:
    var(const char* name, str value) : name_(name), value_(value) {}
    std::string name_;
    str value_;
};

class rule {
public:
    rule(map bindings) : bindings_(bindings) {}
    map bindings_;
};

class build {
public:
    build(
        list outputs,
        list implicit_outputs,
        rule& rule,
        list inputs,
        list implicit_inputs,
        list order_only_inputs,
        map bindings)
        : outputs_(outputs),
          implicit_outputs_(implicit_outputs),
          rule_(rule),
          inputs_(inputs),
          implicit_inputs_(implicit_inputs),
          order_only_inputs_(order_only_inputs),
          bindings_(bindings) {}

    list outputs_;
    list implicit_outputs_;
    rule& rule_;
    list inputs_;
    list implicit_inputs_;
    list order_only_inputs_;
    map bindings_;
};

class buildGroup {
public:
    buildGroup(std::initializer_list<build> builds) : builds(builds) {}
    std::initializer_list<build> builds;
};

// Static constexpr variables for convenience
static auto in = "in"_v;
static auto out = "out"_v;

} // namespace shadowdash

// Macros for convenient syntax
#define let(name, ...)        \
    var name {               \
        #name, str {        \
            __VA_ARGS__     \
        }                   \
    }

#define bind(name, ...)       \
    {                        \
        #name, str {        \
            __VA_ARGS__     \
        }                   \
    }

// Overload operator<< for printing
std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << (token.type_ == Token::LITERAL ? "LITERAL: " : "VAR: ") << token.value_;
    return os;
}

// Implement printing for the other classes as needed
std::ostream& operator<<(std::ostream& os, const str& s) {
    os << "str: ";
    for (const auto& token : s.tokens_) {
        os << token << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const var& v) {
    os << "var: " << v.name_ << ", value: " << v.value_;
    return os;
}

std::ostream& operator<<(std::ostream& os, const rule& r) {
    os << "rule: ";
    for (const auto& binding : r.bindings_) {
        os << binding.first << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const build& b) {
    os << "build: Outputs: ";
    for (const auto& output : b.outputs_.values_) {
        os << output << " ";
    }
    os << "Inputs: ";
    for (const auto& input : b.inputs_.values_) {
        os << input << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const buildGroup& bg) {
    os << "buildGroup: ";
    for (const auto& build : bg.builds) {
        os << build << " ";
    }
    return os;
}
