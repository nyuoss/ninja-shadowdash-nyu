#pragma once

#include <string>
#include <vector>
#include <utility>

namespace shadowdash {

class Token {
public:
    enum Type { LITERAL, VAR };

    Token(Type type, std::string value);

    Token(std::string value);

    Token(const char* value);

    Type type_;
    std::string value_;
};

class str {
public:
    str(std::vector<Token> tokens);

    std::vector<Token> tokens_;
};

using binding = std::pair<std::string, str>;
using map = std::vector<binding>;

class list {
public:
    list(std::vector<str> values);

    std::vector<str> values_;
};

class var {
public:
    var(const char* name, str value);

    std::string name_;
    str value_;
};

class rule {
public:
    rule(const char* name, map bindings);
};

class build {
public:
    build(
        list outputs,
        list implicit_outputs,
        const char* rule,
        list inputs,
        list implicit_inputs,
        list order_only_inputs,
        map bindings);
};

} // namespace shadowdash

#define let(name, ...)         \
    var name {                \
        #name, str {         \
            __VA_ARGS__      \
        }                    \
    }

#define bind(name, ...)        \
    {                          \
        #name, str {         \
            __VA_ARGS__      \
        }                    \
    }
