#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <stdexcept>

namespace shadowdash {

    class Expression {
    public:
        enum class Type { CONSTANT, VARIABLE, PATH };

        constexpr Expression(Type type, std::string_view content)
                : type_(type), content_(content) {}

        Type type_;
        std::string_view content_;
    };

    constexpr Expression constant(const char* value, std::size_t len) {
        return Expression(Expression::Type::CONSTANT, {value, len});
    }

    constexpr Expression variable(const char* value, std::size_t len) {
        return Expression(Expression::Type::VARIABLE, {value, len});
    }

    class Command {
    public:
        Command(std::initializer_list<Expression> parts)
                : parts_(std::vector<Expression>(parts)) {}
        std::vector<Expression> parts_;
    };

    class Rule {
    public:
        Rule(Command command) : command_(std::move(command)) {}

        Command command_;
        std::optional<std::string_view> description_;
        std::optional<std::string_view> depfile_;
        std::optional<std::string_view> deps_;
        std::optional<bool> generator_{false};
        std::optional<bool> restat_{false};
        std::optional<std::string_view> rspfile_;
        std::optional<std::string_view> rspfile_content_;
        std::optional<std::string_view> pool_;
        std::optional<int> jobs_{1};

        bool hasGenerator() const { return generator_ && *generator_; }
        bool needsRestat() const { return restat_ && *restat_; }
    };

    class Build {
    public:
        Build(std::string_view output,
              std::string_view rule,
              std::initializer_list<std::string_view> inputs,
              std::initializer_list<std::string_view> implicit_inputs = {},
              std::initializer_list<std::string_view> order_only_inputs = {},
              std::initializer_list<std::string_view> implicit_outputs = {},
              bool is_phony = false)
                : output_(output), rule_(rule),
                  inputs_(inputs.begin(), inputs.end()),
                  implicit_inputs_(implicit_inputs.begin(), implicit_inputs.end()),
                  order_only_inputs_(order_only_inputs.begin(), order_only_inputs.end()),
                  implicit_outputs_(implicit_outputs.begin(), implicit_outputs.end()),
                  is_phony_(is_phony) {}

        bool needsRebuild() const {
            if (is_phony_) return true;
            if (!std::filesystem::exists(std::string(output_))) return true;

            auto output_time = std::filesystem::last_write_time(std::string(output_));

            for (const auto& input : inputs_) {
                if (!std::filesystem::exists(std::string(input))) {
                  return true;
                }
                if (std::filesystem::last_write_time(std::string(input)) > output_time) {
                    return true;
                }
            }
            return false;
        }

        bool is_phony_;
        std::string_view output_;
        std::string_view rule_;
        std::vector<std::string_view> inputs_;
        std::vector<std::string_view> implicit_inputs_;
        std::vector<std::string_view> order_only_inputs_;
        std::vector<std::string_view> implicit_outputs_;
        std::unordered_map<std::string_view, std::string_view> variables_;
    };

    class ShadowDash {
    private:
        std::unordered_map<std::string_view, Rule> rules_;
        std::vector<Build> builds_;
        std::unordered_map<std::string_view, std::string_view> variables_;
        std::vector<std::string_view> defaults_;
        std::optional<std::string_view> builddir_;

        std::string resolveVariable(const std::string_view& var, const Build& build) const {
            try {
                if (var == "in") {
                    if (build.inputs_.empty()) {
                        throw std::runtime_error("No input files specified");
                    }
                    std::string result;
                    for (const auto& input : build.inputs_) {
                        if (!result.empty()) result += " ";
                        result += std::string(input);
                    }
                    return result;
                } else if (var == "out") {
                    return std::string(build.output_);
                } else {
                    auto it = variables_.find(var);
                    if (it != variables_.end()) {
                        return std::string(it->second);
                    }
                    throw std::runtime_error("Undefined variable: " + std::string(var));
                }
            } catch (const std::exception& e) {
                throw std::runtime_error("Variable resolution failed: " + std::string(var));
            }
        }

        std::string constructCommand(const Build& build) const {
            if (rules_.find(build.rule_) == rules_.end()) {
                throw std::runtime_error("Unknown rule: " + std::string(build.rule_));
            }

            std::stringstream cmd;
            const auto& rule = rules_.at(build.rule_);

            for (const auto& part : rule.command_.parts_) {
                if (part.type_ == Expression::Type::CONSTANT) {
                    cmd << part.content_;
                } else {
                    cmd << resolveVariable(part.content_, build);
                }
                cmd << " ";
            }
            return cmd.str();
        }

        // finds a build given the output name
        const Build* findBuildByOutput(std::string_view output) const {
          for (const auto& build : builds_) {
            if (build.output_ == output) {
              return &build;
            }
          }
          return nullptr;
        }

        void buildDependencies(const Build& build) const {
          for (const auto& input : build.inputs_) {
            std::cout << "Building dependency " << input << std::endl;
            if (!std::filesystem::exists(std::string(input))) {
                // try to find a build that produces this input, and execute the build
                if (const Build* dep_build = findBuildByOutput(input)) {
                    executeSingleBuild(*dep_build);
                } else {
                    throw std::runtime_error("Input file missing and no rule to build it: " + std::string(input));
                }
            }
        }

          // do same thing for implicit inputs
          for (const auto& input : build.implicit_inputs_) {
            if (!std::filesystem::exists(std::string(input))) {
                // try to find a build that produces this input, and execute the build
                if (const Build* dep_build = findBuildByOutput(input)) {
                    executeSingleBuild(*dep_build);
                } else {
                    throw std::runtime_error("Implicit input file missing and no rule to build it: " + std::string(input));
                }
            }
          }
        }

    public:
        void defineRule(std::string_view name, Rule rule) {
            rules_.emplace(name, std::move(rule));
        }

        void defineBuild(Build build) {
            builds_.push_back(std::move(build));
        }

        void defineVariable(std::string_view name, std::string_view value) {
            variables_[name] = value;
        }

        void addDefault(std::string_view target) {
            defaults_.push_back(target);
        }

        void setBuildDir(std::string_view dir) {
            builddir_ = dir;
            if (!std::filesystem::exists(std::string(dir))) {
                std::filesystem::create_directories(std::string(dir));
            }
        }

        void executeBuild() const {
            try {
                // build everything if no defaults
                if (defaults_.empty()) {
                    for (const auto& build : builds_) {
                        executeSingleBuild(build);
                    }
                } else {
                    // build only defaults
                    for (const auto& default_target : defaults_) {
                        bool found = false;
                        for (const auto& build : builds_) {
                            if (build.output_ == default_target) {
                                executeSingleBuild(build);
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            throw std::runtime_error("Default target not found: " + std::string(default_target));
                        }
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Build failed: " << e.what() << "\n";
                throw;
            }
        }

        void executeSingleBuild(const Build& build) const {
            if (!build.needsRebuild()) {
                std::cout << "Skipping up-to-date target: " << build.output_ << "\n";
                return;
            }

            if (build.is_phony_) {
                std::cout << "Executing phony target: " << build.output_ << "\n";
                return;
            }

            std::cout << "Building: " << build.output_ << "\n";

            // build all its dependencies first
            buildDependencies(build);

            for (const auto& dep : build.implicit_inputs_) {
                if (!std::filesystem::exists(std::string(dep))) {
                    throw std::runtime_error("Missing dependency: " + std::string(dep));
                }
            }

            std::string command = constructCommand(build);
            std::cout << "Executing: " << command << "\n";

            if (int result = std::system(command.c_str()); result != 0) {
                throw std::runtime_error("Command failed with code " +
                                        std::to_string(result) + ": " + command);
            }
            std::cout << "\n";
        }

    };

    static constexpr auto in = "in";
    static constexpr auto out = "out";

} // namespace shadowdash

#define RULE(name, ...) \
    shadowDash.defineRule(#name, shadowdash::Rule{ \
        shadowdash::Command{ __VA_ARGS__ } \
    })

#define BUILD(...) \
    shadowDash.defineBuild(shadowdash::Build{ __VA_ARGS__ })

#define VAR(name, value) \
    shadowDash.defineVariable(#name, value)

#define DEFAULT(target) \
    shadowDash.addDefault(target)

#define BUILDDIR(dir) \
    shadowDash.setBuildDir(dir)

#define PHONY(name, rule, ...) \
    shadowDash.defineBuild(shadowdash::Build{#name, rule, {__VA_ARGS__}, {}, {}, {}, true})
