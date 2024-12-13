# Hello World Graph Documentation

This document provides an overview of the updates to the **Hello World Graph** task in the Shadowdash project, including its structure, new content, and instructions on how to run it.

---

## **Overview**

The `Hello World Graph` demonstrates a basic usage of Ninja's build system by introducing a simple graph structure that compiles and links a "Hello, World!" program. The primary updates in this task include:

1. **Build Graph Definition**:
    - A minimal build graph is defined using two custom rules: `compile` and `link`.
    - Each rule is implemented as part of the Ninja state with inputs and outputs linked together.

2. **Rules and Edges**:
    - **Compile Rule**: Uses `g++` to compile `hello.cc` into `hello.o`.
    - **Link Rule**: Links `hello.o` to produce the executable `hello`.

3. **Integration**:
    - The graph integrates seamlessly into the Ninja-based build system, leveraging Ninja's structure and performance.

---

## **Key Components**

### 1. **Rules**

- **Compile Rule**: Defines how the source file (`hello.cc`) is compiled:
  ```plaintext
  g++ -c hello.cc -o hello.o

- **Link Rule**: Defines how the object file (hello.o) is linked to produce the executable:
  ```plaintext
  g++ hello.o -o hello

### 2. **Edges**

The edges connect the rules, defining the dependencies:
- `hello.cc` → [Compile Rule] → `hello.o`
- `hello.o` → [Link Rule] → `hello`

---

## **How to Run**

1. **Prerequisites**:
    - Ensure you have `g++` installed on your system.
    - Ninja is required to run the build graph.

2. **Steps to Build and Run**:
    - Navigate to the project root directory:
      ```bash
      cd shadowdash
      ```
    - Run the `hello` target using the Ninja build system:
      ```bash
      ./build-cmake/ninja hello
      ```
    - Upon successful execution, you will see the compiled `hello` executable. Run it to verify the output:
      ```bash
      ./hello
      ```
    - Expected Output:
      ```plaintext
      Hello, World!
      ```

---

## **File Structure**

- `hello_world_graph/hello.cc`: The "Hello, World!" source file.
- `build-cmake/ninja`: The Ninja executable for building the project.
- Custom rules and graph logic are embedded in the build system to demonstrate the flow.
