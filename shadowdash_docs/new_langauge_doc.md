# ShadowDash New Language Design Manifest Documentation

The `shadowdash_test/manifest.h` file defines the interface and internal logic for ShadowDash, a build configuration framework inspired by Ninja. This document is structured to first introduce **how to use the framework** and then dive into the **design logic**.

---

## How to Use ShadowDash with Newly Designed Language

The primary user-facing elements of ShadowDash are macros designed for defining rules, builds, pools, and defaults concisely. Below are the macros and how to use them, followed by a full example.

### 1. Key Macros

| Macro          | Description                                         | Example                                                                 |
|-----------------|-----------------------------------------------------|-------------------------------------------------------------------------|
| `let`          | Define a variable with a name and value.            | `let(my_var, "value"_l);`                                              |
| `bind`         | Create a key-value binding for a rule or build.     | `bind(command, "g++ -c in"_v);`                                        |
| `new_pool`     | Define a resource pool with a maximum concurrency.  | `new_pool(my_pool, 4);`                                                |
| `default`      | Set default build targets.                          | `default("all"_l, "test"_l);`                                          |
| `list`         | Define a list of strings or tokens.                 | `list("item1"_l, "item2"_l);`                                          |
| `rule`         | Define a rule with specific bindings.               | `rule(compile, bind(command, "g++ -c in"_v, "-o out"_v));`             |
| `str`          | Create a string composed of literals or variables.  | `str("hello.o"_l);`                                                    |
| `ShadowDash`   | Finalize the manifest with all builds and settings. | `ShadowDash({build1, build2}, {pool1}, default_targets);`              |

### 2. Full Example

shadowdash_test/manifest_example.cc is a complete example of using the macros in `manifest.h` to define a build configuration.

---

## New Language Design Logic

### 1. Internal Components

The new language design revolves around the following key classes:

#### Token
Represents a basic element in the build system, with types:
- **LITERAL**: A fixed string value.
- **VAR**: A variable that can be dynamically substituted.

#### String Abstraction (`str_`)
Allows combining `Token`s into complex strings, useful for commands and paths.

#### Lists and Bindings
- **`list_`**: A collection of `str_` instances.
- **`binding`**: Key-value pairs used in rules to define behavior.

#### Rules
The `rule__` class defines how to process inputs, with:
- **Regular rules**: Contain bindings for commands.
- **Special rules**: Predefined behaviors like `phony`.

#### Builds
The `build` class represents a single build operation, with inputs, outputs, and an associated rule.

#### Pools
The `pool` class defines resource limits for concurrent tasks.

#### Defaults
The `shadowdash_default` class sets the default targets.

#### ShadowDash Manifest
The `ShadowDash_` class aggregates all builds, pools, and default targets into a single manifest.

---

## Summary

The `shadowdash_test/manifest.h` file provides:
1. **User-friendly macros** for defining builds and rules.
2. **Robust internal logic** to manage complex dependencies.
3. **Extensibility** for diverse project requirements.

By focusing on the foundational design of ShadowDash’s new language, this module ensures seamless integration with other teams (Converter and Hello World Graph). 
For further details, refer to the example and test cases provided in the project repository.