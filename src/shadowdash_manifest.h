#pragma once

#include <iostream>
#include <string_view>
#include <vector>
#include <utility>

namespace shadowdash {

class Token {
public:
    enum Type { LITERAL, VAR };

    Token(Type type, std::string_view value)
        : type_(type), value_(value) {
        //std::cout << "creating " << type << " : " << value_ << std::endl;
    }

    Token(std::string_view value) : Token(Token::LITERAL, value) {
        //std::cout << "creating LITERAL: " << value_ << std::endl;
    }

    Token(const char* value) : Token(Token::LITERAL, std::string_view(value)) {
        // std::cout << "creating LITERAL: " << value << std::endl;
    }

    ~Token() {
        // std:: cout << this->value_ << " destroyed" << std::endl;
    }

    Type type_;
    std::string value_;
};

// Overload operator<< for Token
std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << (token.type_ == Token::LITERAL ? "LITERAL" : "VAR") << ": " << token.value_;
    return os;
}

// Token operator"" _l(const char* value, std::size_t len) {
//     return Token(Token::Type::LITERAL, std::string_view(value, len));
// }

// Token operator"" _v(const char* value, std::size_t len) {
//     return Token(Token::Type::VAR, std::string_view(value, len));
// }

class str {
public:
    str(std::vector<Token> tokens) : tokens_(tokens) {
        //std::cout << "creating str: ";
        // for (const auto& token : tokens_) {
            //std::cout << token << " ";
        // }
        //std::cout << std::endl;
    }

    std::vector<Token> tokens_;
};

// Overload operator<< for str
std::ostream& operator<<(std::ostream& os, const str& s) {
    os << "str: [";
    for (const auto& token : s.tokens_) {
        os << token << ", ";
    }
    os << "]";
    return os;
}

using binding = std::pair<std::string_view, str>;
using map = std::vector<binding>;

class list {
public:
    list(std::vector<str> values) : values_(values) {
        //std::cout << "creating list: ";
        for (const auto& value : values_) {
            std::cout << value << " ";
        }
        //std::cout << std::endl;
    }

    std::vector<str> values_;
};

// Overload operator<< for list
std::ostream& operator<<(std::ostream& os, const list& l) {
    os << "list: [";
    for (const auto& value : l.values_) {
        os << value << ", ";
    }
    os << "]";
    return os;
}

class var {
public:
    var(const char* name, str value) : name_(std::string_view(name)), value_(value) {}

    std::string_view name_;
    str value_;
};

// Overload operator<< for var
std::ostream& operator<<(std::ostream& os, const var& v) {
    os << "var: " << v.name_ << ", " << v.value_;
    return os;
}


// class rule {
// public:
//     enum SPECIAL_RULE {
//         phony
//     };

//     union _rule {
//         _rule() : sp_rule(phony) {}
//         _rule(map bindings) : bindings_(bindings) {}
//         _rule(SPECIAL_RULE sr) : sp_rule(sr) {}
//         ~_rule() {}

//         map bindings_;
//         SPECIAL_RULE sp_rule;
//     } _rule_data;
//     bool is_special;

//     rule(map bindings) : _rule_data(bindings), is_special(false) {}
//     rule(SPECIAL_RULE sp_rule) : _rule_data(sp_rule), is_special(true) {}

//     rule(const rule& other) : is_special(other.is_special) {
//         if (is_special) {
//             new (&_rule_data) _rule(other._rule_data.sp_rule);  // Copy the special rule
//         } else {
//             new (&_rule_data) _rule(other._rule_data.bindings_);  // Copy the bindings map
//         }
//     }
// };

class rule {
public:
    enum RULE_TYPE {
        normal,
        phony
    };

    std::string_view name;
    RULE_TYPE type;
    map bindings_;

    rule(const char* name, map bindings) : name(std::string_view(name)), bindings_(bindings), type(normal) {}
    rule(RULE_TYPE type) : name("phony"), bindings_({}), type(phony) {}
};

// Overload operator<< for rule
std::ostream& operator<<(std::ostream& os, const rule& r) {
    os << "rule: [";
    if(r.type == rule::normal)
        os << r.type;
    else {
        for (const auto& binding : r.bindings_) {
            os << binding.first << ": " << binding.second << ", ";
        }
    }
    os << "]";
    return os;
}

class build {
public:
    build(
        list outputs,
        list implicit_outputs,
        rule &rule,
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
          bindings_(bindings) {
            
            // this->rule_ = rule;
        //std::cout << "creating build: ";
        //std::cout << "creating build outputs: ";
        //std::cout << this->outputs_ << std::endl;
    }

    // build(const build& other)
    //     : outputs_(other.outputs_),
    //       implicit_outputs_(other.implicit_outputs_),
    //     //   rule_(other.rule_),
    //       inputs_(other.inputs_),
    //       implicit_inputs_(other.implicit_inputs_),
    //       order_only_inputs_(other.order_only_inputs_),
    //       bindings_(other.bindings_) {
    //         this->rule_ = other.rule_;
    //       }

    list outputs_;
    list implicit_outputs_;
    rule &rule_;
    list inputs_;
    list implicit_inputs_;
    list order_only_inputs_;
    map bindings_;
};

// Overload operator<< for build
std::ostream& operator<<(std::ostream& os, const build& b) {
    os << "build: { "
       << "outputs: " << b.outputs_ << ", "
       << "implicit_outputs: " << b.implicit_outputs_ << ", "
       << "rule: " << b.rule_ << ", "
       << "inputs: " << b.inputs_ << ", "
       << "implicit_inputs: " << b.implicit_inputs_ << ", "
       << "order_only_inputs: " << b.order_only_inputs_ << ", "
       << "bindings: [";
    for (const auto& binding : b.bindings_) {
        os << binding.first << ": " << binding.second << ", ";
    }
    os << "] }";
    return os;
}

class buildGroup {
public:
    buildGroup(std::vector<build> builds) : builds(builds) {}

    std::vector<build> builds;
};

// Overload operator<< for buildGroup
std::ostream& operator<<(std::ostream& os, const buildGroup& bg) {
    os << "buildGroup: [";
    for (const auto& b : bg.builds) {
        os << b << ", ";
    }
    os << "]";
    return os;
}

// static auto in = "in"_v;
// static auto out = "out"_v;

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
