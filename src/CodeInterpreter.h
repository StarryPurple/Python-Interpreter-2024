#pragma once
#ifndef CODE_INTERPRETER_H
#define CODE_INTERPRETER_H

#include "BasicStructure.h"

// Every CodeInterpreter object holds the capacity of interpreting ONE code file.
class CodeInterpreter {
  static const Python3Type ConstTrue, ConstFalse, ConstNone;

  std::vector<Operation> operations_{}; // global operation stack.

  std::vector<Function> functions_{}; // the global function storage
  std::map<std::string, int> function_map_{}; // name -> function ordinal
  Function *main_function_{}; // MainFunction.
  Function *cur_function_{}; // initially MainFunction.
  Function *cur_function_def_{nullptr}; // current function under definition. not nullptr only when funcdef_mode is on.

  Python3Type temp_var_{}; // the global, shared storage for variables.

  bool funcdef_mode; // when it's on, new operations shouldn't be added to operations_.

public:
  CodeInterpreter();
  ~CodeInterpreter();
  static const Python3Type& FindVariable(Function *, const std::string &, bool &);
  void DefineVariable(const std::string &, const Python3Type &);
  void InsertOperation(const Operation &);
  void StartFunctionDef(const std::string &);
  void EndFunctionDef();
  void RunMainFunction();
};


#endif // CODE_INTERPRETER_H