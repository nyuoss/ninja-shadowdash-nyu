# Example Code

```ninja
cflags = -Wall

rule cc
  command = gcc $cflags -c $in -o $out

build foo.o: cc foo.c
```

## VARIABLES

- `cflags = -g`: This variable can be used on the right side of an equals sign, dereferencing it with a dollar sign, as shown below:

```ninja
rule cc
  command = gcc $cflags -c $in -o $out
```

## RULES

- Rules start with the `rule` keyword, followed by a name for the rule:

```ninja
rule
  variable = value
```

## BUILD STATEMENT

- Describes the relationship between input and output files.

```ninja
cflags = -Wall -Werror
rule cc
  command = gcc $cflags -c $in -o $out

# If left unspecified, builds use the outer $cflags.
build foo.o: cc foo.c

# You can shadow variables like cflags for a particular build.
build special.o: cc special.c
  cflags = -Wall

# The variable is only shadowed for the scope of special.o;
# subsequent build lines use the outer (original) cflags.
build bar.o: cc bar.c
```

## THE PHONY RULES

- Phony rules can create aliases for other targets:

```ninja
build foo: phony some/file/in/a/faraway/subdir/foo
```

- To create a rule that never rebuilds, use a build rule without any input:

```ninja
rule touch
  command = touch $out
build file_that_always_exists.dummy: touch
build dummy_target_to_follow_a_pattern: phony file_that_always_exists.dummy
```

## DEFAULT TARGET STATEMENTS

- If no targets are specified on the command line, Ninja will build every output that is not named as an input elsewhere:

```ninja
default foo bar
default baz
```

## NOTES

Things we can see:

- `tokens.py`: Define the token types and token class
- `lexer.py`: Split source into tokens
- `parser.py`: Convert tokens into AST
- `ast.py`: Define AST node types
- `semantic_analyzer.py`: Type checking & semantic validation
- `symbol_table.py`: Track identifiers and their properties
- `transformer.py`: AST transformations/optimizations  
- `code_generator.py`: Generate target code from AST
- `patterns.py`: Common patterns/utilities
- `main.py`: Orchestrate the compilation pipeline

## To Run

```bash
./build/ninja_converter input.ninja output.cpp
```