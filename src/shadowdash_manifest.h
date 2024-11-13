#include "ninja.h"
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>

extern State* ninja_state;

namespace shadowdash {

class Token {
public:
    enum Type { LITERAL, VAR };

    Token(Type type, std::string value)
        : type_(type), value_(value) {
        //std::cout << "creating " << type << " : " << value_ << std::endl;
    }

    Token(std::string value) : Token(Token::LITERAL, value) {
        //std::cout << "creating LITERAL: " << value_ << std::endl;
    }

    Token(const char* value) : Token(Token::LITERAL, std::string(value)) {
        // std::cout << "creating LITERAL: " << value << std::endl;
    }

    ~Token() {
        // std:: cout << this->value_ << " destroyed" << std::endl;
    }

    Type type_;
    std::string value_;
};

// Overload operator<< for Token
// std::ostream& operator<<(std::ostream& os, const Token& token) {
//     os << (token.type_ == Token::LITERAL ? "LITERAL" : "VAR") << ": " << token.value_;
//     return os;
// }

// Token operator"" _l(const char* value, std::size_t len) {
//     return Token(Token::Type::LITERAL, std::string(value, len));
// }

// Token operator"" _v(const char* value, std::size_t len) {
//     return Token(Token::Type::VAR, std::string(value, len));
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
// std::ostream& operator<<(std::ostream& os, const str& s) {
//     os << "str: [";
//     for (const auto& token : s.tokens_) {
//         os << token << ", ";
//     }
//     os << "]";
//     return os;
// }

using binding = std::pair<std::string, str>;
using map = std::vector<binding>;

class list {
public:
    list(std::vector<str> values) : values_(values) {
        //std::cout << "creating list: ";
        // for (const auto& value : values_) {
        //     std::cout << value << " ";
        // }
        //std::cout << std::endl;
    }

    std::vector<str> values_;
};

// Overload operator<< for list
// std::ostream& operator<<(std::ostream& os, const list& l) {
//     os << "list: [";
//     for (const auto& value : l.values_) {
//         os << value << ", ";
//     }
//     os << "]";
//     return os;
// }

class var {
public:
    var(const char* name, str value) : name_(std::string(name)), value_(value) {}

    std::string name_;
    str value_;
};

// Overload operator<< for var
// std::ostream& operator<<(std::ostream& os, const var& v) {
//     os << "var: " << v.name_ << ", " << v.value_;
//     return os;
// }


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

    std::string name;
    RULE_TYPE type;
    map bindings_;

    rule(const char* name, map bindings) : name(std::string(name)), bindings_(bindings), type(normal) {
        std::cout << "ninja state address: " << &ninja_state << std::endl;
        Rule* rule_ninja = new Rule(std::string(name));
        for (const auto& binding : this->bindings_) {
            EvalString *value = new EvalString;
            for(shadowdash::Token token : binding.second.tokens_)
            {
                if(token.type_ == token.LITERAL)
                    value->AddText(token.value_);
                else if(token.type_ == token.VAR)
                    value->AddSpecial(token.value_);
            }
            rule_ninja->AddBinding(std::string(binding.first), *value);
        }
        ninja_state->bindings_.AddRule(rule_ninja);
    }
    rule(RULE_TYPE type) : name("phony"), bindings_({}), type(phony) {
        // do nothing since ninja_state has phony rule by default
    }
};

// Overload operator<< for rule
// std::ostream& operator<<(std::ostream& os, const rule& r) {
//     os << "rule: [";
//     if(r.type == rule::normal)
//         os << r.type;
//     else {
//         for (const auto& binding : r.bindings_) {
//             os << binding.first << ": " << binding.second << ", ";
//         }
//     }
//     os << "]";
//     return os;
// }

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
            const Rule* rule_ninja = ninja_state->bindings_.LookupRule(std::string(this->rule_.name));
            if(!rule_ninja) return;
            Edge* edge = ninja_state->AddEdge(rule_ninja);
            string *err = new string();
            
            for(const shadowdash::str& str : this->outputs_.values_) {
                string output;
                for (const shadowdash::Token& token: str.tokens_) {
                    output.append(token.value_);
                }
                ninja_state->AddOut(edge, output, 0, err);
            }

            int implicit_output = 0;
            for(const shadowdash::str& str : this->implicit_outputs_.values_) {
                string output;
                for (const shadowdash::Token& token: str.tokens_) {
                    output.append(token.value_);
                }
                ++implicit_output;
                ninja_state->AddOut(edge, output, 0, err);
            }
            edge->implicit_outs_ = implicit_output;

            for(const shadowdash::str& str : this->inputs_.values_) {
                string input;
                for (const shadowdash::Token& token: str.tokens_) {
                    input.append(token.value_);
                }
                ninja_state->AddIn(edge, input, 0);
            }

            int implicit_input = 0;
            for(const shadowdash::str& str : this->implicit_inputs_.values_) {
                string input;
                for (const shadowdash::Token& token: str.tokens_) {
                    input.append(token.value_);
                }
                ++implicit_input;
                ninja_state->AddIn(edge, input, 0);
            }

            int order_only = 0;
            for(const shadowdash::str& str : this->order_only_inputs_.values_) {
                string input;
                for (const shadowdash::Token& token: str.tokens_) {
                    input.append(token.value_);
                }
                ++order_only;
                ninja_state->AddIn(edge, input, 0);
            }
            edge->implicit_deps_ = implicit_input;
            edge->order_only_deps_ = order_only;

            BindingEnv *env_ = &ninja_state->bindings_;
            BindingEnv* env = new BindingEnv(env_);
            for (const auto& binding : this->bindings_) {
                EvalString *value = new EvalString;
                bool first_iteration = true;
                for(shadowdash::Token token : binding.second.tokens_)
                {
                    if(token.type_ == token.LITERAL)
                        value->AddText(token.value_);
                    else if(token.type_ == token.VAR)
                        value->AddSpecial(token.value_);
                    first_iteration = false;
                }
                env->AddBinding(std::string(binding.first), value->Evaluate(env_));
            }
            edge->env_ = env;
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
// std::ostream& operator<<(std::ostream& os, const build& b) {
//     os << "build: { "
//        << "outputs: " << b.outputs_ << ", "
//        << "implicit_outputs: " << b.implicit_outputs_ << ", "
//        << "rule: " << b.rule_ << ", "
//        << "inputs: " << b.inputs_ << ", "
//        << "implicit_inputs: " << b.implicit_inputs_ << ", "
//        << "order_only_inputs: " << b.order_only_inputs_ << ", "
//        << "bindings: [";
//     for (const auto& binding : b.bindings_) {
//         os << binding.first << ": " << binding.second << ", ";
//     }
//     os << "] }";
//     return os;
// }

class buildGroup {
public:
    buildGroup(std::vector<build> builds) : builds(builds) {}

    std::vector<build> builds;
};

// Overload operator<< for buildGroup
// std::ostream& operator<<(std::ostream& os, const buildGroup& bg) {
//     os << "buildGroup: [";
//     for (const auto& b : bg.builds) {
//         os << b << ", ";
//     }
//     os << "]";
//     return os;
// }

Token operator"" _l(const char* value, std::size_t len) {
    return Token(Token::Type::LITERAL, std::string(value, len));
}

Token operator"" _v(const char* value, std::size_t len) {
    return Token(Token::Type::VAR, std::string(value, len));
}

static rule phony = rule::phony;

static auto in = "in"_v;
static auto out = "out"_v;

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
