// this file is the main header file of shadowdash new language design
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <optional>

namespace shadowdash {

// Token Class
class Token {
public:
    // We have two kinds of tokens
    // LITERAL: literal token, do not subject to changes
    // VAR: variable token, actual value may be stored in the environments
    enum Type { LITERAL, VAR };

    Token(Type type, std::string value)
        : type_(type), value_(value) {}

    Token(std::string value) : Token(Token::LITERAL, value) {}

    Token(const char* value) : Token(Token::LITERAL, std::string(value)) {}

    Type type_;
    std::string value_;
};

// For user friendly Token definition
Token operator"" _l(const char* value, std::size_t len) {
    return Token(Token::Type::LITERAL, std::string(value, len));
}

Token operator"" _v(const char* value, std::size_t len) {
    return Token(Token::Type::VAR, std::string(value, len));
}

// str class, stores a list of tokens
class str_ {
public:
    str_(std::vector<Token> tokens) : tokens_(tokens) {}

    std::vector<Token> tokens_;
};

// binding is a pair of string (name) to a str (list of shadowdash tokens)
using binding = std::pair<std::string, str_>;
// a map is a list of bindings
using map = std::vector<binding>;

// list class
class list_ {
public:
    list_(std::vector<str_> values) : values_(values) {}

    std::vector<str_> values_;
};

// a shadowdash variable, may be stored into the environment
class var {
public:
    var(const char* name, str_ value) : name_(name), value_(value) {}

    const char* name_;
    str_ value_;
};

// ninja has some built-in rules, like phony
// we need to classify them into normal rule or special rule
class rule__ {
public:
    // currently we only found `phony` as one of the special rule in ninja
    enum SPECIAL_RULE {
        phony
    };

    // a rule may be a special rule, or may be a normal rule
    union _rule {
        _rule() : sp_rule(phony) {}
        _rule(map bindings) : bindings_(bindings) {}
        _rule(SPECIAL_RULE sr) : sp_rule(sr) {}
        ~_rule() {}

        map bindings_;
        SPECIAL_RULE sp_rule;
    } _rule_data;
    bool is_special;

    // Constructor for regular and special rules
    rule__(map bindings) : _rule_data(bindings), is_special(false) {}
    rule__(SPECIAL_RULE sp_rule) : _rule_data(sp_rule), is_special(true) {}

    // Copy constructor, handles both regular and special rules
    rule__(const rule__& other) : is_special(other.is_special) {
        if (is_special) {
            new (&_rule_data) _rule(other._rule_data.sp_rule);
        } else {
            new (&_rule_data) _rule(other._rule_data.bindings_);
        }
    }
};
#define phony shadowdash::rule__::phony

// build class
class build {
public:
    build(
        list_ outputs,
        list_ implicit_outputs,
        rule__ rule,
        list_ inputs,
        list_ implicit_inputs,
        list_ order_only_inputs,
        map bindings)
        : outputs_(outputs),
          implicit_outputs_(implicit_outputs),
          rule_(rule),
          inputs_(inputs),
          implicit_inputs_(implicit_inputs),
          order_only_inputs_(order_only_inputs),
          bindings_(bindings) {
    }

    list_ outputs_;
    list_ implicit_outputs_;
    rule__ rule_;
    list_ inputs_;
    list_ implicit_inputs_;
    list_ order_only_inputs_;
    map bindings_;
    std::string pool;
};

// Define a resource pool with name and maximum depth
class pool {
public:
    pool(
        str_ name,
        int depth)
        : name_(name),
          depth_(depth) {}; // Maximum allowable concurrent tasks in this pool.

    str_ name_;
    int depth_;
};

// Represent a default target in the build configuration
class shadowdash_default {
public:
    shadowdash_default(
        list_ target)
      : target_(target) {};

      list_ target_;
};

// ShadowDash class stores the entire manifest (list of builds, pools and default)
class ShadowDash_ {
public:
    ShadowDash_(std::vector<build> builds) : builds(builds), pools({}), default_({}) {}
    ShadowDash_(std::vector<build> builds, std::vector<pool> pools) : builds(builds), pools(pools), default_({}) {}
    ShadowDash_(std::vector<build> builds, std::vector<pool> pools, shadowdash_default default_) : builds(builds), pools(pools), default_({default_}) {}

    std::vector<build> builds;
    std::vector<pool> pools;
    std::vector<shadowdash_default> default_;
};

static auto in = "in"_v;
static auto out = "out"_v;

} // namespace shadowdash

// Macors to simplify the syntax for defining vairables, offering a concise way to create ShadowDash configs.
#define let(name, ...)         \
    var name {                \
        #name, str_ {         \
            __VA_ARGS__      \
        }                    \
    }

#define bind(name, ...)        \
    {                          \
        #name, str_ {         \
            __VA_ARGS__      \
        }                    \
    }

#define new_pool(name, depth) \
  pool name = pool(str({#name}), depth)

#define default(...) \
    auto default_target = shadowdash_default(list(__VA_ARGS__));

#define list(...) \
    list_{{__VA_ARGS__}}

#define rule(name, ...) \
    rule__ name = rule__{{__VA_ARGS__}}

#define str(...) \
    str_{{__VA_ARGS__}}

#define ShadowDash return ShadowDash_

#define manifest ShadowDash_ manifest

#ifndef SHADOWDASH_SOURCE
using namespace shadowdash;
#endif