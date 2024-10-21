#include <iostream>
#include <memory>

#include "graph.h"
#include "state.h"
#include "util.h"
using namespace std;

void CreateHelloWorldGraph(State* state) {
  BindingEnv env;
  env.AddBinding("in", "hello_world.cpp");
  env.AddBinding("out", "hello_world");
  env.AddBinding("DEP_FILE", "hello_world.d");

  // Create rules
  Rule* cxx_compiler = new Rule("CXX_COMPILER__hello_world_");

  // Create command
  EvalString command;
  command.AddText("g++ -std=gnu++11 ");
  command.AddSpecial("$in");
  command.AddText(" -o ");
  command.AddSpecial("$out");
  cxx_compiler->AddBinding("command", command);

  // Create description
  EvalString description;
  description.AddText("Building CXX object ");
  description.AddSpecial("$out");
  cxx_compiler->AddBinding("description", description);

  // Add dependency file
  EvalString depfile;
  depfile.AddSpecial("$DEP_FILE");
  cxx_compiler->AddBinding("depfile", depfile);

  // Add dependency format
  EvalString deps;
  deps.AddText("gcc");
  cxx_compiler->AddBinding("deps", deps);

  // Add rule to state
  state->bindings_.AddRule(cxx_compiler);

  Rule* cxx_linker = new Rule("CXX_EXECUTABLE_LINKER__hello_world_");
  EvalString link;
  link.AddText("g++ ");
  link.AddSpecial("$in");
  link.AddText(" -o ");
  link.AddSpecial("$out");
  cxx_linker->AddBinding("command", link);
  EvalString linkDescription;
  description.AddText("Linking CXX object ");
  description.AddSpecial("$out");
  cxx_linker->AddBinding("description", linkDescription);
  state->bindings_.AddRule(cxx_linker);

  // Create nodes
  Node* source_file = state->GetNode("hello_world.cpp", 0);
  Node* object_file =
      state->GetNode("CMakeFiles/hello_world.dir/hello_world.cpp.o", 0);
  Node* executable = state->GetNode("hello_world", 0);

  // Create edges
  string error_message = "";
  string* err_ptr = &error_message;
  Edge* compile_edge = state->AddEdge(cxx_compiler);
  if (!state->AddOut(compile_edge, object_file->path(), 0, err_ptr)) {
    // Handle error
    *err_ptr = "Failed to add output to compile edge\n";
    Error(error_message.c_str());
    return;
  }
  compile_edge->inputs_.push_back(source_file);

  Edge* link_edge = state->AddEdge(cxx_linker);
  if (!state->AddOut(link_edge, executable->path(), 0, err_ptr)) {
    // Handle error
    *err_ptr = "Failed to add output to link edge\n";
    Error(error_message.c_str());
    return;
  }
  link_edge->inputs_.push_back(object_file);

  // Set up environment for compile edge
  if (compile_edge->env_ == nullptr) {
    compile_edge->env_ = new BindingEnv(&state->bindings_);
  }
  compile_edge->env_->AddBinding(
      "DEP_FILE", "CMakeFiles/hello_world.dir/hello_world.cpp.o.d");

  // Add default target
  if (!state->AddDefault(executable->path(), err_ptr)) {
    // Handle error
    *err_ptr = "Failed to add default target: \n";
    Error(error_message.c_str());
    return;
  }
}

void Error(const string& message) {
  cerr << "Error: " << message << endl;
}

int main() {
  State state;
  CreateHelloWorldGraph(&state);
  return 0;
}