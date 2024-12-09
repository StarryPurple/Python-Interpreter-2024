#ifndef INTERPRETER_STRUCTURE_H
#define INTERPRETER_STRUCTURE_H

#include "BasicOperation.h"
#include <utility>
#include <unordered_map>

// The variable map and variable space will be initially filled through funcdef,
// Then the object will be stored as a template,
// waiting for further call and special modification (as a code suite)
class FunctionSuite {
public:
  using Variable_Space = std::unordered_map<std::string, std::any>;
  using Initialize_Pair = std::pair<std::string, std::any>; // name & initial value
  using Initialize_List = std::vector<Initialize_Pair>;
  static const std::string UnassignedName;
  bool break_sign = false, continue_sign = false, return_sign = false;
  std::string func_name;
  std::any result = Interpreter::Tuple();
  Initialize_List primitive_list;
  Variable_Space variable_space;

  FunctionSuite() = default;
  FunctionSuite(const FunctionSuite &) = default;
  FunctionSuite(const Initialize_List &);
  void DefineVariable(const std::string &, const std::any &);
  std::any &LocalVariable(const std::string &);
  ~FunctionSuite() = default;
};

class PythonProject {
  std::vector<FunctionSuite> function_stack;
public:
  PythonProject();
  void CallBreak();
  void EndBreak();
  void CallContinue();
  void EndContinue();
  void CallReturn();
  void EndReturn();
  void AddResult(const std::any&);
  std::any GiveResult();
  bool IsBreak() const;
  bool IsContinue() const;
  bool IsReturn() const;
  void CallFunction(const FunctionSuite &);
  void ExitFunction();
  std::any &Variable(const std::string &);
};



#endif // INTERPRETER_STRUCTURE_H