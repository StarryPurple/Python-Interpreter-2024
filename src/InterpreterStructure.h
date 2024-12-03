#ifndef INTERPRETER_STRUCTURE_H
#define INTERPRETER_STRUCTURE_H

#include "BasicOperation.h"
#include <utility>
#include <map>

// The variable map and variable space will be initially filled through funcdef,
// Then the object will be stored as a template,
// waiting for further call and special modification (as a code suite)
class FunctionSuite {
public:
  using Variable_Space = std::vector<std::any>;
  using Initialize_Pair = std::pair<std::string, std::any>; // name & initial value
  using Initialize_List = std::vector<Initialize_Pair>;
  static const std::string UnassignedName;

  // variables listed in the order that parameters are given
  std::map<std::string, int> variable_map{};
  Variable_Space variable_space{};

  FunctionSuite() = delete;
  FunctionSuite(const FunctionSuite &) = default;
  FunctionSuite(const Initialize_List &);
  void DefineVariable(const std::string &, const std::any &);
  std::any &LocalVariable(const std::string &);
  ~FunctionSuite() = default;
};

class PythonProject {
  std::vector<FunctionSuite> function_stack;
public:
  PythonProject() = default;
  void CallFunction(const FunctionSuite &);
  void ExitFunction();
  std::any &Variable(const std::string &);
};



#endif // INTERPRETER_STRUCTURE_H