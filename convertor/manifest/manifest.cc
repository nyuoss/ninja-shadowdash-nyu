#include "shadowdash_include/state.h"
#include "manifest.h"

extern State* ninja_state;

namespace shadowdash {

// ============================ Token Class ============================

Token::Token(Type type, std::string value)
    : type_(type), value_(std::move(value)) {}

Token::Token(std::string value) : Token(Token::LITERAL, std::move(value)) {}

Token::Token(const char* value) : Token(Token::LITERAL, std::string(value)) {}

// ============================ str Class ============================

str::str(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

// ============================ list Class ============================

list::list(std::vector<str> values) : values_(std::move(values)) {}

// ============================ var Class ============================

var::var(const char* name, str value) : name_(std::string(name)), value_(value) {}

// ============================ rule Class ============================

rule::rule(const char* name, map bindings) {
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

// ============================ build Class ============================

build::build(
    list outputs,
    list implicit_outputs,
    const char* rule,
    list inputs,
    list implicit_inputs,
    list order_only_inputs,
    map bindings)
    {
        const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
        if(!rule_ninja) return;
        Edge* edge = ninja_state->AddEdge(rule_ninja);
        std::string *err = new std::string();

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
        
        for(const shadowdash::str& str : outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }

        int implicit_output = 0;
        for(const shadowdash::str& str : implicit_outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ++implicit_output;
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }
        edge->implicit_outs_ = implicit_output;

        for(const shadowdash::str& str : inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }

        int implicit_input = 0;
        for(const shadowdash::str& str : implicit_inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ++implicit_input;
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }

        int order_only = 0;
        for(const shadowdash::str& str : order_only_inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ++order_only;
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }
        edge->implicit_deps_ = implicit_input;
        edge->order_only_deps_ = order_only;
    }

build::build(
    list outputs,
    const char* rule,
    list inputs)
    {
        const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
        if(!rule_ninja) return;
        Edge* edge = ninja_state->AddEdge(rule_ninja);
        std::string *err = new std::string();

        BindingEnv *env_ = &ninja_state->bindings_;
        BindingEnv* env = new BindingEnv(env_);
        edge->env_ = env;
        
        for(const shadowdash::str& str : outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }

        edge->implicit_outs_ = 0;

        for(const shadowdash::str& str : inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }
        edge->implicit_deps_ = 0;
        edge->order_only_deps_ = 0;
    }

build::build(
    list outputs,
    const char* rule,
    list inputs,
    list order_only_inputs,
    map bindings)
    {
        const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
        if(!rule_ninja) return;
        Edge* edge = ninja_state->AddEdge(rule_ninja);
        std::string *err = new std::string();

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
        
        for(const shadowdash::str& str : outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }

        edge->implicit_outs_ = 0;

        for(const shadowdash::str& str : inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }

        int order_only = 0;
        for(const shadowdash::str& str : order_only_inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ++order_only;
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }
        edge->implicit_deps_ = 0;
        edge->order_only_deps_ = order_only;
    }


build::build(
    list outputs,
    const char* rule,
    list inputs,
    map bindings)
    {
        const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
        if(!rule_ninja) return;
        Edge* edge = ninja_state->AddEdge(rule_ninja);
        std::string *err = new std::string();

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
        
        for(const shadowdash::str& str : outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }

        edge->implicit_outs_ = 0;

        for(const shadowdash::str& str : inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }

        edge->implicit_deps_ = 0;
        edge->order_only_deps_ = 0;
    }

build::build(
    list outputs,
    const char* rule,
    map bindings)
    {
        const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
        if(!rule_ninja) return;
        Edge* edge = ninja_state->AddEdge(rule_ninja);
        std::string *err = new std::string();

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
        
        for(const shadowdash::str& str : outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }

        edge->implicit_outs_ = 0;
        edge->implicit_deps_ = 0;
        edge->order_only_deps_ = 0;
    }


build::build(
    list outputs,
    list implicit_outputs,
    const char* rule,
    list order_only_inputs,
    map bindings)
    {
        const Rule* rule_ninja = ninja_state->bindings_.LookupRule(rule);
        if(!rule_ninja) return;
        Edge* edge = ninja_state->AddEdge(rule_ninja);
        std::string *err = new std::string();

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
        
        for(const shadowdash::str& str : outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }

        int implicit_output = 0;
        for(const shadowdash::str& str : implicit_outputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ++implicit_output;
            ninja_state->AddOut(edge, value->Evaluate(edge->env_), 0, err);
        }
        edge->implicit_outs_ = implicit_output;

        int order_only = 0;
        for(const shadowdash::str& str : order_only_inputs.values_) {
            EvalString *value = new EvalString;
            for (const shadowdash::Token& token: str.tokens_) {
                if(token.type_ == token.LITERAL)
                    value->AddText(std::move(token.value_));
                else if(token.type_ == token.VAR)
                    value->AddSpecial(std::move(token.value_));
            }
            ++order_only;
            ninja_state->AddIn(edge, value->Evaluate(edge->env_), 0);
        }
        edge->implicit_deps_ = 0;
        edge->order_only_deps_ = order_only;
    }

} // namespace shadowdash