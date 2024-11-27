#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

namespace shadowdash {
    inline int executeCommand(const std::string& command) {
        std::cout << "Executing: " << command << std::endl;
        return std::system(command.c_str());
    }
    inline bool file_exists(const std::string& filename) {
        struct stat buffer;
        return ::stat(filename.c_str(), &buffer) == 0;
    }

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

    class Pool {
    public:
        Pool(std::string name, int depth) 
            : name_(std::move(name)), depth_(depth), current_jobs_(0) {}
        
        Pool(const Pool& other)
            : name_(other.name_), depth_(other.depth_), current_jobs_(0) {}
        
        Pool& operator=(const Pool& other) {
            if (this != &other) {
                name_ = other.name_;
                depth_ = other.depth_;
                current_jobs_ = 0;
            }
            return *this;
        }

        bool canStartJob() const {
            return current_jobs_ < depth_;
        }

        void startJob() {
            if (!canStartJob()) {
                throw std::runtime_error("Pool " + name_ + " is full");
            }
            current_jobs_++;
        }

        void finishJob() {
            if (current_jobs_ > 0) {
                current_jobs_--;
            }
        }

    private:
        std::string name_;
        int depth_;
        std::atomic<int> current_jobs_;
    };

    class Rule {
    public:
        Rule(Command command) : command_(std::move(command)) {}

        Rule(Command command, std::string_view pool, int jobs = 1) 
        : command_(std::move(command)), pool_(pool), jobs_(jobs) {}

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
        // Constructor for initializer_list
        Build(std::string_view output,
              std::string_view rule,
              std::initializer_list<std::string_view> inputs = {},
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

        // Constructor for vectors
        Build(std::string_view output,
              std::string_view rule,
              const std::vector<std::string_view>& inputs,
              const std::vector<std::string_view>& implicit_inputs = {},
              const std::vector<std::string_view>& order_only_inputs = {},
              const std::vector<std::string_view>& implicit_outputs = {},
              bool is_phony = false)
                : output_(output), rule_(rule),
                  inputs_(inputs),
                  implicit_inputs_(implicit_inputs),
                  order_only_inputs_(order_only_inputs),
                  implicit_outputs_(implicit_outputs),
                  is_phony_(is_phony) {}

        bool needsRebuild() const {
            if (is_phony_) return true;
            
            struct stat output_stat;
            if (::stat(std::string(output_).c_str(), &output_stat) != 0) return true;

            for (const auto& input : inputs_) {
                struct stat input_stat;
                if (::stat(std::string(input).c_str(), &input_stat) != 0) {
                    return true;
                }
                if (input_stat.st_mtime > output_stat.st_mtime) {
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
        std::unordered_map<std::string, Pool> pools_;


        std::string resolveVariable(const std::string_view& var, const Build& build) const {
            try {
                if (var == "in") {
                    if (build.inputs_.empty()) {
                        throw std::runtime_error("No input files specified");
                    }
                    return std::string(build.inputs_[0]);  // For now, just use first input
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

            // Better command construction without extra spaces
            bool first = true;
            for (const auto& part : rule.command_.parts_) {
                if (!first) {
                    cmd << " ";  // Add single space between parts
                }
                first = false;

                if (part.type_ == Expression::Type::CONSTANT) {
                    cmd << part.content_;
                } else if (part.type_ == Expression::Type::VARIABLE) {
                    try {
                        std::string resolved = resolveVariable(part.content_, build);
                        if (!resolved.empty()) {
                            cmd << resolved;
                        }
                    } catch (const std::exception& e) {
                        throw std::runtime_error("Variable resolution failed for '" + 
                            std::string(part.content_) + "': " + e.what());
                    }
                }
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
                if (!file_exists(std::string(input))) {
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
                if (!file_exists(std::string(input))) {
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

        void definePool(std::string name, int depth) {
            pools_.emplace(name, Pool(name, depth));
        }

        void addDefault(std::string_view target) {
            defaults_.push_back(target);
        }

         void setBuildDir(std::string_view dir) {
            builddir_ = dir;
            if (!file_exists(std::string(dir))) {
                if (!createDirectory(std::string(dir))) {
                    throw std::runtime_error("Failed to create build directory: " + std::string(dir));
                }
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
            std::cout << "\nStarting build for target: " << build.output_ << std::endl;

            if (!build.needsRebuild()) {
                std::cout << "Skipping up-to-date target: " << build.output_ << "\n";
                return;
            }

            std::cout << (build.is_phony_ ? "Executing phony target: " : "Building: ") << build.output_ << "\n";

            // build all its dependencies first
            buildDependencies(build);

            for (const auto& dep : build.implicit_inputs_) {
                if (!file_exists(std::string(dep))) {
                    throw std::runtime_error("Missing dependency: " + std::string(dep));
                }
            }

            std::string command = constructCommand(build);
            std::cout << "Executing: " << command << "\n";

            // Get the rule and check for pool
            const auto& rule = rules_.at(build.rule_);
            Pool* pool = nullptr;
            if (rule.pool_) {
                auto pool_it = pools_.find(std::string(*rule.pool_));
                if (pool_it == pools_.end()) {
                    throw std::runtime_error("Unknown pool: " + std::string(*rule.pool_));
                }
                pool = const_cast<Pool*>(&pool_it->second);
                
                // Wait until we can acquire a slot in the pool
                while (!pool->canStartJob()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                pool->startJob();
            }

            try {
                if (int result = std::system(command.c_str()); result != 0) {
                    throw std::runtime_error("Command failed with code " +
                                            std::to_string(result) + ": " + command);
                }
            } catch (...) {
                if (pool) {
                    pool->finishJob();
                }
                throw;
            }

            if (pool) {
                pool->finishJob();
            }
            std::cout << "\n";
        }
    };
    inline bool createDirectory(const std::string& path) {
        #ifdef _WIN32
        return _mkdir(path.c_str()) == 0 || errno == EEXIST;
        #else
        return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
        #endif
    }

    static constexpr auto in = "in";
    static constexpr auto out = "out";

} // namespace shadowdash

#define RULE(name, ...) \
    shadowDash.defineRule(#name, shadowdash::Rule{ \
        shadowdash::Command{ __VA_ARGS__ } \
    })

#define RULE_WITH_POOL(name, cmd, pool, ...) \
    shadowDash.defineRule(#name, shadowdash::Rule{ \
        cmd, pool, ##__VA_ARGS__ \
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

#define POOL(name, depth) \
    shadowDash.definePool(name, depth)
