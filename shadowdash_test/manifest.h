#include "shadowdash_include/state.h"
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
        : type_(type), value_(std::move(value)) {}

    Token(std::string value) : Token(Token::LITERAL, std::move(value)) {}

    Token(const char* value) : Token(Token::LITERAL, std::string(value)) {}

    Type type_;
    std::string value_;
};

class str {
public:
    str(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

    std::vector<Token> tokens_;
};

using binding = std::pair<std::string, str>;
using map = std::vector<binding>;

class list {
public:
    list(std::vector<str> values) : values_(std::move(values)) {}

    std::vector<str> values_;
};

class var {
public:
    var(const char* name, str value) : name_(std::string(name)), value_(value) {}

    std::string name_;
    str value_;
};

class rule {
public:
    rule(const char* name, map bindings) {
        if(name == "phony") return;
        Rule* rule_ninja = new Rule(std::string(name));
        for (const auto& binding : bindings) {
            EvalString *value = new EvalString;
            for(shadowdash::Token token : binding.second.tokens_)
            {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            rule_ninja->AddBinding(std::move(binding.first), *value);
        }
        ninja_state->bindings_.AddRule(rule_ninja);
    }
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
        map bindings){
            const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
            if(!rule_ninja) return;
            Edge* edge = ninja_state->AddEdge(rule_ninja);
            std::string *err = new std::string();
            
            for(const shadowdash::str& str : outputs.values_) {
                std::string output;
                for (const shadowdash::Token& token: str.tokens_) {
                    output.append(std::move(token.value_));
                }
                ninja_state->AddOut(edge, std::move(output), 0, err);
            }

            int implicit_output = 0;
            for(const shadowdash::str& str : implicit_outputs.values_) {
                std::string output;
                for (const shadowdash::Token& token: str.tokens_) {
                    output.append(std::move(token.value_));
                }
                ++implicit_output;
                ninja_state->AddOut(edge, std::move(output), 0, err);
            }
            edge->implicit_outs_ = implicit_output;

            for(const shadowdash::str& str : inputs.values_) {
                std::string input;
                for (const shadowdash::Token& token: str.tokens_) {
                    input.append(std::move(token.value_));
                }
                ninja_state->AddIn(edge, std::move(input), 0);
            }

            int implicit_input = 0;
            for(const shadowdash::str& str : implicit_inputs.values_) {
                std::string input;
                for (const shadowdash::Token& token: str.tokens_) {
                    input.append(std::move(token.value_));
                }
                ++implicit_input;
                ninja_state->AddIn(edge, std::move(input), 0);
            }

            int order_only = 0;
            for(const shadowdash::str& str : order_only_inputs.values_) {
                std::string input;
                for (const shadowdash::Token& token: str.tokens_) {
                    input.append(std::move(token.value_));
                }
                ++order_only;
                ninja_state->AddIn(edge, std::move(input), 0);
            }
            edge->implicit_deps_ = implicit_input;
            edge->order_only_deps_ = order_only;

            BindingEnv *env_ = &ninja_state->bindings_;
            BindingEnv* env = new BindingEnv(env_);
            for (const auto& binding : bindings) {
                EvalString *value = new EvalString;
                for(shadowdash::Token token : binding.second.tokens_)
                {
                    if(token.type_ == token.LITERAL)
                        value->AddText(std::move(token.value_));
                    else if(token.type_ == token.VAR)
                        value->AddSpecial(std::move(token.value_));
                }
                env->AddBinding(std::move(binding.first), value->Evaluate(env_));
            }
            edge->env_ = env;
    }
};

Token operator"" _l(const char* value, std::size_t len) {
    return Token(Token::Type::LITERAL, std::string(value, len));
}

Token operator"" _v(const char* value, std::size_t len) {
    return Token(Token::Type::VAR, std::string(value, len));
}

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
