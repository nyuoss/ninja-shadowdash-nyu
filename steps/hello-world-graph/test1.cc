#include <memory>
#include "graph.h"
#include "state.h"
#include "util.h"

void CreateHelloWorldGraph(State* state) {
  // Create rules
  Rule* cxx_compiler = new Rule("CXX_COMPILER__hello_world_");
  cxx_compiler->AddBinding("command", "g++ -std=gnu++11 $in -o $out");
  cxx_compiler->AddBinding("description", "Building CXX object $out");
  cxx_compiler->AddBinding("depfile", "$DEP_FILE");
  cxx_compiler->AddBinding("deps", "gcc");
  state->bindings_.AddRule(cxx_compiler);  

  Rule* cxx_linker = new Rule("CXX_EXECUTABLE_LINKER__hello_world_");
  cxx_linker->AddBinding("command", "g++ $in -o $out");
  cxx_linker->AddBinding("description", "Linking CXX executable $out");
  state->bindings_.AddRule(cxx_linker);

  // Create nodes
  Node* source_file = state->GetNode("hello_world.cpp", 0);
  Node* object_file = state->GetNode("CMakeFiles/hello_world.dir/hello_world.cpp.o", 0);
  Node* executable = state->GetNode("hello_world", 0);

  // Create edges
  Edge* compile_edge = state->AddEdge(cxx_compiler);
  if (!state->AddOut(compile_edge, object_file->path(), 0)) {
    // Handle error
    fprintf(stderr, "Failed to add output to compile edge\n");
    return;
  }
  compile_edge->inputs_.push_back(source_file);
  
  Edge* link_edge = state->AddEdge(cxx_linker);
  if (!state->AddOut(link_edge, executable->path(), 0)) {
    // Handle error
    fprintf(stderr, "Failed to add output to link edge\n");
    return;
  }
  link_edge->inputs_.push_back(object_file);

  // Set up environment for compile edge
  if (compile_edge->env_ == nullptr) {
    compile_edge->env_ = new BindingEnv(&state->bindings_);
  }
  compile_edge->env_->AddBinding("DEP_FILE", "CMakeFiles/hello_world.dir/hello_world.cpp.o.d");

  // Add default target
  std::string error_message;
  if (!state->AddDefault(executable->path(), &error_message)) {
    // Handle error
    fprintf(stderr, "Failed to add default target: %s\n", error_message.c_str());
    return;
  }
}