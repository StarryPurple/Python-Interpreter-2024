#pragma once
#ifndef CODE_INTERPRETER_H
#define CODE_INTERPRETER_H

#include "BasicStructure.h"
// Every CodeInterpreter object holds the capacity of interpreting ONE code file.
class CodeInterpreter {

  std::vector<Function> functions_{}; // the global function storage
  std::map<std::string, int> function_map_{}; // name -> function ordinal
  CodeSuite *main_suite_{}; // MainFunction. Maybe redundant. // It is!
  CodeSuite *cur_suite_{};
  Function *cur_function_def_{}; // current function under definition. not nullptr only when funcdef_mode is on.
  int operation_cnt = 0;
  bool oper_cnt_mode = false;

public:
  bool funcdef_mode; // when it's on, new operations shouldn't be added to operations_.
  std::vector<Python3Type> shared_res_{}; // the global, shared storage for variables.
  static const Python3Type ConstTrue, ConstFalse, ConstNone;
  void ExecuteSingleOperation(const Operation &);
  void ExecuteOperations(const std::vector<Operation> &);
  CodeInterpreter();
  ~CodeInterpreter();
  Python3Type& FindVariable(CodeSuite *, const std::string &, bool &);
  // TODO: Warning: functions with same names are considered identical.
  Function* FindFunction(const std::string &);
  void DefineVariable(const std::string &, const Python3Type &);
  void AppendOperation(const Operation &);
  void StartFunctionDef(const std::string &);
  void EndFunctionDef();
  void StartOperationCount();
  int EndOperationCount();
  void RunNewOperations(CodeSuite *);
  void RunNewOperations();
  void Load(Python3Type);
  Python3Type PopResult();
  Operation ArithOperation(Operation::FunctionOperations);
  Operation SelfOperation(Operation::FunctionOperations);
  Operation StackPopOperation();
  Operation StackPushNoneOperation();
  Operation LoadOperation(Python3Type &);
  Operation WriteOperation(Python3Type &);
};


#endif // CODE_INTERPRETER_H