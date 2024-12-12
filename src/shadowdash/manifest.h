#pragma once

#include <initializer_list>
#include <string_view>
#include <string>
#include <vector>

namespace shadowdash {

// The atom "string" object in the new language with type identifying whether  
// a string object is a "Literal" or a "Variable"
class Token {
public:
  enum Type {LITERAL, VAR}; 
  // Token type: LITERAL (constant) or VAR (variable).

  // Constructor for a Token with a specified type and value.
  constexpr Token(Type type, std::string_view value) : 
    type_(type), value_(value) {}

  // Constructor for a Token defaulting to LITERAL type.
  constexpr Token(std::string_view value) : Token(Token::LITERAL, value) {}

  // Constructor for a Token from a C-style string, defaulting to LITERAL.
  constexpr Token(const char* value) : Token(Token::LITERAL, value) {}

  Type type_;                  // The type of the token.
  std::string_view value_;     // The value of the token (string content).
};

// User-defined literal to create a LITERAL token.
constexpr Token operator"" _l(const char* value, std::size_t len) {
  return Token(Token::Type::LITERAL, { value, len });
}

// User-defined literal to create a VAR token.
constexpr Token operator"" _v(const char* value, std::size_t len) {
  return Token(Token::Type::VAR, { value, len });
}

// Represents a string as a collection of tokens.
class str {
public:
  // Constructor accepting an initializer list of tokens.
  str(std::initializer_list<Token> tokens) : tokens_(tokens) {}

  // Returns the number of tokens in the string.
  constexpr size_t size() const { return tokens_.size(); }

  // Access a specific token by index.
  constexpr const Token& operator[](size_t i) const 
  { return *(tokens_.begin() + i); }

  std::initializer_list<Token> tokens_; 
  // The list of tokens forming the string.
};

// Alias for a name-value binding, where the value is a `str`.
// typically used when passing in arguments for rules, builds, etc.
// example: depth = 4 -> binding("depth", {"4"}) in binding language
using binding = std::pair<std::string, str>;

// map: alias for an initializaiton_list of bindings
// represents a list of all the arguments
using map = std::initializer_list<binding>;

// Represents a list of `str` objects.
class list {
public:
  // Constructor accepting an initializer list of `str` objects.
  list(std::initializer_list<str> values) : values_(values) {}

  // Returns the size of the list.
  constexpr size_t size() const { return values_.size(); }

  // Access a specific `str` by index.
  constexpr const str& operator[](size_t i) const 
  { return *(values_.begin() + i); }

  std::initializer_list<str> values_; // The list of strings.
};

// Ninja variables with attributes of name and value
class var {
public:
  const char* name_;
  str value_;

  var(const char* name, str value) : name_(name), value_(value) {}
};

// Ninja rule object
class rule {
public:
  // Constructor with a name and bindings.
  constexpr rule(std::string_view name, map bindings) : 
    name(name), bindings_(bindings) {}

  // Constructor with only a name.
  constexpr rule(std::string_view name) : name(name) {}

  // Get the name of the rule.
  constexpr std::string_view get_rule_name() const { return name; }

  // Returns the number of bindings in the rule.
  constexpr size_t size() const { return bindings_.size(); }

  // Access a specific binding by index.
  constexpr const binding& operator[](size_t i) const 
    { return *(bindings_.begin() + i); }

  std::string_view name; // Name of the rule.
  map bindings_;
  // 
  /*
  bindings_: {
    (type binding, {op1, {args}}),
    (type binding, {op2, {args}}),
    ...
  }
  records operations that will be executed within this rule
  as well as arguments required for each operation in list 
  of tokens
  */
  // 
};

// Exception class for invalid pool arguments.
class PoolException : public std::exception {
public:
  // Error message for invalid pool arguments.
  const char* what() const noexcept override {
    return "Pool has argument that is not depth!";
  }
};

// Represents a pool with a depth binding.
class pool_ {
public:
  // Constructor that validates the binding is for "depth".
  pool_(binding depth) : depth_(depth) {
    if (depth.first != "depth") throw PoolException();
  // throw an excpetion when pool_ objects received arguments other than depth
  }

  // Get the depth binding.
  constexpr const binding& get_depth() const { return depth_; }

  binding depth_; // The depth argument of pool.
};

// Represents a build configuration with inputs, outputs, and rules.
class build {
public:
  build(
      list outputs,              // List of output files.
      list implicit_outputs,     // Implicit output files.
      rule& rule,                // Associated build rule.
      list inputs,               // List of input files.
      list implicit_inputs,      // Implicit input files.
      list order_only_inputs,    // Order-only dependencies.
      map bindings               // Additional arguments.
  ) : outputs_(outputs),
      implicit_outputs_(implicit_outputs),
      rule_(rule),
      inputs_(inputs),
      implicit_inputs_(implicit_inputs),
      order_only_inputs_(order_only_inputs),
      bindings_(bindings) {}

  list outputs_;              // Outputs of the build.
  list implicit_outputs_;     // Implicit outputs.
  rule& rule_;                // Associated rule.
  list inputs_;               // Inputs of the build.
  list implicit_inputs_;      // Implicit inputs.
  list order_only_inputs_;    // Order-only dependencies.
  map bindings_;              // Additional arguments.
};

// Represents a collection of builds returned by a manifest.
struct BuildsReturn {
  // Constructor accepting a list of builds.
  BuildsReturn(std::initializer_list<build> builds) : builds(builds) {}

  // Returns the number of builds.
  constexpr size_t size() const { return builds.size(); }

  // Access a specific build by index.
  constexpr const build& operator[](size_t i) const 
  { return *(builds.begin() + i); }

  std::initializer_list<build> builds; // List of builds.
};

// Represents default build targets.
class default_ {
public:
  // Add a list of targets to the defaults.
  default_(list addedList) {
    for (size_t i = 0; i < addedList.size(); i++) 
      targets_.push_back(addedList[i]);
  }

  // Add a single target to the defaults.
  default_(str addedTarget) {
    targets_.push_back(addedTarget);
  }

  static std::vector<str> targets_; // List of default targets.
};

std::vector<str> default_::targets_; // Definition of static targets list.

// Predefined constants for common variables.
static constexpr auto in = "in"_v;        // Input variable, used in rules
static constexpr auto out = "out"_v;      // Output variable, used in rules
static auto phony = rule{{}};             // Default phony rule.
static auto console = pool_(binding("depth", str{ {"1"} })); 
// Default console pool.

} // namespace shadowdash

// Macro to define a variable with a name and value(s).
#define let(name, ...) \
  var name {           \
    #name, str {       \
      __VA_ARGS__      \
    }                  \
  }

// Macro to create a binding with a name and value(s).
#define bind(name, ...) \
  {                     \
    #name, str {        \
      __VA_ARGS__       \
    }                   \
  }

// Macro to define a rule with a name and bindings.
#define make_rule(name, ...) \
rule name {                  \
  #name, {__VA_ARGS__}       \
}